#ifndef __MTreeNodeIterator
#define __MTreeNodeIterator

#include <framework/list/MObjectListIterator.h>

class MTreeNode;

/** a tree node iterator, only iterates over nodes */
class MTreeNodeIterator :
	public MObjectListIterator
{
public:

	/** constructor */
	MTreeNodeIterator( MObjectList* pList );

	/** destructor */
	virtual ~MTreeNodeIterator();

	/** return true if there is any next element */
	virtual bool hasNext();

	/** returns the next element */
	virtual MTreeNode* nextTreeNode();
};

#endif