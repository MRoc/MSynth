#ifndef __MTreeDocument
#define __MTreeDocument

#include "MTreeNode.h"

/**
 * imeplements of a abstract tree document
 * which is a tree containing out of
 * MTreeNodes, similar to a dom xml tree
 */
class MTreeDocument :
	public MObject
{
public:

	/**
	 * the runtime type info of this class
	 */
	static MRtti gvRtti;

protected:

	/**
	 * the root node
	 */
	MTreeNode* ivPtRoot;

public:

	/**
	 * constructor
	 */
	MTreeDocument();

	/**
	 * destructor
	 */
	virtual ~MTreeDocument();

	/**
	 * return the root node of this tree
	 */
	virtual MTreeNode* getRoot();

	/**
	 * sets the root node of this tree
	 */
	virtual void setTreeNode( MTreeNode* ptNode );

	/**
	 * generates the xml
	 */
	virtual void toXml( IPrintStream* ptOut );

	/**
	 * parses the specified file
	 */
	virtual void parse( String fileName ) = 0;

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * queries for a specified interface
	 */
	virtual void* getInterface( const String &className ) const;
};

#endif