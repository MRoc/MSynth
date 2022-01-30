#ifndef __MAudioThread
#define __MAudioThread

#include <framework/thread/MSyncThread.h>
#include <framework/sync/MSynchronize.h>
#include "MAudioEventQueue.h"
#include "../interfaces/IProcessor.h"

/** 
 * a synchronized thread to render queued audio engine events.
 *
 * another thread can post events. this events are rendered by this
 * thread. if the queue has more than one event, older events are
 * released. so, allways only the latest event is rendered.
 *
 * when the postJob method is invoked, a event is enqueued and a signal
 * is posted to the sleeping thread to wake up and render. after rendering,
 * the thread goes sleep until the next post event invocation.
 */
class MAudioThread :
	public MSyncThread
{
protected:

	class MAudioThreadWorker;

	/**
	 * the worker runnable
	 */
	MAudioThreadWorker* ivPtWorker;

public:

	/**
	 * constructor
	 */
	MAudioThread( IProcessor* pSrc );

	/** 
	 * destructor
	 */
	virtual ~MAudioThread();

	/** 
	 * method posting a audio job,
	 * to be invoked from another thread.
	 */
	virtual bool postJob( IAudioEngineEvent* pEvent );

protected:

	/**
	 * this worker does one rendering task.
	 * it has a audio event queue where all rendering tasks are enqueued.
	 * in the run method of this class, one rendering event is dequeued 
	 * and executed.
	 */
	class MAudioThreadWorker :
		public IRunnable
	{
	protected:

		/**
		 * the plugged in audio source used for rendering
		 */
		IProcessor* ivPtSource;

		/** 
		 * the audio event queue
		 */
		MAudioEventQueue ivEventQueue;

	public:

		/**
		 * constructor poviding the audio rendering source
		 */
		MAudioThreadWorker( IProcessor* ivPtSource );

		/** 
		 * destructor
		 */
		virtual ~MAudioThreadWorker();

		/**
		 * returns the aggregated event queue
		 */
		virtual MAudioEventQueue* getEventQueue();

		/** 
		 * the method doing the task.
		 * dequeues a audio event and executes.
		 */
		virtual void run();
	};
};

#endif