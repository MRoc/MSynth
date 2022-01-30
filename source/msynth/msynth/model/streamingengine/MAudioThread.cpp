#include "MAudioThread.h"
#include <framework/io/MLogger.h>
#include "../interfaces/MSoundBufferUtils.h"

/** 
 * constructor
 */
MAudioThread::MAudioThread( IProcessor* pSource ) :
	MSyncThread( 0 ),
	ivPtWorker( 0 )
{
	ivPtRunnable = ivPtWorker = new MAudioThreadWorker( pSource );
}

/** 
 * destructor
 */
MAudioThread::~MAudioThread()
{
	delete ivPtWorker;
}

/** 
 * method posting a audio job,
 * to be invoked from another thread.
 */
bool MAudioThread::postJob( IAudioEngineEvent* pEvent )
{
	ivPtWorker->getEventQueue()->enqueue( pEvent );
	return trigger();
}

/**
 * constructor
 */
MAudioThread::MAudioThreadWorker::MAudioThreadWorker( IProcessor* pSource ) :
	ivPtSource( pSource )
{
}

/** 
 * destructor
 */
MAudioThread::MAudioThreadWorker::~MAudioThreadWorker()
{
}

/** 
 * the method doing the task
 */
void MAudioThread::MAudioThreadWorker::run()
{
	IAudioEngineEvent* pEvent = ivEventQueue.dequeue();
	if( pEvent )
	{
		unsigned int samplesToRender = pEvent->getWriteLength();
		MSoundBuffer buffer( 2, samplesToRender );
		ivPtSource->goNext( &buffer, 0, samplesToRender );
		pEvent->write( &buffer );
		pEvent->release();
	}
}

/**
 * returns the aggregated event queue
 */
MAudioEventQueue* MAudioThread::MAudioThreadWorker::getEventQueue()
{
	return &ivEventQueue;
}