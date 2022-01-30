#ifndef __INodeConverter
#define __INodeConverter

#include <framework/MTypes.h>
#include <framework/treedocument/ISerializeable.h>

/**
 * this interface represents a node conversion.
 * a node conversion is any operation converting
 * the structure of a MTreeDocument
 */
class INodeConverter :
	public IMUnknown,
	public ISerializeable
{
public:

	/**
	 * converts the given node
	 */
	virtual void convert( MTreeNode* ptNode ) = 0;

	/**
	 * loading
	 */
	virtual void load( MTreeNode* ptNode ) = 0;

	/**
	 * storing
	 */
	virtual MTreeNode* save() = 0;
};

typedef std::vector<INodeConverter*> NodeConvList;
typedef NodeConvList::iterator NodeConvListIter;

#endif