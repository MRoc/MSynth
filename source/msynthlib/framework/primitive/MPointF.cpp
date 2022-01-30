#include "MPointF.h"

/**
 * runtime support
 */
MRtti MPointF::gvRtti = MRtti( "MPointF", MPointF::createInstance );

/**
 * the xml attribute name storing x
 */
const String MPointF::ATTR_X = "x";

/**
 * the xml attribute name storing y
 */
const String MPointF::ATTR_Y = "y";

/**
 * constructor
 */
MPointF::MPointF() :
	ivX( 0.0f ),
	ivY( 0.0f )
{
}

/**
 * destructor
 */
MPointF::~MPointF()
{
}

/**
 * returns the x value
 */
float MPointF::getX() const
{
	return this->ivX;
}

/**
 * sets the x value
 */
void MPointF::setX( float f )
{
	this->ivX = f;
}

/**
 * returns the y value
 */
float MPointF::getY() const
{
	return this->ivY;
}

/**
 * sets the y value
 */
void MPointF::setY( float f )
{
	this->ivY = f;
}

/**
 * returns a string representation
 */
String MPointF::toString() const
{
	String back("");
	back.Format(
		"%s=%f %s=%f",
		ATTR_X.getData(),
		ivX,
		ATTR_Y.getData(),
		ivY
		);
	return back;
}

/**
 * returns the runtime type info
 */
IRtti* MPointF::getRtti() const
{
	return &gvRtti;
}

/**
 * querries for a specified interface
 */
void* MPointF::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MPointF*)this);
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else
		return MObject::getInterface( className );
}

/**
 * save as tree node
 */
MTreeNode* MPointF::save()
{
	MTreeNode* ptBack = new MTreeNode( ELEM_OBJECT );
	ptBack->setAttribute( ATTR_CLASS, getRtti()->getClassName() );
	ptBack->setAttribute( ATTR_X, MFloat::toString( ivX ) );
	ptBack->setAttribute( ATTR_Y, MFloat::toString( ivY ) );
	return ptBack;
}

/**
 * load from tree node
 */
void MPointF::load( MTreeNode* ptNode )
{
	ivX =
		MFloat::parse(
			ptNode->getAttribute( ATTR_X ) );
	ivY =
		MFloat::parse(
			ptNode->getAttribute( ATTR_Y ) );
}

/**
 * creates an instance of this class
 */
MObject* MPointF::createInstance()
{
	return new MPointF();
}