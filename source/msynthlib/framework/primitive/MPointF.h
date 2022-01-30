#ifndef __MPointF
#define __MPointF

#include "MFloat.h"
#include "../treedocument/ISerializeable.h"

/**
 * defines a point with floats as values
 */
class MPointF :
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
	 * the xml attribute name storing x
	 */
	static const String ATTR_X;

	/**
	 * the xml attribute name storing y
	 */
	static const String ATTR_Y;

	/**
	 * stores the x value of the point
	 */
	float ivX;

	/**
	 * stores the y value of the point
	 */
	float ivY;

public:

	/**
	 * constructor
	 */
	MPointF();

	/**
	 * destructor
	 */
	virtual ~MPointF();

	/**
	 * returns the x value
	 */
	virtual float getX() const;

	/**
	 * sets the x value
	 */
	virtual void setX( float f );

	/**
	 * returns the y value
	 */
	virtual float getY() const;

	/**
	 * sets the y value
	 */
	virtual void setY( float f );

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
};

#endif