#ifndef __MTreeNode
#define __MTreeNode

#include "MTreeLeaf.h"

#include "MTreeNodeIterator.h"


/**
 * a implementation of a tree node used
 * with the MTreeDocument to store and
 * load data. it is similar to a DOMNode,
 * but is can not have more than one text child.
 * that is represented the ivContent.
 */
class MTreeNode :
	public MTreeLeaf
{
public:

	/** the runtime type info of this class */
	static MRtti gvRtti;

protected:

	/** the nodes attributes (xml attributes) */
	StrStrMap ivAttributes;

	/** the child nodes of this node */
	MObjectList ivChilds;

public:

	/** constructor */
	MTreeNode();

	/** constructor */
	MTreeNode( const String& name );

	/** destructor */
	virtual ~MTreeNode();

	/** sets a attribute of this node */
	virtual void setAttribute( const String& key, const String& value );

	/** returns a attribute of this node */
	virtual String getAttribute( const String& key );

	/** returns a attribute of this node, if not available, it returns the defaultvalue */
	virtual String getAttribute( const String& key, const String& defaultValue );

	/** returns true if the attribute is stored */
	virtual bool hasAttribute( const String& key );

	/** returns the number of attributes */
	virtual unsigned int getAttributeCount();

	/** returns the attribute name at the specified indesx */
	virtual String getAttributeName( unsigned int index );

	/** removes the specified attribute from node */
	virtual void removeAttribute( const String& attributeName );

	/** adds a child node to this node */
	virtual void addChild( MTreeLeaf* ptNode );

	/** removes the child at the specified index */
	virtual MTreeLeaf* removeChild( unsigned int index );

	/** deletes the child at the specified index */
	virtual void deleteChild( unsigned int index );

	/** returns the number of child nodes */
	virtual unsigned int getChildCount();

	/** returns the node at the specified index */
	virtual MTreeLeaf* getChild( unsigned int index );

	/** returns the first node with the given name */
	virtual MTreeNode* getFirstNode( const String& nodeName );

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

	/** return a iterator */
	virtual MTreeNodeIterator getIterator();
};

#endif