#ifndef __MObjectListIterator
#define __MObjectListIterator

#include "MObjectList.h"
#include "IListIterator.h"

/** a iterator for iterating over a objectlist */
class MObjectListIterator
	: public IListIterator
{
protected:

	/** reference to list */
	MObjectList* ivPtList;

	/** the iterator index */
	int ivIndex;

public:

	/** constuctor */
	MObjectListIterator( MObjectList* pObjectList );

	/** destructor */
	virtual ~MObjectListIterator();

	/** return true if there is any next element */
	virtual bool hasNext();

	/** returns the next element */
	virtual MObject* next();
};

#endif