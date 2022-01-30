#ifndef __MCriticalSection
#define __MCriticalSection

#include "../MTypes.h"
#include "../MNativeTypes.h"

/**
 * implementation of a cricical section
 */
class MCriticalSection
{
protected:

	/**
	 * the win32 handle to a critical section
	 */
	CRITICAL_SECTION ivCriticalSection;

public:

	/**
	 * constructor
	 */
	MCriticalSection();

	/**
	 * destructor
	 */
	virtual ~MCriticalSection();

	/**
	 * locks the critical section
	 */
	virtual void lock();

	/**
	 * unlocks the critical section
	 */
	virtual void unlock();
};

#endif