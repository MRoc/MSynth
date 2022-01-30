#ifndef __MElementMatch
#define __MElementMatch

#include "IElementMatch.h"
#include "MXmlMatchingNode.h"

/**
 * implementation of a element matching node
 */
class MElementMatch :
	public MXmlMatchingNode,
	public IElementMatch
{
public:

	/**
	 * runtime support
	 */
	static MRtti gvRtti;

protected:

	StrList ivOldNames;
	String ivName;

public:

	/**
	 * constructor
	 */
	MElementMatch();

	/**
	 * destructor
	 */
	virtual ~MElementMatch();

	/**
	 * query interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * saving
	 */
	virtual MTreeNode* save();

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * loading
	 */
	virtual void load( MTreeNode* ptNode );

	/**
	 * returns the name of the new element (can be the same as old one)
	 */
	virtual String getElementName();

	/**
	 * sets the element name
	 */
	virtual void setElementName( String en );

	/**
	 * returns the number of old names this tag had
	 */
	virtual unsigned int getOldNameCount();

	/**
	 * returns the old name at the specified index
	 */
	virtual String getOldName( unsigned int index );

	/**
	 * adds a old name to the oldname list
	 */
	virtual void addOldName( String name );

	/**
	 * returns true if the given name is in the name list
	 */
	virtual bool hasOldName( String name );

	/**
	 * returns true if the given node has the element name
	 * or a old one.
	 */
	virtual bool match( MTreeNode* ptNode );

	/**
	 * does the conversion
	 */
	virtual void convert( MTreeNode* ptNode );

	/**
	 * static create instance method for MDefaultFactory
	 */
	static MObject* createInstance();
};

#endif