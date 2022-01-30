#ifndef __MAttributeContentReplacer
#define __MAttributeContentReplacer

#include <framework/MObject.h>

#include "INodeConverter.h"

/**
 * replaces the content of a attribute
 */
class MAttributeContentReplacer :
	public MObject,
	public INodeConverter
{
public:
	
	/**
	 * runtime type info
	 */
	static MRtti gvRtti;

protected:

	/**
	 * list of possible attribute names
	 */
	StrList ivAttributeNames;

	/**
	 * the new attribute content
	 */
	String ivAttributeContent;

public:

	/**
	 * constructor
	 */
	MAttributeContentReplacer();

	/**
	 * destructor
	 */
	virtual ~MAttributeContentReplacer();

	/**
	 * converts the given node
	 */
	virtual void convert( MTreeNode* ptNode );

	/**
	 * queries for a interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * returns the runtime info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * loading
	 */
	virtual void load( MTreeNode* ptNode );

	/**
	 * storing
	 */
	virtual MTreeNode* save();

	/**
	 * creates a instance of this class
	 */
	static MObject* createInstance();
};

#endif