/**
 * (C)2000-2003 MRoc hifiShock
 */

#include "MOscSquare.h"

MRtti MOscSquare::gvRtti = MRtti( "MOscSquare", MOscSquare::createInstance );

MOscSquare::MOscSquare()
{
	ivPtWidthControl = new MFloatControl( "Width", 0.5f, 0.99f,	0.5f, false );
	reset();
}

MOscSquare::~MOscSquare()
{
	delete ivPtWidthControl;
}

unsigned int MOscSquare::getControlCount()
{
	return MOsc::getControlCount() + MSQUARE_CONTROL_COUNT;
}

IControl* MOscSquare::getControl( unsigned int index )
{
	if( index < MOsc::getControlCount() )
		return MOsc::getControl( index );
	else
		return ivPtWidthControl;
}

void MOscSquare::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt )
{
	if( ivFreq == 0 )
		return;

	FP *pt = buffer->getData(0) + startFrom;
	FP *ptStop = buffer->getData(0) + stopAt;

	FP vol = ivPtVolumeControl->getValue();
	FP width = getWidth();

	if( ivIsPortamento )
	{
		FP portamentoDelta = (ivPortamentoTargetFrequence - ivFreq) / (ivPortamentoLength - ivPortamentoStep);
		for( ; pt<ptStop; pt++ )
		{
			if( d_osc > 0 )
			{
				if( (++d_step) > (d_sqtune*width))
				{
					d_osc	= -vol;
					d_step	= 0;
				}
			}
			else
			{
				if( (++d_step) > (d_sqtune*(1.0-width)))
				{
					d_osc	= vol;
					d_step	= 0;
				}
			}
			(*pt) = d_osc;
			
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
		FP w1 = d_sqtune * width;
		FP w2 = d_sqtune * (1-width);
		unsigned int stepsToRender = stopAt - startFrom;
		unsigned int stepsRendered = 0;

		for( ; pt<ptStop; pt++,d_step++)
		{
			if( d_osc > 0 )
			{
				if( d_step > w1)
				{
					d_osc	= -vol;
					d_step	= 0;
				}
			}
			else
			{
				if( d_step > w2)
				{
					d_osc	= vol;
					d_step	= 0;
				}
			}
			(*pt) = d_osc;
		}
	}
}

void MOscSquare::reset()
{
	d_osc = 0;
	ivFreq = 0;
	d_step = 0;
	d_sqtune = 0;
}

void MOscSquare::setFreq(FP freq)
{
	d_osc = 0;
	ivFreq = freq;
	d_sqtune = MW_SAMPLINGRATE / ( ivFreq * ivOctaveMultiplier * ivPtTuneControl->getValue() );
	d_step = d_sqtune * ivPtPhaseControl->getValue();
}

void MOscSquare::addPitch(FP delta)
{
	ivFreq += delta;
	d_sqtune = ( MW_SAMPLINGRATE / ( ivFreq * ivOctaveMultiplier * ivPtTuneControl->getValue() ) );
}

void MOscSquare::setWidth(FP width)
{
	ivPtWidthControl->setValue( width );
}

FP MOscSquare::getWidth()
{
	return ivPtWidthControl->getValue();
}
		
String	MOscSquare::getName()
{
	return getShortName();
}

String MOscSquare::getShortName()
{
	return "Square";
}

void MOscSquare::fillBuffer( FP* dataTemp, unsigned int startAt, unsigned int stopAt, FP value )
{
	FP *bufferStop = dataTemp + startAt + stopAt;
	for( FP* pt=dataTemp+startAt;pt<bufferStop;pt++ )
		(*pt) = value;
}

IRtti* MOscSquare::getRtti() const
{
	return &gvRtti;
}

MObject* MOscSquare::createInstance()
{
	return new MOscSquare();
}
