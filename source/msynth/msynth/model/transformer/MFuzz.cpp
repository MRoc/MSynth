#include "MFuzz.h"

MRtti MFuzz::gvRtti = MRtti( "MFuzz", MFuzz::createInstance );

MFuzz::MFuzz() :
	MTransformer(),
	ivPtMixControl( 0 ),
	ivPtBoostControl( 0 ),
	ivTreshold( 0.0f ),
	ivBoost( 0.0f )
{
	ivPtMixControl = new MFloatControl( String( "Mix" ), 0.0f, 1.0f, 1.0f, false );
	ivPtBoostControl = new MFloatControl( String( "Boost" ), 0.0f, 0.99f, 0.6f, true );
	ivPtBoostControl->addControlListener( this );
	updateBoostCoeffs();
}

MFuzz::~MFuzz()
{
	SAFE_DELETE( ivPtMixControl );
	ivPtBoostControl->removeControlListener( this );
	SAFE_DELETE( ivPtBoostControl );
}

unsigned int MFuzz::getControlCount()
{
	return 2;
}

IControl* MFuzz::getControl( unsigned int index )
{
	switch( index )
	{
		case MIX: return ivPtMixControl; break;
		case BOOST: return ivPtBoostControl; break;
		default: return 0; break;
	};
}

void MFuzz::valueChanged( MControlListenerEvent *anEvent )
{
	if( anEvent->getSource() == this->ivPtBoostControl )
		updateBoostCoeffs();
}

void MFuzz::updateBoostCoeffs()
{
	ivTreshold = 1.0f - ivPtBoostControl->getValue();
	ivBoost = 1.0f / ivTreshold;
}

void MFuzz::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt )
{
	FP wetMix = ivPtMixControl->getValue() * ivBoost;
	FP dryMix = 1 - ivPtMixControl->getValue();

	for( unsigned int i=0;i<buffer->getChannelCount();i++ )
	{
		FP *pt = buffer->getData(i) + startFrom;
		FP *ptStop = buffer->getData(i) + stopAt;
		for( ; pt< ptStop;pt++ )
		{
			FP back = (*pt);
		
			if( back > ivTreshold )
				back = ivTreshold;
			else if( back < -ivTreshold )
				back = -ivTreshold;
		
			(*pt) = (*pt) * dryMix + wetMix * back;
		}
	}
}

String MFuzz::getName()
{
	return String("Fuzz");
}

String MFuzz::getShortName()
{
	return String("Fuzz");
}

IRtti* MFuzz::getRtti() const
{
	return &gvRtti;
}

void* MFuzz::getInterface( const String &className ) const
{
	if( className == "MFuzz" )
		return (void*) ((MFuzz*)this);
	else if( className == "IControlListener" )
		return (void*) ((IControlListener*)this);
	else
		return MTransformer::getInterface( className );
}

MObject* MFuzz::createInstance()
{
	return new MFuzz();
}