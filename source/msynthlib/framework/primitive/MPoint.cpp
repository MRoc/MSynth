#include "mpoint.h"

/**
 * the xml attribute name storing x
 */
const String MPoint::ATTR_X = "x";

/**
 * the xml attribute name storing y
 */
const String MPoint::ATTR_Y = "y";

/**
 * runtime support
 */
MRtti MPoint::gvRtti = MRtti( "MPoint", MPoint::createInstance );

/**
 * default constructor
 */
MPoint::MPoint() :
	ivX( 0 ),
	ivY( 0 )
{
}

/**
 * constructor
 */
MPoint::MPoint( int x, int y ) :
	ivX( x ),
	ivY( y )
{
}

/**
 * destructor
 */
MPoint::~MPoint()
{
}

/**
 * returns a string representation
 */
String MPoint::toString() const
{
	String back("");
	back.Format(
		"%s=%i %s=%i",
		ATTR_X.getData(),
		ivX,
		ATTR_Y.getData(),
		ivY );
	return back;
}

/**
 * returns the class name for runtime purpose
 */
IRtti* MPoint::getRtti() const
{
	return &gvRtti;
}

/**
 * querries for a specified interface
 */
void* MPoint::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MPoint*)this);
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else
		return MObject::getInterface( className );
}

/**
 * sets the x value
 */
void MPoint::setX( int x )
{
	ivX = x;
}

/**
 * returns the x value
 */
const int MPoint::getX() const
{
	return ivX;
}

/**
 * sets the y value
 */
void MPoint::setY( int y )
{
	ivY = y;
}

/**
 * returns the y value
 */
const int MPoint::getY() const
{
	return ivY;
}

/**
 * adds two points and returns the new point
 */
MPoint MPoint::add( MPoint p )
{
	MPoint back;
	back.setX( getX() + p.getX() );
	back.setY( getY() + p.getY() );
	return back;
}

/**
 * subs two points and returns the new point
 */
MPoint MPoint::sub( MPoint p )
{
	MPoint back;
	back.setX( getX() - p.getX() );
	back.setY( getY() - p.getY() );
	return back;
}

/**
 * multiplies the coordinates with the given
 */
MPoint MPoint::mul( int multi )
{
	return
		MPoint(
			this->ivX * multi,
			this->ivY * multi
		);
}

/**
 * returns true if the given point equals this one
 */
bool MPoint::equals( MPoint p )
{
	return this->ivX == p.getX() && this->ivY == p.getY();
}

/**
 * save as tree node
 */
MTreeNode* MPoint::save()
{
	MTreeNode* ptBack = new MTreeNode( ELEM_OBJECT );
	ptBack->setAttribute( ATTR_CLASS, getRtti()->getClassName() );
	ptBack->setAttribute( ATTR_X, MInt::toString( getX() ) );
	ptBack->setAttribute( ATTR_Y, MInt::toString( getY() ) );
	return ptBack;
}

/**
 * load from tree node
 */
void MPoint::load( MTreeNode* ptNode )
{
	ivX =
		MInt::parse(
			ptNode->getAttribute( ATTR_X ) );
	ivY =
		MInt::parse(
			ptNode->getAttribute( ATTR_Y ) );
}

/**
 * creates an instance
 */
MObject* MPoint::createInstance()
{
	return new MPoint();
}