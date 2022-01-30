/**
 * (C) 2000-2003 MRoc
 */

#include "MOscSaw.h"

MRtti MOscSaw::gvRtti = MRtti( "MOscSaw", MOscSaw::createInstance );

MOscSaw::MOscSaw()
{
	reset();
}

MOscSaw::~MOscSaw()
{
}

void MOscSaw::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt )
{
	if( ivFreq == 0 )
		return;

	FP *pt = buffer->getData(0) + startFrom;
	FP *ptStop = buffer->getData(0) + stopAt;

	FP vol = ivPtVolumeControl->getValue();

	if( ivIsPortamento )
	{
		FP portamentoDelta = (ivPortamentoTargetFrequence - ivFreq ) / (ivPortamentoLength - ivPortamentoStep);
		for( ;pt<ptStop; pt++ )
		{
			if(( d_osc += d_delta ) > 1.0)
				d_osc = -1;
			if( ivPortamentoStep < ivPortamentoLength )
			{	
				addPitch( portamentoDelta );
				ivPortamentoStep++;
			}
			else
				stopPortamento();

			(*pt) = d_osc * vol;
		}
	}
	else
	{
		for( ;pt<ptStop; pt++ )
		{
			if(( d_osc += d_delta ) > 1.0)
				d_osc = -1;
			(*pt) = d_osc * vol;
		}
	}
}

void MOscSaw::reset()
{
	d_osc = 0;
	ivFreq = 0;
}

void MOscSaw::setFreq( FP freq )
{
	ivFreq = freq;
	FP freqTemp = ivFreq * ivPtTuneControl->getValue() * ivOctaveMultiplier;
	d_delta	= 2 * freqTemp / MW_SAMPLINGRATE;
	d_osc = ( MW_SAMPLINGRATE / freqTemp * d_delta ) * ivPtPhaseControl->getValue();
}

void MOscSaw::addPitch( FP delta )
{
	ivFreq += delta;
	d_delta	= 2 * (ivFreq * ivOctaveMultiplier * ivPtTuneControl->getValue()) / MW_SAMPLINGRATE;
}

String	MOscSaw::getName()
{
	return getRtti()->getClassName();
}

String	MOscSaw::getShortName()
{
	return String("Saw");
}

IRtti* MOscSaw::getRtti() const
{
	return &gvRtti;
}

MObject* MOscSaw::createInstance()
{
	return new MOscSaw();
}	
