#ifndef __MAttribute2Element
#define __MAttribute2Element

#include <framework/MObject.h>
#include "INodeConverter.h"

/**
 * converts attributes to child elements.
 */
class MAttribute2Element :
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
	 * list of excluded attributes that should not be converted
	 */
	StrList ivExcludeAttributes;

	/**
	 * the name of the new element
	 */
	String ivNewElementName;

	/**
	 * the name of the new attribute that will store the old value
	 */
	String ivNewAttributeOldValue;

	/**
	 * the name of the new attribute that will store the old attribute name
	 */
	String ivNewAttributeOldName;


public:

	/**
	 * constructor
	 */
	MAttribute2Element();

	/**
	 * destrutor
	 */
	virtual ~MAttribute2Element();

	/**
	 * converts the given node
	 */
	virtual void convert( MTreeNode* ptNode );

	/**
	 * returns the name of the new element
	 */
	virtual String getNewElementName();

	/**
	 * returns name of the new attribute that will store the old value
	 */
	virtual String getNewAttributeOldValue();

	/**
	 * returns the name of the new attribute that will store the old attribute name
	 */
	virtual String getNewAttributeOldName();

	/**
	 * returns the number of attributes to be excluded
	 */
	virtual unsigned int getExcludeCount();

	/**
	 * returns the attribute that should be excluded at the specified index
	 */
	virtual String getExcludeAttribute( unsigned int index );

	/**
	 * returns true if the given attribute name should be excluded
	 */
	virtual bool excludeAttribute( String attributeName );

	/**
	 * loading
	 */
	virtual void load( MTreeNode* node );

	/**
	 * saving
	 */
	virtual MTreeNode* save();

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * query for a interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * creates a new instance of this class
	 */
	static MObject* createInstance();
};

#endif