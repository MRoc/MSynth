/*

	MPeakEq (C)2001 MRoc hifiShock

	version 0.1 design
	version 0.2 working

*/

#include "MPeakEq.h"

MRtti MPeakEq::gvRtti = MRtti( "MPeakEq", MPeakEq::createInstance );

const int MPeakEq::gvSectionCount = 7;
const FP MPeakEq::gvFrequencies[] = {80.0f, 120.0f, 240.0f, 1000.0f, 4000.0f, 8000.0f, 16000.0};

const FP MPeakEq::LOG10 = (FP) log( 10.0f );
const FP MPeakEq::TWOPI = 2.0f * PI;

MPeakEq::MPeakEq()
{
}

MPeakEq::MPeakEq( int channelCount )
{
	ivSectionCount = 2;
	ivArPtSections = new MEqSection*[ivSectionCount];
	ivArPtGainControls = new MFloatControl*[ivSectionCount];
	for( int i=0;i<2;i++ )
	{
		char buffer[1024];
		sprintf( buffer, "dB %iHz", i==0 ? 90 : 12000 );
		ivArPtGainControls[i] = new MFloatControl(
			String( "f" ) + MInt::toString( int(gvFrequencies[i]) ) + String( "Hz" ),
			-12.0f,
			+12.0f,
			0.0f,
			false,
			MFloatControl::CURVE_LINEAR,
			String( buffer ),
			1.0f );

		ivArPtGainControls[ i ]->addControlListener( this );

		ivArPtSections[i] = new MEqSection();
		ivArPtSections[i]->gain = 0.0f;
		ivArPtSections[i]->q = 1.0f;

		ivArPtSections[i]->coeffs.a0 = 0.0f;
		ivArPtSections[i]->coeffs.a1 = 0.0f;
		ivArPtSections[i]->coeffs.a2 = 0.0f;
		ivArPtSections[i]->coeffs.b1 = 0.0f;
		ivArPtSections[i]->coeffs.b2 = 0.0f;
		ivArPtSections[i]->stacks = 0;
		ivArPtSections[i]->stackCount = 0;
	}

	ivArPtSections[ 0 ]->frequence = 90;
	ivArPtSections[ 1 ]->frequence = 12000;

	calculateFilterCoefficients();
	initFx( channelCount );
}

bool MPeakEq::create( unsigned int channelCount )
{
//	ASSERT( ivSectionCount < 10 );
	ivSectionCount = gvSectionCount;
	ivArPtSections = new MEqSection*[ivSectionCount];
	ivArPtGainControls = new MFloatControl*[ivSectionCount];
	for( unsigned int i=0;i<ivSectionCount;i++ )
	{
		ivArPtGainControls[i] = new MFloatControl(
			String( "f" ) + MInt::toString( int(gvFrequencies[i]) ) + String( "Hz" ),
			-12.0f,
			+12.0f,
			0.0f,
			false,
			MFloatControl::CURVE_LINEAR,
			String( "dB" ),
			1.0f );

		ivArPtGainControls[ i ]->addControlListener( this );

		ivArPtSections[i] = new MEqSection();
		ivArPtSections[i]->frequence = gvFrequencies[i];
		ivArPtSections[i]->gain = 0.0f;
		ivArPtSections[i]->q = 1.0f;

		ivArPtSections[i]->coeffs.a0 = 0.0f;
		ivArPtSections[i]->coeffs.a1 = 0.0f;
		ivArPtSections[i]->coeffs.a2 = 0.0f;
		ivArPtSections[i]->coeffs.b1 = 0.0f;
		ivArPtSections[i]->coeffs.b2 = 0.0f;
		ivArPtSections[i]->stacks = 0;
		ivArPtSections[i]->stackCount = 0;
	}

	calculateFilterCoefficients();
	initFx( channelCount );

	return true;
}

MPeakEq::~MPeakEq()
{
	for( unsigned int i=0;i<ivSectionCount;i++ )
	{
		ivArPtGainControls[ i ]->removeControlListener( this );
		SAFE_DELETE( ivArPtGainControls[ i ] );
		if( ivArPtSections[ i ]->stacks )
		{
			for( int j=0;j<ivArPtSections[ i ]->stackCount;j++ )
				delete ivArPtSections[ i ]->stacks[ j ];
			delete ivArPtSections[ i ]->stacks;
		}
		delete ivArPtSections[i];
	}
	delete ivArPtGainControls;
	delete ivArPtSections;
};

unsigned int MPeakEq::getControlCount()
{
	return ivSectionCount;
}

