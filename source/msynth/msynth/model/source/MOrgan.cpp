/**
 * (C) 2001-2003 MRoc
 */


#include "MOrgan.h"

MRtti MOrgan::gvRtti = MRtti( "MOrgan", MOrgan::createInstance );

const FP MOrgan::gvMulAdder[] = { 1.0f, 1.01f, -1.01f, 1.02f, -1.02f, 1.03f, -1.03f };

MOrgan::MOrgan()
{
	FP twoP = 0.5f;
	for( int i=0;i<SINECOUNT;i++ )
	{
		ivPtVolControls[ i ] = new MFloatControl(
			String( "Vol" ) + MUtil::toString( i + 1 ),
			0.0f, 2.0f, 1.0f / FP(i + 1), false );

		ivArMultiply[i] = twoP * gvMulAdder[i];
		twoP *= 2.0f;

		ivArPtOsc[ i ] = new MOscSine();
	}
}

MOrgan::~MOrgan()
{
	for( int i=0;i<SINECOUNT;i++ )
	{
		SAFE_DELETE( ivPtVolControls[ i ] );
		SAFE_DELETE( ivArPtOsc[ i ] );
	}
}

void MOrgan::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt )
{
	if( ivFreq == 0 )
		return;

	int bufferLength = stopAt - startFrom;
	MSoundBuffer *bufferTemp = new MSoundBuffer( 1, bufferLength );

	FP vol = ivPtVolumeControl->getValue();
	FP tempVol = 0;

	for( int i=0;i<SINECOUNT;i++ )
	{
		ivArPtOsc[ i ]->goNext( bufferTemp, 0, bufferLength );
		tempVol = vol * ivPtVolControls[ i ]->getValue();
		FP *pt = buffer->getData(0) + startFrom;
		FP *ptStop = buffer->getData(0) + stopAt;
		FP *source = bufferTemp->getData(0);

		for( ; pt<ptStop; pt++, source++)
			(*pt) += (*source) * tempVol;
	}

	delete bufferTemp;
}
	
void MOrgan::setFreq(FP freq)
{
	ivFreq = freq;
	for( int i=0;i<SINECOUNT;i++ )
		ivArPtOsc[i]->setFreq( freq * ivOctaveMultiplier * ivArMultiply[i] );
}

void MOrgan::addPitch(FP delta)
{
	setFreq( ivFreq + delta );
}

void MOrgan::updateFromFineTune()
{
	for( int i=0;i<SINECOUNT;i++ )
		((MFloatControl*)ivArPtOsc[i]->getControl(MOsc::TUNE))->setValue( ivPtTuneControl->getValue() );
}

void MOrgan::valueChanged( MControlListenerEvent *anEvent )
{
	IControl* pSrc = anEvent->getSource();
	if( pSrc == ivPtOctaveControl )
		updateOctaveMultiplier();
	else if( pSrc == ivPtTuneControl )
		updateFromFineTune();
}

void MOrgan::reset()
{
	for( int i=0;i<SINECOUNT;i++ )
		ivArPtOsc[i]->reset();
}

String	MOrgan::getName()
{
	return getRtti()->getClassName();
}

String	MOrgan::getShortName()
{
	return "Organ";
}

unsigned int MOrgan::getControlCount()
{
	return MOsc::getControlCount() + SINECOUNT;
}

IControl* MOrgan::getControl( unsigned int index )
{
	if( index < MOsc::getControlCount() )
	{
		if( index  == PHASE )
			return 0;
		return MOsc::getControl( index );
	}
	else
		return ivPtVolControls[ index - MOsc::getControlCount() ];

	return 0;
}

IRtti* MOrgan::getRtti() const
{
	return &gvRtti;
}

MObject* MOrgan::createInstance()
{
	return new MOrgan();
}
