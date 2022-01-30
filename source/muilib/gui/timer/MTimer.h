#ifndef __MTimer
#define __MTimer

#include <framework/MTypes.h>
#include "MTimerImpl.h"

/**
 * HANDLE WITH CARE
 */
class MTimer
{
protected:

	/** the internal timer implementation */
	MTimerImpl* ivImpl;

public:

	/** constructor */
	MTimer();

	/** destructor */
	virtual ~MTimer();

	/** starts the timer */
	virtual void startTimer( int millis );

	/** stops the timer */
	virtual void stopTimer();

	/** returns true if timer is running */
	virtual bool isRunnning();

	/** adds a timer listener */
	virtual void addListener( IActionListener* pListener );

	/** removes a timer listener */
	virtual void removeListener( IActionListener* pListener );

	/** returns the number of cycles the timer was called back since last start */
	virtual unsigned int getTimerCycles();

	/** returns the internal impl */
	virtual MTimerImpl* getImpl();
};

#endif