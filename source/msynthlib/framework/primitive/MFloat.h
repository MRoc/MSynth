#ifndef __MFloat
#define __MFloat

#include "../MObject.h"
#include "../treedocument/ISerializeable.h"

class MFloat :
	public MObject,
	public ISerializeable
{
public:

	/**
	 * runtime type info
	 */
	static MRtti gvRtti;

protected:

	/**
	 * the internal stored value
	 */
	float ivValue;

	/**
	 * the attribute name for value
	 */
	static const String ATTR_VALUE;

public:

	/**
	 * constructor
	 */
	MFloat();

	/**
	 * constructor
	 */
	MFloat( float f );


	/**
	 * destructor
	 */
	virtual ~MFloat();

	/**
	 * sets the stored value
	 */
	virtual void setValue( float value );

	/**
	 * returns the stored value
	 */
	virtual float getValue();

	/**
	 * returns a string representation
	 */
	virtual String toString() const;

	/**
	 * queries for an interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * serialize as tree node
	 */
	virtual MTreeNode* save();

	/**
	 * load from treenode
	 */
	virtual void load( MTreeNode* node );

	/**
	 * creates an instance of this class
	 */
	static MObject* createInstance();

	/**
	 * to stirng the given float
	 */
	static String toString( float f );

	/**
	 * parse the given string
	 */
	static float parse( String str );

	/**
	 * parse the given string
	 */
	static float parse( String str, float min, float max, float defaultValue );
};

#endif