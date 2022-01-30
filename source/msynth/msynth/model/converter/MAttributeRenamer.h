#ifndef __MAttributeRenamer
#define __MAttributeRenamer

#include <framework/MObject.h>
#include <framework/primitive/MInt.h>
#include <framework/primitive/MFloat.h>
#include <framework/primitive/MBool.h>

#include "INodeConverter.h"

/**
 * converter renaming an attribute
 */
class MAttributeRenamer :
	public MObject,
	public INodeConverter
{
public:

	/**
	 * runtime type info
	 */
	static MRtti gvRtti;

private:

	/**
	 * the new attribute name
	 */
	String attrName;

	/**
	 * possible old names of the attribute that should be renaded
	 */
	StrList oldNames;

public:

	/**
	 * constructor
	 */
	MAttributeRenamer();

	/**
	 * destructor
	 */
	virtual ~MAttributeRenamer();

	/**
	 * returns the new attribute name
	 */
	virtual String getAttributeName();

	/**
	 * deletes this instance
	 */
	virtual void release();

	/**
	 * does the renaming of a node, only renames one attribute.
	 * iterates the old attribute names and searches in the node
	 * if one is found. If it is found, the attribute name is replaced
	 * by the new name. After renaming one attribute, the function returns,
	 * or if no old one found, after renaming nothing.
	 */
	virtual void convert( MTreeNode* nodeToBeConverted );

	/**
	 * loads the attribute renamer
	 */
	virtual void load( MTreeNode* node );

	/**
	 * saving
	 */
	virtual MTreeNode* save();

	/**
	 * returns the old name at the specified index
	 */
	virtual String getOldName( unsigned int index );

	/**
	 * returns the number of old attribute names
	 */
	virtual unsigned int getOldNameCount();

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * query interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * creates an instance of this class
	 */
	static MObject* createInstance();

private:

	/**
	 * adds a old attribute name
	 */
	virtual void addOldName( String oldName );

};

#endif