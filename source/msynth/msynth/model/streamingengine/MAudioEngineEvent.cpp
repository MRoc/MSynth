#include "MAudioEngineEvent.h"
#include <framework/io/MLogger.h>
#include "../interfaces/MSoundBufferUtils.h"

/** 
 * constructor
 */
MAudioEngineEvent::MAudioEngineEvent(
	LPDIRECTSOUNDBUFFER ptBuffer,
	unsigned int writeOffset,
	unsigned int bytesToRender,
	__int64 timeStampCreated,
	__int64 timeStampTooLate
	) :
	ivPtBuffer( ptBuffer ),
	ivWriteOffset( writeOffset ),
	ivBytesToRender( bytesToRender ),
	ivTimeStampCreated( timeStampCreated ),
	ivTimeStampTooLate( timeStampTooLate ),
	ivState( IAudioEngineEvent::VIRGIN )
{
}

/** 
 * destructor
 */
MAudioEngineEvent::~MAudioEngineEvent()
{
}

/**
 * returns the event's current state
 */
IAudioEngineEvent::EState MAudioEngineEvent::getState()
{
	return this->ivState;
}

/**
 * this method is called by the receiver of the event and signals
 * that the event is finally consumed. after the invokation, the
 * event is closed and not valid anymore.
 */
void MAudioEngineEvent::write( MSoundBuffer* pBuffer )
{
	LPVOID pv_writePos1 = 0, pv_writePos2 = 0;
	DWORD pdw_blockSize1 = 0, pdw_blockSize2 = 0;

	//TRACE( "locking %i bytes at %i\n", ivBytesToRender, ivWriteOffset );

    // lock...
    if(
		SUCCEEDED(
			ivPtBuffer->Lock(
				ivWriteOffset,
				ivBytesToRender, 
				&pv_writePos1,
				&pdw_blockSize1, 
				&pv_writePos2,
				&pdw_blockSize2,
				0 ) ) )
	{
		// assert...
		//RELEASE_ASSERT( ! pdw_blockSize2 );
		//RELEASE_ASSERT( ivBytesToRender == pdw_blockSize1 );

		// copy to buffer...
		pBuffer->export2pcm16bit2channel( (MW_SAMPLETYPE*) pv_writePos1 );

		// unlock...
		if(
			FAILED( 
				ivPtBuffer->Unlock(
					pv_writePos1,
					pdw_blockSize1,
					pv_writePos2,
					pdw_blockSize2 ) ) )
		{
			MLogger::logWarning( "MAudioEngineEvent::unlock failed!" );
		}
	}
	else
	{
		MLogger::logWarning( "MAudioEngineEvent::lock failed!" );
	}

	// release event...
	ivState = IAudioEngineEvent::CONSUMED;
}

/**
 * returns the number of samples to be rendered
 */
unsigned int MAudioEngineEvent::getWriteLength()
{
	//return ivBytesToRender / ( sizeof( MW_SAMPLETYPE )/*2bytesPerSample*/ /*2channelsStereo*/ );
	return ivBytesToRender / (2*sizeof( MW_SAMPLETYPE ));
}

/**
 * discards the event, e.g. is the render thread was too slow
 * and came too late.
 */
void MAudioEngineEvent::close()
{
	ivState = IAudioEngineEvent::REJECTED;
}

/**
 * returns the timestamp this event was created.
 */
__int64 MAudioEngineEvent::getTimeStampStart()
{
	return ivTimeStampCreated;
}

/**
 * returns the timestamp when this render task took
 * to long, the last time to invoke write.
 */
__int64 MAudioEngineEvent::getTimeStampTooLate()
{
	return ivTimeStampTooLate;
}

/**
 * deletes this instance of event
 */
void MAudioEngineEvent::release()
{
	delete this;
}