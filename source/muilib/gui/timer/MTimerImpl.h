#ifndef __MTimerImpl
#define __MTimerImpl

#include <framework/listener/IActionListener.h>

/** a timer impl compiler firewall */
class MTimerImpl
{
public:

	/** constructor */
	MTimerImpl(){}

	/** destructor */
	virtual ~MTimerImpl(){}

	/** starts the timer */
	virtual void startTimer( int millis ) = 0;

	/** stops the timer */
	virtual void stopTimer() = 0;

	/** returns true if timer is running */
	virtual bool isRunnning() = 0;

	/** adds a timer listener */
	virtual void addListener( IActionListener* pListener ) = 0;

	/** removes a timer listener */
	virtual void removeListener( IActionListener* pListener ) = 0;

	/** returns the number of cycles the timer was called back since last start */
	virtual unsigned int getTimerCycles() const = 0;
};

#endif