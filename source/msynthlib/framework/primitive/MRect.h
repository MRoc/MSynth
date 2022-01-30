#ifndef __MRect
#define __MRect

#include "../MObject.h"
#include "MPoint.h"
#include "MSize.h"

/**
 * implementation of a rectangle
 */
class MRect :
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
	 * the xml attribute name storing width
	 */
	static const String ATTR_WIDTH;

	/**
	 * the xml attribute name storing height
	 */
	static const String ATTR_HEIGHT;

	/**
	 * the point of the top left point
	 */
	int ivX, ivY, ivWidth, ivHeight;

public:

	/**
	 * default constructor
	 */
	MRect();

	/**
	 * default constructor
	 */
	MRect( const MRect& rect );

	/**
	 * constructor
	 */
	MRect( const MPoint& p, const MSize& s );

	/**
	 * constructor
	 */
	MRect( const MPoint& p1, const MPoint& p2 );

	/**
	 * constructor
	 */
	MRect( int x, int y, int width, int height );

	/**
	 * destructor
	 */
	virtual ~MRect();

    /**
	 * returns a string representation
	 */
	virtual String toString() const;

	/**
	 * returns the class name for runtime purpose
	 */
	virtual IRtti* getRtti() const;

	/**
	 * queries for a specified interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * sets the coordinate x
	 */
	virtual void setX( int x );

	/**
	 * returns the coordinate x
	 */
	virtual const int getX() const;

	/**
	 * returns the coordinate x right
	 */
	virtual const int getRight() const;

	/**
	 * sets the coordinate y
	 */
	virtual void setY( int y );

	/**
	 * returns the coordinate y
	 */
	virtual const int getY() const;

	/**
	 * returns the coordinate y bottom
	 */
	virtual const int getBottom() const;

	/**
	 * sets the width of this rect
	 */
	virtual void setWidth( int width );

	/**
	 * returns the width of this rect
	 */
	virtual const int getWidth() const;

	/**
	 * sets the height of this rect
	 */
	virtual void setHeight( int height );

	/**
	 * returns the height
	 */
	virtual const int getHeight() const;

	/**
	 * sets the point
	 */
	virtual void setPoint( const MPoint& p );

	/**
	 * returns the point at the upper left corner
	 */
	virtual MPoint getPoint() const;

	/**
	 * sets the size
	 */
	virtual void setSize( const MSize& s );

	/**
	 * returns the size of the rectangle
	 */
	virtual MSize getSize() const;

	/**
	 * returns true if the given point is contained in
	 * the rectangle
	 */
	virtual bool contains( const MPoint& p ) const;

	/**
	 * returns a point in the center of the rect
	 */
	virtual MPoint getCenter() const;

	/**
	 * returns the point at the bottom right corner
	 */
	virtual MPoint getBottomRight() const;

	/**
	 * serialize as tree node
	 */
	virtual MTreeNode* save();

	/**
	 * load from treenode
	 */
	virtual void load( MTreeNode* node );

	/**
	 * create an instance of a MRect
	 */
	static MObject* createInstance();

	/**
	 * returns true if the given rect overlaps this one
	 */
	virtual bool overlap( const MRect &rect ) const;

	/**
	 * intersection of two rectangles
	 */
	friend MRect operator*(const MRect& r1,const MRect& r2);

	/**
	 * union of two rectangles
	 */
	friend MRect operator+(const MRect& p1, const MRect& r2);

	friend MRect operator+(const MRect& r, const MPoint& p);
	friend MRect operator-(const MRect& r, const MPoint& p);
	MRect& operator+=(const MPoint &p);
	MRect& operator-=(const MPoint &p);
	friend MRect operator+(const MRect& r, const MSize& p);
	friend MRect operator-(const MRect& r, const MSize& p);
	MRect& operator+=(const MSize &p);
	MRect& operator-=(const MSize &p);

	/**
	 * union
	 */
	MRect& operator+=(const MRect &r);

	/**
	 * intersection
	 */
	MRect& operator*=(const MRect &r);
};

#endif