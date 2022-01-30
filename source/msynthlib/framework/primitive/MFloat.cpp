#include "MFloat.h"
#include <framework/io/MLogger.h>

/**
 * runtime type info
 */
MRtti MFloat::gvRtti = MRtti( "MFloat", MFloat::createInstance );

/**
 * the attribute name for value
 */
const String MFloat::ATTR_VALUE = "value";

/**
 * constructor
 */
MFloat::MFloat() :
	ivValue( 0.0f )
{
}

/**
 * constructor
 */
MFloat::MFloat( float f ) :
	ivValue( f )
{
}

/**
 * destructor
 */
MFloat::~MFloat()
{
}

/**
 * sets the stored value
 */
void MFloat::setValue( float value )
{
	ivValue = value;
}

/**
 * returns the stored value
 */
float MFloat::getValue()
{
	return ivValue;
}

/**
 * returns a string representation
 */
String MFloat::toString() const
{
	return MFloat::toString( ivValue );
}

/**
 * queries for an interface
 */
void* MFloat::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return ((MFloat*)this);
	else if( className == "ISerializeable" )
		return ((ISerializeable*)this);
	else
		return MObject::getInterface( className );
}

/**
 * returns the runtime type info
 */
IRtti* MFloat::getRtti() const
{
	return &gvRtti;
}

/**
 * creates an instance of this class
 */
MObject* MFloat::createInstance()
{
	return new MFloat();
}

/**
 * to string the given float
 */
String MFloat::toString( float f )
{
	String back("");
	back.Format( "%f", f );
	return back;
}

/**
 * parse the given string
 */
float MFloat::parse( String str )
{
	if( strlen( str ) == 0 )
		return 0.0f;
	return (float) atof( str );
}

/**
 * parse the given string
 */
float MFloat::parse( String str, float min, float max, float defaultValue )
{
	if( strlen( str ) == 0 )
	{
		//MLogger::logWarning( "<MFloat::parse msg=\"string length = 0\"/>" );
		return defaultValue;
	}

	float v = (float) atof( str.getData() );

	/*MLogger::logMessage(
		"<MFloat::parse string=\"%s\" value=\"%f\"/>",
		LPCTSTR( str ),
		v );*/

	if( v < min )
	{
		//MLogger::logWarning( "<MFloat::parse: value %f &lt; min %f", v, min );
		v = min;
	}
	else if( v > max )
	{
		//MLogger::logWarning( "<MFloat::parse: msg=\"value %f &gt; max %f", v, max );
		v = max;
	}

	return v;
}

/**
 * serialize as tree node
 */
MTreeNode* MFloat::save()
{
	MTreeNode* ptBack = new MTreeNode( ELEM_OBJECT );
	ptBack->setAttribute( ATTR_CLASS, gvRtti.getClassName() );
	ptBack->setAttribute( ATTR_VALUE, MFloat::toString( ivValue ) );
	return ptBack;
}

/**
 * load from treenode
 */
void MFloat::load( MTreeNode* node )
{
	ivValue =
		MFloat::parse( node->getAttribute( ATTR_VALUE ) );
}