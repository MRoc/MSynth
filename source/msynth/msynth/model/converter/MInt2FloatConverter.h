#ifndef __Int2DoubleConverter
#define __Int2DoubleConverter

#include <framework/primitive/MInt.h>
#include <framework/primitive/MFloat.h>
#include <framework/primitive/MBool.h>

#include "INodeConverter.h"

/**
 * converts a attribute value from a int to float
 */
class MInt2FloatConverter :
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
	 * the attribute name
	 */
	String attrName;

	/**
	 * the old names
	 */
	StrList oldNames;

	/**
	 * recalculating coeffs
	 */
	float a, t, ivMin, ivMax;

	/**
	 * quad curve
	 */
	bool quad;

public:

	/**
	 * constructor
	 */
	MInt2FloatConverter();

	/**
	 * constructor
	 */
	MInt2FloatConverter( String name );

	/**
	 * convstructor
	 */
	MInt2FloatConverter( String name, float _a, float _t, float _min, float _max );

	/**
	 * destructor
	 */
	virtual ~MInt2FloatConverter();

	/**
	 * returns the attribute name that should be converted
	 */
	virtual String getAttributeName();

	/**
	 * does the conversion
	 */
	virtual void convert( MTreeNode* node );

	/**
	 * loads from tree node
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
	 * returns the number of old names
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
	 * adds a old name
	 */
	virtual void addOldName( String oldName );
};

#endif