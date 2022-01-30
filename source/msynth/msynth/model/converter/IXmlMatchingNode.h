#ifndef __IXmlMatchingNode
#define __IXmlMatchingNode

#include <framework/MTypes.h>
#include <framework/treedocument/ISerializeable.h>
#include "INodeConverter.h"

/**
 * this interface represents a node conversion with condition.
 * the conversion should be delegated to the INodeConverters
 */
class IXmlMatchingNode :
	public INodeConverter
{
public:

	/**
	 * invokes the converters
	 */
	virtual void convert( MTreeNode* ptNode ) = 0;

	/**
	 * sets the parent condition
	 */
	virtual void setParent( IXmlMatchingNode* ptParent ) = 0;

	/**
	 * returns the parent condition
	 */
	virtual IXmlMatchingNode* getParent() = 0;

	/**
	 * returns the number of child conditions
	 */ 
	virtual unsigned int getChildCount() = 0;

	/**
	 * returns the child condition at the specified index
	 */
	virtual IXmlMatchingNode* getChild( unsigned int index ) = 0;

	/**
	 * storing
	 */
	virtual MTreeNode* save() = 0;

	/**
	 * loading
	 */
	virtual void load( MTreeNode* ptNode ) = 0;
};

#endif