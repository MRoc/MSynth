/**
 * (C) 2000-2003 MRoc
 */

#include "MOscTriangle.h"

MRtti MOscTriangle::gvRtti = MRtti( "MOscTriangle", MOscTriangle::createInstance );

MOscTriangle::MOscTriangle()
{
	reset();
}

MOscTriangle::~MOscTriangle()
{
}

void MOscTriangle::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt )
{
	if( ivFreq == 0 )
	{
		return;
	}

	FP *bufferStop = buffer->getData(0) + stopAt;
	FP vol = ivPtVolumeControl->getValue();

	if( ivIsPortamento )
	{
		FP portamentoDelta = (ivPortamentoTargetFrequence - ivFreq) / (ivPortamentoLength - ivPortamentoStep);
		for( FP *bufferPointer = buffer->getData(0) + startFrom; bufferPointer < bufferStop; bufferPointer++)
		{
			d_osc += ( d_delta * 2 );
			if( (d_osc > 1.0) || (d_osc < -1.0) )
			{
				d_delta *= -1;
			}
			if( ivPortamentoStep < ivPortamentoLength )
			{	
				addPitch( portamentoDelta );
				ivPortamentoStep++;
			}
			else
			{
				stopPortamento();
			}
			(*bufferPointer) = d_osc * vol;
		}
	}
	else
	{
		for( FP *bufferPointer = buffer->getData(0) + startFrom; bufferPointer < bufferStop; bufferPointer++)
		{
			d_osc += (d_delta*2);
			if( (d_osc > 1.0) || (d_osc < -1.0) )
				d_delta *= -1;
	
			(*bufferPointer) = d_osc * vol;
		}
	}
}

void MOscTriangle::setFreq(FP freq)
{
	ivFreq = freq;
	FP tempFreq = ivFreq * ivOctaveMultiplier * ivPtTuneControl->getValue();
	d_delta	= 2 * tempFreq / MW_SAMPLINGRATE;
	d_osc = ( ( d_delta / 2 ) * ( MW_SAMPLINGRATE / tempFreq ) ) * ivPtPhaseControl->getValue();
}

void MOscTriangle::addPitch(FP delta)
{
	ivFreq += delta;
	if(d_delta > 0)			
		d_delta	= 2 * (ivFreq * ivOctaveMultiplier * ivPtTuneControl->getValue()) / MW_SAMPLINGRATE;
	else
		d_delta	=  (-2) * (ivFreq * ivOctaveMultiplier * ivPtTuneControl->getValue()) / MW_SAMPLINGRATE;
}

void MOscTriangle::reset()
{
	d_osc = 0;
	ivFreq = 0;
}

String	MOscTriangle::getName()
{
	return getRtti()->getClassName();
}

String	MOscTriangle::getShortName()
{
	return "Triangle";
}

IRtti* MOscTriangle::getRtti() const
{
	return &gvRtti;
}

MObject* MOscTriangle::createInstance()
{
	return new MOscTriangle();
}
