/**
 * (C) 2000-2003 MRoc
 */

#include "MOscSine.h"

MRtti MOscSine::gvRtti = MRtti( "MOscSine", MOscSine::createInstance );

MOscSine::MOscSine() :
	MOsc(),
	ivX( 1.0f ),
	ivY( 0.0f ),
	ivCos( 0.0f ),
	ivSin( 0.0f )
{
}

MOscSine::~MOscSine()
{
}

void MOscSine::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt )
{
	if( ivFreq == 0 )
	{
		return;
	}

	FP *bufferStop = buffer->getData(0) + stopAt;
	FP temp = 0;

	FP vol = ivPtVolumeControl->getValue();

	double
		xTmp = ivX,
		yTmp = ivY;

	if( ivIsPortamento )
	{
		FP portamentoDelta = (ivPortamentoTargetFrequence - ivFreq) / (ivPortamentoLength - ivPortamentoStep);
		for( FP *bufferPointer = buffer->getData(0) + startFrom; bufferPointer < bufferStop; bufferPointer++)
		{
			xTmp = ivX;
			yTmp = ivY;
			ivX = ivCos*xTmp + ivSin*yTmp;
			ivY = ivCos*yTmp - ivSin*xTmp;

			(*bufferPointer) = FP(ivY) * vol;

			if( ivPortamentoStep < ivPortamentoLength )
			{	
				addPitch( portamentoDelta );
				ivPortamentoStep++;
			}
			else
				stopPortamento();
		}
	}
	else
	{
		for( FP *pt = buffer->getData(0) + startFrom; pt < bufferStop; pt++)
		{
			xTmp = ivX;
			yTmp = ivY;
			ivX = ivCos*xTmp + ivSin*yTmp;
			ivY = -ivSin*xTmp + ivCos*yTmp;
			(*pt) = FP(ivY) * vol;
		}
	}
}

void MOscSine::reset()
{
	ivX = 1.0f;
	ivY = 0.0f;
	ivSin = 0.0f;
	ivCos = 0.0f;
}

void MOscSine::setFreq( FP freq )
{
	ivFreq = freq;
	FP tmp = ivFreq * ivOctaveMultiplier * ivPtTuneControl->getValue() * 2 * PI / MW_SAMPLINGRATE;

	ivSin = sin( -tmp );
	ivCos = cos( -tmp );
}

void MOscSine::addPitch( FP delta )
{
	ivFreq += delta;
	FP tmp = ivFreq * ivOctaveMultiplier * ivPtTuneControl->getValue() * 2 * PI / MW_SAMPLINGRATE;
	ivSin = -sin( -tmp );
	ivCos = cos( -tmp );
}

String MOscSine::getName()
{
	return getRtti()->getClassName();
}

String	MOscSine::getShortName()
{
	return "Sine";
}

IRtti* MOscSine::getRtti() const
{
	return &gvRtti;
}

MObject* MOscSine::createInstance()
{
	return new MOscSine();
}
