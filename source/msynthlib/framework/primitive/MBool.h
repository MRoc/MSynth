#ifndef __MBool
#define __MBool

#include "../MObject.h"
#include "../treedocument/ISerializeable.h"
#include "MInt.h"

/**
 * implementation of a bool as MObject
 */
class MBool :
	public MObject,
	public ISerializeable
{
public:

	/**
	 * runtime support
	 */
	static MRtti gvRtti;

protected:

	/**
	 * the arttribute name for storing in a xml node
	 */
	static const String ATTR_VALUE;

	/**
	 * the internal boolean storage
	 */
	bool ivData;

public:

	/**
	 * constructor from a xml node
	 */
	MBool();

	/**
	 * constructor
	 */
	MBool( const bool value );

	/**
	 * destructor
	 */
	virtual ~MBool();

	/**
	 * returns a string representation
	 */
	virtual String toString() const;

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * querries for a specified interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * returns the internale boolean value
	 */
	virtual bool getValue();

	/**
	 * sets the internale boolean value
	 */
	virtual void setValue( bool value );

	/**
	 * serialize as tree node
	 */
	virtual MTreeNode* save();

	/**
	 * load from treenode
	 */
	virtual void load( MTreeNode* node );

	/**
	 * creates an instance
	 */
	static MObject* createInstance();

	/**
	 * returns a string representation, 1 or 0
	 */
	static String toString( bool b );

	/**
	 * parses a bool from 0 or 1
	 */
	static bool parse( String str, bool defaultValue );
};

#endif