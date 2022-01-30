#ifndef __MSynchronize
#define __MSynchronize

#include "MCriticalSection.h"

/**
 * implementation of a synchronize object.
 * should only be used on the stack.
 * when created, the critical section is locked.
 * when destroyed, the critical section is unlocked.
 */
class MSynchronize
{
protected:

	/**
	 * the critical section object
	 */
	MCriticalSection* ivPtCriticalSection;

public:

	/**
	 * constructor
	 */
	MSynchronize( MCriticalSection* ptCriticalSection );

	/**
	 * destructor
	 */
	virtual ~MSynchronize();
};

#endif