#ifndef __ISerializeable
#define __ISerializeable

#include "../MTypes.h"
#include "../treedocument/MTreeNode.h"

/**
 * interface for serializable objects that
 * can serialize into a MTreeNode.
 */
class ISerializeable
{
public:

	/** loads from the given tree node */
	virtual void load( MTreeNode* ptNode ) = 0;

	/** stores as tree node */
	virtual MTreeNode* save() = 0;
};

#endif