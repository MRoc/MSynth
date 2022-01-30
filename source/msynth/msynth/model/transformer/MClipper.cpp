#include "MClipper.h"

MRtti MClipper::gvRtti = MRtti( "MClipper", MClipper::createInstance );

MClipper::MClipper()
{
	ivPtTresholdControl = new MFloatControl( String( "Cut" ), 0.0f, 1.0f, 0.9f, false );
}

MClipper::~MClipper()	
{
	SAFE_DELETE( ivPtTresholdControl );
}

unsigned int MClipper::getControlCount()
{
	return 1;
}

IControl* MClipper::getControl( unsigned int index )
{
	return ivPtTresholdControl; 
}

void MClipper::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt )
{
	FP max = ivPtTresholdControl->getValue() * CLIP_MAX;
	for( unsigned int i=0;i<buffer->getChannelCount();i++ )
	{
		FP negMax = -max;
		FP *bufferStop = buffer->getData( i ) + stopAt;
		for( FP *pointer = buffer->getData( i )+startFrom; pointer<bufferStop; pointer++)
			if( (*pointer) > max )
				(*pointer) =  max;
			else if( (*pointer) < negMax )
				(*pointer) =  negMax;
	}
}

String MClipper::getType()
{
	return String("processor");
}

String	MClipper::getName()
{
	return String("Clipper");
}

String	MClipper::getShortName()
{
	return getName();
}

IRtti* MClipper::getRtti() const
{
	return &gvRtti;
}

MObject *MClipper::createInstance()
{
	return new MClipper();
}