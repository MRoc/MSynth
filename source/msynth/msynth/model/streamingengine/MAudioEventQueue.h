#ifndef __MAudioEventQueue
#define __MAudioEventQueue

#include "IAudioEngineEvent.h"
#include <framework/sync/MSynchronize.h>

/**
 * a thread safe event queue for queueing audio events.
 * 
 * the dequeue only dequeues the newest event, all older ones
 * will be released. to allways the latest event is dequeued.
 */
class MAudioEventQueue :
	private std::vector<IAudioEngineEvent*>
{
protected:

	/** 
	* for sync purpose
	*/
	MCriticalSection ivSync;

public:

	/** 
	 * constructor
	 */
	MAudioEventQueue();

	/**
	 * destructor
	 */
	virtual ~MAudioEventQueue();

	/**
	 * enqueues an event
	 */
	void enqueue( IAudioEngineEvent* pEvent );

	/**
	 * dequeues the latest event available,
	 * if more than one event is queued, all
	 * older events are released.
	 */
	IAudioEngineEvent* dequeue();

	/**
	 * returns the number of queued events
	 */
	unsigned int getEventCount();

	/**
	 * returns event by index
	 */
	IAudioEngineEvent* getEvent( unsigned int index );

	/**
	 * removes the indexed event
	 */
	IAudioEngineEvent* remove( unsigned int index );
};

#endif