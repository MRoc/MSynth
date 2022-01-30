/*

	MNoise (C)2000/2001 MRoc hifiShock

*/

#include "MNoise.h"

/**
 * the runtime type info
 */
MRtti MNoise::gvRtti = MRtti( "MNoise", MNoise::createInstance );

#define quad( a ) ((a)*(a))

//-----------------------------------------------------------------------------	
// + construction
//-----------------------------------------------------------------------------	
MNoise::MNoise()
{
	reset();

	/**
	 * filter setting for brown noise
	 */
	double freq = 16.0f;
	double Ofreq = PI * ( freq / MW_SAMPLINGRATE );
	double cosO = cos( Ofreq );
	double b = sqrt( quad(2.0-cosO) - 1.0 ) - 2.0 + cosO;
	double a = 1 + b;
}

//-----------------------------------------------------------------------------	
// + destruction
//-----------------------------------------------------------------------------	
MNoise::~MNoise()
{
}

//-----------------------------------------------------------------------------	
// + goNext
//-----------------------------------------------------------------------------	
void MNoise::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt )
{
	FP vol = ivPtVolumeControl->getValue() / MW_MAX;
	FP* pt = buffer->getData(0) + startFrom;
	FP *ptStop = buffer->getData(0) + stopAt;
	for( ; pt < ptStop ; pt++ )
	{
		(*pt) = FP( ( ivRandomizer.next() % 0xFFFF ) ) * vol;
	}
	ivFilter.filter( buffer->getData( 0 ), stopAt - startFrom );
}

//-----------------------------------------------------------------------------	
// + setFreq
//-----------------------------------------------------------------------------	
void MNoise::setFreq(FP freq)
{
}

//-----------------------------------------------------------------------------	
// + addPitch
//-----------------------------------------------------------------------------	
void MNoise::addPitch(FP delta)
{
}

//-----------------------------------------------------------------------------	
// + setFineTune
//-----------------------------------------------------------------------------	
void MNoise::setFineTune(FP tune)
{
}

//-----------------------------------------------------------------------------	
// + getFineTune
//-----------------------------------------------------------------------------	
FP MNoise::getFineTune()
{
		return 1;
}

//-----------------------------------------------------------------------------	
// + reset
//-----------------------------------------------------------------------------	
void MNoise::reset()
{
}

//-----------------------------------------------------------------------------	
// + getName
//-----------------------------------------------------------------------------	
String	MNoise::getName()
{
	return String("Noise");
}

//-----------------------------------------------------------------------------	
// + getShortName
//-----------------------------------------------------------------------------	
String	MNoise::getShortName()
{
	return getName();
}

unsigned int MNoise::getControlCount()
{
	return 2;
}

IControl* MNoise::getControl( unsigned int index )
{
	if( index == VOLUME )
		return ivPtVolumeControl;
	else
		return 0;
}	

IRtti* MNoise::getRtti() const
{
	return &gvRtti;
}

//-----------------------------------------------------------------------------	
// + createInstance
//-----------------------------------------------------------------------------	
MObject* MNoise::createInstance()
{
	return new MNoise();
}	
