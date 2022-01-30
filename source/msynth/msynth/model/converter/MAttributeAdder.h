#ifndef __MAttributeAdder
#define __MAttributeAdder

#include <framework/MObject.h>

#include "INodeConverter.h"

/**
 * adds a attribute to a element
 */
class MAttributeAdder :
	public MObject,
	public INodeConverter
{
public:

	/**
	 * the runtime type info
	 */
	static MRtti gvRtti;

protected:

	/**
	 * the attribute to be added
	 */
	String ivAttributeName;

	/**
	 * the content of the attribute to be added
	 */
	String ivAttributeContent;

public:

	/**
	 * constructor
	 */
	MAttributeAdder();

	/**
	 * destructor
	 */
	virtual ~MAttributeAdder();

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * query for a interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * converts the given node
	 */
	virtual void convert( MTreeNode* ptNode );

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