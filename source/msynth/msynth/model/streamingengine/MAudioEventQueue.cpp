#include "MAudioEventQueue.h"


/** 
 * constructor
 */
MAudioEventQueue::MAudioEventQueue()
{
}

/**
 * destructor
 */
MAudioEventQueue::~MAudioEventQueue()
{
}

/**
 * enqueues an event
 */
void MAudioEventQueue::enqueue( IAudioEngineEvent* pEvent )
{
	MSynchronize sync( &ivSync );
	push_back( pEvent );
}

/**
 * dequeues the latest event available,
 * if more than one event is queued, all
 * older events are released.
 */
IAudioEngineEvent* MAudioEventQueue::dequeue()
{
	MSynchronize sync( &ivSync );
	IAudioEngineEvent* pBack = 0;
	if( size() )
	{
		// remove all unused events
		while( size() > 1 )
		{
			pBack = at( 1 );
			erase( begin() + 1 );
			pBack->release();
		}
		pBack = at( 0 );
		erase( begin() );
	}
	return pBack;
}

/**
 * returns the number of queued events
 */
unsigned int MAudioEventQueue::getEventCount()
{
	MSynchronize sync( &ivSync );
	return (unsigned int) size();
}

/**
 * returns event by index
 */
IAudioEngineEvent* MAudioEventQueue::getEvent( unsigned int index )
{
	MSynchronize sync( &ivSync );
	ASSERT( index < (unsigned int) size() );
	return (IAudioEngineEvent*) at( index );
}

/**
 * removes the indexed event
 */
IAudioEngineEvent* MAudioEventQueue::remove( unsigned int index )
{
	MSynchronize sync( &ivSync );
	ASSERT( index < (unsigned int) size() );
	IAudioEngineEvent* pBack = (IAudioEngineEvent*) at( index );
	erase( begin() + index );
	return pBack;
}

