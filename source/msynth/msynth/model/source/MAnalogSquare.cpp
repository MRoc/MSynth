#include "MAnalogSquare.h"
#include "../interfaces/MSoundBufferUtils.h"

MRtti MAnalogSquare::gvRtti = MRtti( "MAnalogSquare", MAnalogSquare::createInstance );

MAnalogSquare::MAnalogSquare() :
	ivSineCount( 10 ),
	ivPtSines( 0 )
{
	ivPtSines = new MOscSine*[ ivSineCount ];
	for( unsigned int i=0;i<ivSineCount;i++ )
		ivPtSines[i] = new MOscSine();
}

MAnalogSquare::~MAnalogSquare()
{
	if( ivPtSines )
	{
		for( unsigned int i=0;i<ivSineCount;i++ )
			delete ivPtSines[i];
		delete ivPtSines;
	}
}

void MAnalogSquare::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt )
{
	unsigned int samples = stopAt - startFrom;
	MSoundBuffer sinebuffer = MSoundBuffer( 1, samples );
	MSoundBuffer mixbuffer = MSoundBuffer( 1, samples );
	for(unsigned int i=0;i<ivSineCount;i++ )
	{
		ivPtSines[i]->goNext( &sinebuffer, 0, samples );
		MSoundBufferUtils::multiply( &sinebuffer, 1.0f/((i+1.0f)*2.0f-1.0f) );
		MSoundBufferUtils::add( &mixbuffer, &sinebuffer, 0, mixbuffer.getDataLength() );
	}
	MSoundBufferUtils::multiply( &mixbuffer, ivPtVolumeControl->getValue() );
	FP* ptTarget = buffer->getData(0) + startFrom;
	FP* ptStart = mixbuffer.getData(0);
	FP* ptStop = ptStart + samples;
	for( FP* pt = ptStart; pt<ptStop; pt++, ptTarget++ )
		(*ptTarget) = (*pt);
}
void MAnalogSquare::reset()
{
	for( unsigned int i=0;i<ivSineCount;i++ )
		ivPtSines[i]->reset();
}

void MAnalogSquare::setFreq( FP freq )
{
	for( unsigned int i=0;i<ivSineCount;i++ )
	{
		int multi = ((i+1)*2 - 1);
		ivPtSines[i]->setFreq( freq * multi );
	}
}

void MAnalogSquare::addPitch( FP delta)
{
}

String MAnalogSquare::getName()
{
	return "MAnalogSquare";
}

String MAnalogSquare::getShortName()
{
	return "analog square";
}

IRtti* MAnalogSquare::getRtti() const
{
	return &gvRtti;
}

MObject* MAnalogSquare::createInstance()
{
	return new MAnalogSquare();
}

unsigned int MAnalogSquare::getControlCount()
{
	return 3;
}

IControl* MAnalogSquare::getControl( unsigned int index )
{
	IControl* back = 0;
	switch( index )
	{
		case OCTAVE: back = ivPtOctaveControl; break;
		case VOLUME: back = ivPtVolumeControl; break;
		case TUNE: back = ivPtTuneControl; break;
	};
	return back;
}