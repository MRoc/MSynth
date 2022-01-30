#include "mint.h"

/**
 * the maximal number a singed int gat be
 */
const int MInt::MAX_INT = 2147483641;

/**
 * the maximal number a singed int gat be
 */
const int MInt::MIN_INT = -2147483642;

/**
 * runtime support
 */
MRtti MInt::gvRtti = MRtti( "MInt", MInt::createInstance );

/**
 * the arttribute name for storing in a xml node
 */
const String MInt::ATTR_VALUE = "value";

/**
 * default constructor
 */
MInt::MInt() :
	ivData( 0 )
{
}

/**
 * constructor
 */
MInt::MInt( const signed int value ) :
	ivData( value )
{
}

/**
 * destructor
 */
MInt::~MInt()
{
}

/**
 * returns a string representation
 */
String MInt::toString() const
{
	String buffer;
	buffer.Format( "%i", ivData );
	return buffer;
}

/**
 * returns the class name for runtime purpose
 */
IRtti* MInt::getRtti() const
{
	return &gvRtti;
}

/**
 * querries for a specified interface
 */
void* MInt::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MInt*)this);
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else
		return MObject::getInterface( className );
}

/**
 * returns the stored int value
 */
signed int MInt::getValue()
{
	return ivData;
}

/**
 * sets the stored int value
 */
void MInt::setValue( signed int value )
{
	this->ivData = value;
}


/**
 * save as tree node
 */
MTreeNode* MInt::save()
{
	MTreeNode* ptBack = new MTreeNode( ELEM_OBJECT );
	ptBack->setAttribute( ATTR_CLASS, getRtti()->getClassName() );
	ptBack->setAttribute( ATTR_VALUE, MInt::toString( getValue() ) );
	return ptBack;
}

/**
 * load from tree node
 */
void MInt::load( MTreeNode* ptNode )
{
	ivData =
		MInt::parse(
			ptNode->getAttribute( ATTR_VALUE ) );
}

/**
 * creates an instance of a MInt
 */
MObject* MInt::createInstance()
{
	return (MObject*) new MInt();
}

/**
 * parses the int in the text
 */
int MInt::parse( const char* txt )
{
	if( strlen( txt ) == 0 )
		return 0;
	return atoi( txt );
}

/**
 * parses the int in the text
 */
int MInt::parse( const char* txt, int defaultValue )
{
	if( strlen( txt ) == 0 )
		return defaultValue;
	return atoi( txt );
}

/**
 * parses the int in the text and tests it
 */
int MInt::parse( const char* txt, int min, int max, int defaultValue )
{
	if( strlen( txt ) == 0 )
		return defaultValue;
	int value = atoi( txt );
	if( value < min )
		value = min;
	else if( value > max )
		value = max;
	return value;
}

/**
 * to strings an int
 */
String MInt::toString( int number )
{
	String back;
	back.Format( "%i", number );
	return back;
}