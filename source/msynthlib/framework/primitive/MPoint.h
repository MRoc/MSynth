#ifndef __MPoint
#define __MPoint

#include "../MObject.h"
#include "../treedocument/ISerializeable.h"
#include "MInt.h"

/**
 * implementation of a 2D point object
 * storing a x and a y
 */
class MPoint :
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
	int ivX;

	/**
	 * stores the x value of the point
	 */
	int ivY;

public:

	/**
	 * default constructor
	 */
	MPoint();

	/**
	 * constructor
	 */
	MPoint( int x, int y );

	/**
	 * destructor
	 */
	virtual ~MPoint();

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
	 * sets the x value
	 */
	virtual void setX( int x );

	/**
	 * returns the x value
	 */
	virtual const int getX() const;

	/**
	 * sets the y value
	 */
	virtual void setY( int y );

	/**
	 * returns the y value
	 */
	virtual const int getY() const;

	/**
	 * adds two points and returns the new point
	 */
	virtual MPoint add( MPoint p );

	/**
	 * subs two points and returns the new point
	 */
	virtual MPoint sub( MPoint p );

	/**
	 * multiplies the coordinates with the given
	 */
	virtual MPoint mul( int multi );

	/**
	 * returns true if the given point equals this one
	 */
	virtual bool equals( MPoint p );

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

	friend bool operator==(const MPoint& p,const MPoint& q){ return p.ivX==q.ivX && p.ivY==q.ivY; }
	friend bool operator!=(const MPoint& p,const MPoint& q){ return p.ivX!=q.ivX || p.ivY!=q.ivY; }
	MPoint& operator=(const MPoint& p){ ivX=p.ivX; ivY=p.ivY; return *this; }
	MPoint& operator+=(const MPoint& p){ ivX+=p.ivX; ivY+=p.ivY; return *this; }
	MPoint& operator-=(const MPoint& p){ ivX-=p.ivX; ivY-=p.ivY; return *this; }
	MPoint& operator*=(int c){ ivX*=c; ivY*=c; return *this; }
	MPoint& operator/=(int c){ ivX/=c; ivY/=c; return *this; }
	MPoint operator-(){ return MPoint(-ivX,-ivY); }
	friend  MPoint operator+(const MPoint& p,const MPoint& q){ return MPoint(p.ivX+q.ivX,p.ivY+q.ivY); }
	friend  MPoint operator-(const MPoint& p,const MPoint& q){ return MPoint(p.ivX-q.ivX,p.ivY-q.ivY); }
	friend  MPoint operator*(const MPoint& p,int c){ return MPoint(p.ivX*c,p.ivY*c); }
	friend  MPoint operator*(int c,const MPoint& p){ return MPoint(c*p.ivX,c*p.ivY); }
	friend  MPoint operator/(const MPoint& p,int c){ return MPoint(p.ivX/c,p.ivY/c); }
	friend  MPoint operator/(int c,const MPoint& p){ return MPoint(c/p.ivX,c/p.ivY); }

	friend class MRect;
};

#endif