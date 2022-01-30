#ifndef __MAudioEngineEvent
#define __MAudioEngineEvent

#include "IAudioEngineEvent.h"

#include <framework/MTypes.h>
#include <framework/MNativeTypes.h>
#include <mmsystem.h>
#include <dsound.h>

/**
 * implementation of the IAudioEngineEvent interface.
 */
class MAudioEngineEvent :
	public IAudioEngineEvent
{
protected:

	/**
	 * the direct sound buffer to render into.
	 */
	LPDIRECTSOUNDBUFFER ivPtBuffer;

	/** 
	 * the write offset in the direct sound buffer.
	 */
	unsigned int ivWriteOffset;

	/** 
	 * the number of bytes to render into the buffer.
	 */
	unsigned int ivBytesToRender;

	/**
	 * the timestamp of creation.
	 */
	__int64 ivTimeStampCreated;

	/**
	 * the timestamp when writting is too late.
	 */
	__int64 ivTimeStampTooLate;

	/**
	 * the current state
	 */
	EState ivState;

public:

	/** 
	 * constructor
	 */
	MAudioEngineEvent(
		LPDIRECTSOUNDBUFFER ptBuffer,
		unsigned int writeOffset,
		unsigned int bytesToRender,
		__int64 timeStampCreated,
		__int64 timeStampTooLate );

	/** 
	 * destructor
	 */
	virtual ~MAudioEngineEvent();

	/**
	 * returns the event's current state
	 */
	virtual EState getState();

	/**
	 * returns the number of samples to be rendered
	 */
	virtual unsigned int getWriteLength();

	/**
	 * this method is called by the receiver of the event and signals
	 * that the event is finally consumed. after the invokation, the
	 * event is closed and not valid anymore.
	 */
	virtual void write( MSoundBuffer* pBuffer );

	/**
	 * discards the event, e.g. is the render thread was too slow
	 * and came too late.
	 */
	virtual void close();

	/**
	 * returns the timestamp this event was created.
	 */
	virtual __int64 getTimeStampStart();

	/**
	 * returns the timestamp when this render task took
	 * to long, the last time to invoke write.
	 */
	virtual __int64 getTimeStampTooLate();

	/**
	 * deletes this instance of event
	 */
	virtual void release();

};

#endif