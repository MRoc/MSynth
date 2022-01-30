#ifndef __MChildElementEnumeration
#define __MChildElementEnumeration

#include "MXmlMatchingNode.h"

/**
 * enumerates all childs in the convert method
 * and invokes the converter child's convert
 * method for every child found in the xml element
 */
class MChildElementEnumeration :
	public MXmlMatchingNode
{
public:

	/**
	 * runtime support
	 */
	static MRtti gvRtti;

public:

	/**
	 * constructor
	 */
	MChildElementEnumeration();

	/**
	 * destructor
	 */
	virtual ~MChildElementEnumeration();

	/**
	 * enumerates child matchings and invokes 
	 * the convert method for every element child
	 * found in the treenode node. then the own
	 * converters are invoked
	 */
	virtual void convert( MTreeNode* ptNode );

	/**
	 * returns allways true
	 */
	virtual bool match( MTreeNode* ptNode );

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * returns an instance of a MAttributeMatch
	 */
	static MObject* createInstance();
};

#endif