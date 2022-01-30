#ifndef __IAudioEngineEvent
#define __IAudioEngineEvent

#include "../interfaces/MSoundBuffer.h"

/** 
 *
 * this event is a render task posted by the IAudioEngine to a render stream.
 * if received, the render stream should render its samples with help of the
 * write method.
 *
 * there are two timestamps. the first one returns the timestamp when the task
 * was created, the second one, when it's too late to render.
 *
 */
class IAudioEngineEvent
{
public:

	/** 
	 * the state of the event,
	 * VIRGIN: never used, just created
	 * CONSUMED: already rendered event
	 * REJECTED: render thrad took too long
	 */
	enum EState
	{
		VIRGIN,
		CONSUMED,
		REJECTED
	};

	/**
	 * returns the event's current state
	 */
	virtual EState getState() = 0;

	/**
	 * returns the number of samples to be rendered
	 */
	virtual unsigned int getWriteLength() = 0;

	/**
	 * this method is called by the receiver of the event and signals
	 * that the event is finally consumed. after the invokation, the
	 * event is closed and not valid anymore.
	 */
	virtual void write( MSoundBuffer* pBuffer ) = 0;

	/**
	 * discards the event, e.g. is the render thread was too slow
	 * and came too late.
	 */
	virtual void close() = 0;

	/**
	 * returns the timestamp this event was created.
	 */
	virtual __int64 getTimeStampStart() = 0;

	/**
	 * returns the timestamp when this render task took
	 * to long, the last time to invoke write.
	 */
	virtual __int64 getTimeStampTooLate() = 0;

	/**
	 * deletes this instance of event
	 */
	virtual void release() = 0;

};

#endif