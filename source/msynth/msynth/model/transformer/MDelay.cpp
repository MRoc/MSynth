/*

	MDelay (C)2000-2003 MRoc hifiShock


	v. 0.1	-	design
	v. 0.2	-	optimized interpolation (ceil->cast)
	v. 0.3	-	descripted
	v. 0.4	-	removed interpolation because its senseless (samplingRate 44100Hz, no interpolation needed)
	v. 0.5  -	implementing MObject...

*/
#include "MDelay.h"

//delay time in milliseconds
#define MAX_DELAY_TIME 1021
#define DEFAULT_MIX 0.5f
#define DEFAULT_DELAYTIME 300.0f
#define DEFAULT_FEEDBACK 0.5f

MRtti MDelay::gvRtti = MRtti( "MDelay", MDelay::createInstance );

MDelay::MDelay() :
	ivPtMix( 0 ),
	ivPtFeedback( 0 ),
	ivPtDelayTime( 0 ),
	ivPtDelayBuffer( 0 ),
	ivStep( 0 ),
	ivBufferLength( 0 )
{
}

MDelay::~MDelay()
{
	SAFE_DELETE( ivPtMix );
	SAFE_DELETE( ivPtFeedback );
	SAFE_DELETE( ivPtDelayTime );

	TRACE("<Deleting MDelay>\n");
	for( unsigned int i=0;i<ivChannelCount;i++ )
		SAFE_DELETE( ivPtDelayBuffer[ i ] );
	SAFE_DELETE( ivPtDelayBuffer );
}

bool MDelay::create( unsigned int channelCount )
{
	ivChannelCount = channelCount;
	ivBufferLength = int( MAX_DELAY_TIME * MW_SAMPLINGRATE / 1000 );
	ivPtDelayBuffer = new FP*[ ivChannelCount ];
	for( unsigned int i=0;i<ivChannelCount;i++ )
		ivPtDelayBuffer[ i ] = new FP[ ivBufferLength ];

	ivPtMix = new MFloatControl( String( "Mix" ), 0.0f, 1.0f, DEFAULT_MIX, false );
	ivPtFeedback = new MFloatControl( String( "Feedback" ), 0.0f, 0.9f, DEFAULT_FEEDBACK, false );
	ivPtDelayTime = new MFloatControl( String( "DelayTime" ), 0.0f, MAX_DELAY_TIME, DEFAULT_DELAYTIME, true );

	processEvent( &MEvent( MEvent::RESET ) );

	return true;
}

unsigned int MDelay::getControlCount()
{
	return 3;
}

IControl* MDelay::getControl( unsigned int index )
{
	switch( index )
	{
		case MIX: return ivPtMix; break;
		case FEEDBACK: return ivPtFeedback; break;
		case DELAYTIME: return ivPtDelayTime; break;
		default: return 0; break;
	}
}

void MDelay::setDelay( FP delay )
{
	ivPtDelayTime->setValue( delay );
}

FP MDelay::getDelay()
{
	return ivPtDelayTime->getValue();
}

void MDelay::setFeedback( FP feedback /* (0.0 - 1.0) */)
{
	ASSERT( feedback >= 0.0 && feedback <= 1.0 );
	ivPtFeedback->setValue( feedback );
}

FP MDelay::getFeedback()
{
	return ivPtFeedback->getValue();
}

void MDelay::processEvent( MEvent* ptEvent )
{
	if( ptEvent->getType() == MEvent::RESET )
	{
		for( unsigned int j=0;j<ivChannelCount;j++ )
			ZeroMemory( ivPtDelayBuffer[ j ], sizeof( FP ) * ivBufferLength );
		ivStep = 0;
	}
}

void MDelay::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt )
{
	int
		step,
		tempStep,
		offset = int( ivPtDelayTime->getValue() * MW_SAMPLINGRATE / 1000.0 );

	FP
		*pt,
		*ptStop,
		*ptCycleBuffer,
		tmpOut,
		mixWet = ivPtMix->getValue(),
		mixDry = 1 - ivPtMix->getValue(),
		feedback = ivPtFeedback->getValue();

	for( unsigned int j=0;j<ivChannelCount;j++ )
	{
		step = ivStep;
		pt=buffer->getData(j) + startFrom;
		ptStop = buffer->getData(j) + stopAt;
		ptCycleBuffer = ivPtDelayBuffer[ j ];
		for( ;pt<ptStop;pt++ )
		{
			tempStep = step - offset;
			if( tempStep < 0 )
				tempStep += ivBufferLength;

			ASSERT( tempStep >= 0 && tempStep < (int)ivBufferLength );
			ASSERT( step >= 0 && step < (int)ivBufferLength );

			tmpOut = ptCycleBuffer[ tempStep ];
			ptCycleBuffer[ step ] = feedback * ( (*pt) + tmpOut );
	
			if( (++step) >= int(ivBufferLength) )
				step = 0;
	
			(*pt) =  (*pt) * mixDry + tmpOut * mixWet;
		}
	}

	ivStep = step;
}

String MDelay::getName()
{
	return "Delay";
}

String MDelay::getShortName()
{
	return getName();
}

IRtti* MDelay::getRtti() const
{
	return &gvRtti;
}

MObject* MDelay::createInstance()
{
	return new MDelay();
}