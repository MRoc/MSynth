#include "MObjectListIterator.h"

/** constuctor */
MObjectListIterator::MObjectListIterator( MObjectList* pObjectList ) :
	ivIndex( -1 ),
	ivPtList( pObjectList )
{
	ASSERT( ivPtList );
}

/** destructor */
MObjectListIterator::~MObjectListIterator()
{
}

/** return true if there is any next element */
bool MObjectListIterator::hasNext()
{
	ASSERT( ivPtList );
	return ivIndex < ivPtList->getLength();
}

/** returns the next element */
MObject* MObjectListIterator::next()
{
	ASSERT( ivPtList );
	ASSERT( hasNext() );
	return ivPtList->get( ivIndex++ );
}