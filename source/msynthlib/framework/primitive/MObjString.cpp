/**
 * (C) 2001-2003 MRoc
 */


#include "MObjString.h"

/**
 * runtime support
 */
MRtti MObjString::gvRtti = MRtti( "MObjString", MObjString::createInstance );

/**
 * the arttribute name for storing in a xml node
 */
const String MObjString::ATTR_VALUE = "value";

/**
 * constructor
 */
MObjString::MObjString() :
	ivData( "" )
{
}

/**
 * constructor
 */
MObjString::MObjString( const char* string ) :
	ivData( String( string ) )
{
}

/**
 * constructor
 */
MObjString::MObjString( const MObjString& string ) :
	ivData( string.ivData  )
{
}

/**
 * destructor
 */
MObjString::~MObjString()
{
}

/**
 * returns a string representation
 */
String MObjString::toString() const
{
	return ivData;
}

/**
 * returns the class name for runtime purpose
 */
IRtti* MObjString::getRtti() const
{
	return &gvRtti;
}

/**
 * querries for a specified interface
 */
void* MObjString::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MObjString*)this);
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else
		return MObject::getInterface( className );
}

/**
 * save as tree node
 */
MTreeNode* MObjString::save()
{
	MTreeNode* ptBack = new MTreeNode( ELEM_OBJECT );
	ptBack->setAttribute( ATTR_CLASS, getRtti()->getClassName() );
	ptBack->setAttribute( ATTR_VALUE, toString() );
	return ptBack;
}

/**
 * load from tree node
 */
void MObjString::load( MTreeNode* ptNode )
{
	ivData = ( ptNode->getAttribute( ATTR_VALUE ) );
}

/**
 * creates an instance of a MObjString
 */
MObject* MObjString::createInstance()
{
	return new MObjString();
}

/**
 * sets the string
 */
void MObjString::setData( String text )
{
	ivData = text;
}