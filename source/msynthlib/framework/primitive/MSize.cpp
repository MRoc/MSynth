#include "MSize.h"

/**
 * runtime support
 */
MRtti MSize::gvRtti = MRtti( "MSize", MSize::createInstance );

/**
 * the width attribute tag used for xml serialisation
 */
const String MSize::ATTR_WIDTH = "width";

/**
 * the height attribute tag used for xml serialisation
 */
const String MSize::ATTR_HEIGHT = "height";


/**
 * default constructor
 */
MSize::MSize() :
	MObject(),
	ivWidth( 0 ),
	ivHeight( 0 )
{
}

/**
 * default constructor
 */
MSize::MSize( const MSize& toCopy ) :
	MObject(),
	ivWidth( toCopy.ivWidth ),
	ivHeight( toCopy.ivHeight )
{
}

/**
 * constructor
 */
MSize::MSize( int width, int height ) :
	MObject(),
	ivWidth( width ),
	ivHeight( height )
{
}

/**
 * destructor
 */
MSize::~MSize()
{
}

/**
 * sets the stored width
 */
void MSize::setWidth( int width )
{
	ivWidth = width;
}

/**
 * returns the stored width
 */
const int MSize::getWidth() const
{
	return ivWidth;
}

/**
 * sets the stored height
 */
void MSize::setHeight( int height )
{
	ivHeight = height;
}

/**
 * returns the stored height
 */
const int MSize::getHeight() const
{
	return ivHeight;
}

/**
 * adds value to the height
 */
MSize& MSize::addY( int value )
{
	ivHeight += value;
	return *this;
}

/**
 * adds value to the width
 */
MSize& MSize::addX( int value )
{
	ivWidth += value;
	return *this;
}

/**
 * adds wx to the width
 * adds hy to the height
 */
MSize& MSize::add( int wx, int hy )
{
	ivWidth += wx;
	ivHeight += hy;
	return *this;
}

/**
 * returns a string representation
 */
String MSize::toString() const
{
	String back("");
	back.Format(
		"%s=%i %s=%i",
		ATTR_WIDTH.getData(),
		ivWidth,
		ATTR_HEIGHT.getData(),
		ivHeight );
	return back;
}

/**
 * returns the class name for runtime purpose
 */
IRtti* MSize::getRtti() const
{
	return &gvRtti;
}

/**
 * querries for a specified interface
 */
void* MSize::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MSize*)this);
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else
		return MObject::getInterface( className );
}

/**
 * save as tree node
 */
MTreeNode* MSize::save()
{
	MTreeNode* ptBack = new MTreeNode( ELEM_OBJECT );
	ptBack->setAttribute( ATTR_CLASS, getRtti()->getClassName() );
	ptBack->setAttribute( ATTR_WIDTH, MInt::toString( getWidth() ) );
	ptBack->setAttribute( ATTR_HEIGHT, MInt::toString( getHeight() ) );
	return ptBack;
}

/**
 * load from tree node
 */
void MSize::load( MTreeNode* ptNode )
{
	ivWidth = MInt::parse( ptNode->getAttribute( ATTR_WIDTH ) );
	ivHeight = MInt::parse( ptNode->getAttribute( ATTR_HEIGHT ) );
}

/**
 * creates an instance of this class
 */
MObject* MSize::createInstance()
{
	return new MSize();
}