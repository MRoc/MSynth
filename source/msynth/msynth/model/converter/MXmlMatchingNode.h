#ifndef __MXmlMatchingNode
#define __MXmlMatchingNode

#include "IXmlMatchingNode.h"

#include <framework/MObject.h>
#include <framework/MDefaultFactory.h>
#include <framework/MException.h>

/**
 * implementation of a matching node
 * if asked for matching, the matching node asks
 * all childs, if it matches
 */
class MXmlMatchingNode :
	public MObject,
	public IXmlMatchingNode
{
public:

	/**
	 * runtime support
	 */
	static MRtti gvRtti;

protected:

	/**
	 * parent reference
	 */
	IXmlMatchingNode* ivPtParent;

	/**
	 * list of all child conditions that must match
	 */
	MObjectList ivChilds;

	/**
	 * the storec converters for this matching
	 */
	MObjectList ivConverters;

public:

	/**
	 * constructor
	 */
	MXmlMatchingNode();

	/**
	 * destructor
	 */
	virtual ~MXmlMatchingNode();

	/**
	 * query interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * returns the parent condition
	 */
	virtual IXmlMatchingNode* getParent();

	/**
	 * sets the parent condition
	 */
	virtual void setParent( IXmlMatchingNode* ptNode );

	/**
	 * returns the number of child conditions
	 */ 
	virtual unsigned int getChildCount();

	/**
	 * returns the child condition at the specified index
	 */
	virtual IXmlMatchingNode* getChild( unsigned int index );

	/**
	 * adds a child condition
	 */
	virtual void addChild( IXmlMatchingNode* ptNode );

	/**
	 * returns true the node matched
	 */
	virtual bool match( MTreeNode* ptNode ) = 0;

	/**
	 * if own match method was successfull,
	 * the converters are invoked, then the
	 * convert method of all childs are invoked.
	 */
	virtual void convert( MTreeNode* ptNode );

	/**
	 * storing
	 */
	virtual MTreeNode* save();

	/**
	 * loading
	 */
	virtual void load( MTreeNode* ptNode );
};

#endif