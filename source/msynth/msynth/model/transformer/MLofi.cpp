#include "MLofi.h"

MRtti MLofi::gvRtti = MRtti( "MLofi", MLofi::createInstance );

MLofi::MLofi()
{
	ivPtMixControl = new MFloatControl( String( "Mix" ), 0.0f, 1.0f, 1.0f, false );
}

MLofi::~MLofi()
{
	SAFE_DELETE( ivPtMixControl );
}

unsigned int MLofi::getControlCount()
{
	return 1;
}

IControl* MLofi::getControl( unsigned int index )
{
	ASSERT( index == 0 );
	return ivPtMixControl;
}

void MLofi::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt )
{
	FP wetMix = ivPtMixControl->getValue();
	FP dryMix = 1 - wetMix;

	for( unsigned int i=0;i<buffer->getChannelCount();i++ )
	{
		FP *bufferStop = buffer->getData(i) + stopAt;
		for( FP *pointer=buffer->getData(i) + startFrom; pointer<bufferStop; pointer++ )
		{
			FP input = (*pointer);
			if( input > 1)
				input = 1;
			else if(input < -1)
				input = -1;

			signed char cInput = signed char(input * 127);

			(*pointer) = (*pointer) * dryMix + wetMix * FP(cInput) / 127;
		}
	}
}

String MLofi::getName()
{
	return String( "Lofi");
}

String MLofi::getShortName()
{
	return getName();
}

IRtti* MLofi::getRtti() const
{
	return &gvRtti;
}

MObject* MLofi::createInstance()
{
	return new MLofi();
}
