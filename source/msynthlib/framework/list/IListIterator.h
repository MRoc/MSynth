#ifndef __IListIterator
#define __IListIterator

#include <framework/MTypes.h>

/** definition of a list iterator */
class IListIterator
{
public:

	/** return true if there is any next element */
	virtual bool hasNext() = 0;

	/** returns the next element */
	virtual MObject* next() = 0;
};

#endif