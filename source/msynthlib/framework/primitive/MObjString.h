#ifndef __MObjString
#define __MObjString

#include "../MObject.h"
#include "../treedocument/ISerializeable.h"

/**
 * object implementing a string object
 */
class MObjString :
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
	 * storing the string data
	 */
	String ivData;

public:

	/**
	 * constructor
	 */
	MObjString();

	/**
	 * constructor
	 */
	MObjString( const char* string );

	/**
	 * destructor
	 */
	virtual ~MObjString();

	/**
	 * returns the class name for runtime purpose
	 */
	virtual IRtti* getRtti() const;

	/**
	 * queries for a specified interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * serialize as tree node
	 * (inheritanced from ISerializeable)
	 */
	virtual MTreeNode* save();

	/**
	 * load from treenode
	 * (inheritanced from ISerializeable)
	 */
	virtual void load( MTreeNode* node );

	/**
	 * creates an instance of a MObjString
	 */
	static MObject* createInstance();

	/**
	 * returns a string representation
	 */
	virtual String toString() const;

	/**
	 * sets the string
	 */
	virtual void setData( String text );
};
#endif