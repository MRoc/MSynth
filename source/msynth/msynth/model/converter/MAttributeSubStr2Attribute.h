#ifndef __MAttributeSubStr2Attribute
#define __MAttributeSubStr2Attribute

#include <framework/MObject.h>
#include <framework/primitive/MInt.h>

#include "INodeConverter.h"
#include "MAttributeMatch.h"

/**
 * takes a substring from a attribute and creates
 * a new attribute with the extracted substring
 */
class MAttributeSubStr2Attribute :
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
	 * index no. one for the substring
	 */
	unsigned int ivIndex;

	/**
	 * the substring mode
	 */
	MAttributeMatch::MatchingType ivMatchingType;

	/**
	 * the attribute name where to extract the substring
	 */
	String ivAttributeName;

	/**
	 * the new name of the attribute
	 */
	String ivNewAttributeName;

public:

	/**
	 * constructor
	 */
	MAttributeSubStr2Attribute();

	/**
	 * destructor
	 */
	virtual ~MAttributeSubStr2Attribute();

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