IControl* MPeakEq::getControl( unsigned int index )
{
	ASSERT( index < ivSectionCount );
	return ivArPtGainControls[ index ];
}

void MPeakEq::valueChanged( MControlListenerEvent *anEvent )
{
	for( unsigned int i=0;i<ivSectionCount;i++ )
		if( anEvent->getSource() == ivArPtGainControls[i] )
		{
			calculateFilterCoefficients( i );
			break;
		}
}

void MPeakEq::initFx( int _channelCount )
{
	this->ivChannelCount = _channelCount;
	if( ivArPtSections )
		for( unsigned int i=0;i<ivSectionCount;i++ )
		{
			if( ivArPtSections[ i ]->stacks )
			{
				for( int j=0;j<ivArPtSections[ i ]->stackCount;j++ )
					delete ivArPtSections[ i ]->stacks[ j ];
				delete ivArPtSections[ i ]->stacks;
			}
			ivArPtSections[ i ]->stackCount = ivChannelCount;
			ivArPtSections[ i ]->stacks = new MBiquadStack*[2];
			for( int j=0;j<ivArPtSections[ i ]->stackCount;j++ )
				ivArPtSections[ i ]->stacks[ j ] = new MBiquadStack();
		}

	processEvent( &MEvent( MEvent::RESET ) );
}

void MPeakEq::processEvent( MEvent* ptEvent )
{
	if( ptEvent->getType() == MEvent::RESET )
	{
		MEqSection* curSec = 0;
		MBiquadStack* curStack = 0;
		for( unsigned int i=0;i<ivSectionCount;i++ )
		{
			curSec = ivArPtSections[ i ];
			for( int j=0;j<curSec->stackCount;j++)
			{
				curStack = curSec->stacks[ j ];
				curStack->x0 = 0.0f;
				curStack->x1 = 0.0f;
				curStack->x2 = 0.0f;
				curStack->y0 = 0.0f;
				curStack->y1 = 0.0f;
				curStack->y2 = 0.0f;
			}
		}
	}
}

void MPeakEq::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt )
{
	ASSERT( buffer );
	ASSERT( startFrom <= stopAt );

	MEqSection* section = 0;
	for( unsigned int c=0;c<buffer->getChannelCount();c++ )
		for( unsigned int s=0;s<ivSectionCount;s++ )
		{
			section = ivArPtSections[ s ];
			ivBiquad.goNext(
				&(section->coeffs),
				section->stacks[ c ],
				buffer->getData( c ) + startFrom,
				(stopAt-startFrom) );
		}
}

void MPeakEq::calculateFilterCoefficients()
{
	for( unsigned int i=0;i<ivSectionCount;i++ )
		calculateFilterCoefficients( i );
};

void MPeakEq::calculateFilterCoefficients( unsigned int index )
{
	MEqSection* section = ivArPtSections[ index ];
	MBiquadCoeffs* coeffs = &( section->coeffs );

	FP gain = ivArPtGainControls[ index ]->getValue();
	FP A = (FP) exp( gain/40.0f * LOG10 );
	FP omega = ( TWOPI * section->frequence / MW_SAMPLINGRATE );
	FP sn = (FP) sin( omega );
	FP cs = (FP) cos( omega );
	FP alpha = sn / ( 2.0f * section->q );
	FP alphaMulA = alpha * A;
	FP alphaDivA = alpha / A;
	FP b0 = 1.0f + alphaMulA;
	FP b1 = -2.0f * cs;
	FP b2 = 1.0f - alphaMulA;
	FP a0 = 1.0f + alphaDivA;
	FP a1 = b1;
	FP a2 = 1.0f - alphaDivA;

	coeffs->a0 = (b0/a0);
	coeffs->a1 = (b1/a0);
	coeffs->a2 = (b2/a0);
	coeffs->b1 = (a1/a0);
	coeffs->b2 = (a2/a0);
}

String MPeakEq::getName()
{
	return String("Equalizer");
}

String MPeakEq::getShortName()
{
	return String("Equalizer");
}

String MPeakEq::toString()
{
	return getName();
}

IRtti* MPeakEq::getRtti() const
{
	return &gvRtti;
}

void* MPeakEq::getInterface( const String &className ) const
{
	if( className == "MPeakEq" )
		return (void*) ((MPeakEq*)this);
	if( className == "IControlListener" )
		return (void*) ((IControlListener*)this);
	else
		return MTransformer::getInterface( className );
}

MObject* MPeakEq::createInstance()
{
	return new MPeakEq();
}