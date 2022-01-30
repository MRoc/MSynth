#ifndef __MSyncThread
#define __MSyncThread

#include "MThread.h"
#include "IRunnable.h"

/** 
 * a thread that can be synchronized with a win32 event
 */
class MSyncThread :
	public MThread
{
protected:

	/**
	 * the handle to the win32 event
	 */
	HANDLE ivHandleEvent;

	/**
	 * reference to the run method to be
	 * invoked when event is triggered.
	 */
	IRunnable* ivPtRunnable;

public:

	/**
	 * constructor
	 *
	 * the runnable::run method is invoked
	 * every time the event is triggered.
	 */
	MSyncThread( IRunnable* pRunnable );

	/**
	 * destructor
	 */
	virtual ~MSyncThread();

	/**
	 * waits for the event until resume
	 * is invoked from another thread.
	 */
	virtual void run();

	/**
	 * triggers the event,
	 * invoked from another thread.
	 */
	virtual bool trigger();
};

#endif
