#include "mbool.h"

/**
 * runtime support
 */
MRtti MBool::gvRtti = MRtti( "MBool", MBool::createInstance );

/**
 * the arttribute name for storing in a xml node
 */
const String MBool::ATTR_VALUE = "value";

/**
 * default constructor
 */
MBool::MBool() :
	ivData( false )
{
}

/**
 * constructor
 */
MBool::MBool( const bool value ) :
	ivData( value )
{
}

/**
 * destructor
 */
MBool::~MBool()
{
}

/**
 * returns a string representation
 */
String MBool::toString() const
{
	return ivData ? "true" : "false";
}

/**
 * returns the runtime type information
 */
IRtti* MBool::getRtti() const
{
	return &gvRtti;
}

/**
 * querries for a specified interface
 */
void* MBool::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MBool*)this);
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else
		return MObject::getInterface( className );
}

/**
 * returns the internal boolean value
 */
bool MBool::getValue()
{
	return ivData;
}

/**
 * sets the internal boolean value
 */
void MBool::setValue( bool value )
{
	ivData = value;
}

/**
 * save as tree node
 */
MTreeNode* MBool::save()
{
	MTreeNode* ptBack = new MTreeNode( ELEM_OBJECT );
	ptBack->setAttribute( ATTR_CLASS, getRtti()->getClassName() );
	ptBack->setAttribute( ATTR_VALUE, getValue() ? "1" : "0" );
	return ptBack;
}

/**
 * load from tree node
 */
void MBool::load( MTreeNode* ptNode )
{
	ivData =
		MInt::parse(
			ptNode->getAttribute( ATTR_VALUE ) ) ? true : false;
}

/**
 * creates an instance of a MBool
 */
MObject* MBool::createInstance()
{
	return new MBool();
}

/**
 * returns a string representation, 1 or 0
 */
String MBool::toString( bool b )
{
	return b ? "1" : "0";
}

/**
 * parses a bool from 0 or 1
 */
bool MBool::parse( String str, bool defaultValue )
{
	if( strlen( str ) == 0 )
		return defaultValue;
	else
		return atoi( str ) == 1 ? true : false;
}