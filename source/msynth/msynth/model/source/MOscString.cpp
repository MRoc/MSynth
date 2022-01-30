/**
 * (C) 2001-2003 MRoc
 */

#include "MOscString.h"

const FP MOscString::gvDetune[ SAWCOUNT ] =
	{ 1.0f, 2.0f, 3.99f, 8.01f, 15.99f, 32.f, 64.0f };//{ 1.0, 1.001, 1.002, 1.003, 1.004, 1.005, 1.006 };//

MRtti MOscString::gvRtti = MRtti( "MOscString", MOscString::createInstance );

MOscString::MOscString()
{
	TRACE("<CREATING MOscString>\n");
	reset();
	setFreq(0);

	for( int i=0;i<SAWCOUNT;i++ )
		ivPtVolControls[ i ] = new MFloatControl(
			String( "Vol" ) + MUtil::toString( i + 1 ),
			0.0f, 2.0f, 1.0f, false );
}

MOscString::~MOscString()
{
	for( int i=0;i<SAWCOUNT;i++ )
		SAFE_DELETE( ivPtVolControls[ i ] );
}

unsigned int MOscString::getControlCount()
{
	return MOsc::getControlCount() + SAWCOUNT;
}

IControl* MOscString::getControl( unsigned int index )
{
	if( index < MOsc::getControlCount() )
	{
		if( index == PHASE )
			return 0;
		return MOsc::getControl( index );
	}
	else
		return ivPtVolControls[ index - MOsc::getControlCount() ];
}

void MOscString::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt )
{
	if( ivFreq == 0 )
		return;

	FP *bufferStop = buffer->getData(0) + stopAt;

	FP vol = ivPtVolumeControl->getValue();
	FP tempVols[SAWCOUNT];
	for ( int s=0; s<SAWCOUNT; s++ )
		tempVols[ s ] = ivPtVolControls[ s ]->getValue();

	if( ivIsPortamento )
	{
		FP portamentoDelta = (ivPortamentoTargetFrequence - ivFreq) / (ivPortamentoLength - ivPortamentoStep);
		FP fineTune = ivPtTuneControl->getValue();
		for( FP *bufferPointer = buffer->getData(0) + startFrom; bufferPointer < bufferStop; bufferPointer++)
		{
			(*bufferPointer) = 0.0;
			for ( s=0; s<SAWCOUNT; s++ )
			{
				saw[s] += gvDetune[s] * d_delta - (int)saw[s];
				(*bufferPointer) += ((saw[s] - 0.5f) / (1+2*s)) * tempVols[s];
			}
			(*bufferPointer) *= vol;
			if( ivPortamentoStep < ivPortamentoLength )
			{	
				addPitch( portamentoDelta );
				ivPortamentoStep++;
			}
			else
			{
				stopPortamento();
			}
		}
	}
	else
	{
		for( FP *bufferPointer = buffer->getData(0)  + startFrom; bufferPointer < bufferStop; bufferPointer++)
		{
			(*bufferPointer) = 0.0;
			for ( int s=0; s<SAWCOUNT; s++ )
			{
				saw[s] += gvDetune[s] * d_delta - (int)saw[s];
				(*bufferPointer) += ((saw[s] - 0.5f) / FP(1+2*s)) * tempVols[s];
			}
			(*bufferPointer) *= vol;
		}
	}
}

void MOscString::setFreq(FP freq)
{
	d_osc = 0;
	ivFreq = freq;
	d_delta	= (ivFreq * ivOctaveMultiplier * ivPtTuneControl->getValue()) / MW_SAMPLINGRATE;
}

void MOscString::addPitch(FP delta)
{
	ivFreq += delta;
	d_delta	= (ivFreq * ivOctaveMultiplier * ivPtTuneControl->getValue()) / MW_SAMPLINGRATE;
}

void MOscString::reset()
{
	d_osc = 0;
	ivFreq = 0;

	for ( int i=0; i<SAWCOUNT; i++ )
	{
		saw[i] = FP(( rand() & 255 ) / 255.0f);
	}
}

String	MOscString::getName()
{
	return getRtti()->getClassName();
}

String MOscString::getShortName()
{
	return "String";
}

IRtti* MOscString::getRtti() const
{
	return &gvRtti;
}

MObject* MOscString::createInstance()
{
	return new MOscString();
}
