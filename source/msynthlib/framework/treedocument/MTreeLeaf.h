#ifndef __MTreeLeaf
#define __MTreeLeaf

#include "../MObject.h"
#include "../list/MObjectList.h"
#include "../io/IPrintStream.h"

/** a treeleaf is a simple textnode in a xml document */
class MTreeLeaf :
	public MObject
{
public:

	/** the runtime type info of this class */
	static MRtti gvRtti;

	/** the types of leafs */
	enum LeafType
	{
		LEAF,
		NODE
	};

protected:

	/** the text/name of the node */
	String ivName;

	/** the nodes parent node (null if root) */
	MTreeLeaf* ivPtParent;

public:

	/** constructor */
	MTreeLeaf();

	/** constructor */
	MTreeLeaf( const String& name );

	/** destructor */
	virtual ~MTreeLeaf();

	/** sets the node's name */
	virtual void setName( const String& name );

	/** returns the node's name */
	virtual const String &getName() const;

	/** returns the parent node of this node or null if this node is a root node */
	virtual MTreeLeaf* getParent() const;

	/** returns a xml representing this node */
	virtual void toXml( IPrintStream* out );

	/** queries for an interface */
	virtual void* getInterface( const String &className ) const;

	/** returns the runtime type info */
	virtual IRtti* getRtti() const;

	/** creates an instance of this class */
	static MObject* createInstance();

	/** returns the leaf/node type */
	virtual LeafType getType() const;

	friend class MTreeNode;

protected:

	/** sets the parent of this node */
	virtual void setParent( MTreeLeaf* ptParent );
};


#endif