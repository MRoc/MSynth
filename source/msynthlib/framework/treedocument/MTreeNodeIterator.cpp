#include "MTreeNodeIterator.h"
#include "MTreeNode.h"

/** constructor */
MTreeNodeIterator::MTreeNodeIterator( MObjectList* pList ) :
	MObjectListIterator( pList )
{
}

/** destructor */
MTreeNodeIterator::~MTreeNodeIterator()
{
}

/** return true if there is any next element */
bool MTreeNodeIterator::hasNext()
{
	int count = ivPtList->getLength();
	for( int i=ivIndex+1;i<count;i++ )
		if( ((MTreeLeaf*)ivPtList->get( i ))->getType() == MTreeLeaf::NODE )
			return true;
	return false;
}

/** returns the next element */
MTreeNode* MTreeNodeIterator::nextTreeNode()
{
	ivIndex++;
	int count = ivPtList->getLength();
	for( ;ivIndex<count;ivIndex++ )
		if( ((MTreeLeaf*)ivPtList->get( ivIndex ))->getType() == MTreeLeaf::NODE )
			return ((MTreeNode*)ivPtList->get( ivIndex ));
	return 0;
}
