#include "MInsets.h"

MRtti MInsets::gvRtti = MRtti( "MInsets", MInsets::createInstance );

MInsets::MInsets() :
	MObject(),
	ivLeft( 0 ),
	ivRight( 0 ),
	ivTop( 0 ),
	ivBottom( 0 )
{
}

MInsets::MInsets( int left, int right, int top, int bottom ) :
	MObject(),
	ivLeft( left ),
	ivRight( right ),
	ivTop( top ),
	ivBottom( bottom )
{
}

MInsets::~MInsets()
{
}

int MInsets::left() const{ return ivLeft; }
int MInsets::right() const{ return ivRight; }
int MInsets::top() const{ return ivTop; }
int MInsets::bottom() const{ return ivBottom; }
int MInsets::width() const{ return ivLeft + ivRight; }
int MInsets::height() const{ return ivTop + ivBottom; }

IRtti* MInsets::getRtti() const
{
	return &gvRtti;
}

void* MInsets::getInterface( const String &className ) const
{
	if( className == "MInsets" )
		return (void*) ((MInsets*)this);
	else
		return MObject::getInterface( className );
}

String MInsets::toString() const
{
	String str("");
	str.Format(
		"left: %i right: %i top: %i bottom: %i",
		ivLeft,
		ivRight,
		ivTop,
		ivBottom
		);
	return str;
}

/**
 * loads from the given tree node
 */
void MInsets::load( MTreeNode* ptNode )
{
	ASSERT( ptNode->getAttribute( MObject::ATTR_CLASS ) == "MInsets" );
	this->ivTop = MInt::parse( ptNode->getAttribute( "top" ), 0 );
	this->ivBottom = MInt::parse( ptNode->getAttribute( "bottom" ), 0 );
	this->ivLeft = MInt::parse( ptNode->getAttribute( "left" ), 0 );
	this->ivRight = MInt::parse( ptNode->getAttribute( "right" ), 0 );
}

/**
 * stores as tree node
 */
MTreeNode* MInsets::save()
{
	MTreeNode* ptBack = new MTreeNode( MObject::ELEM_OBJECT );
	ptBack->setAttribute( MObject::ATTR_CLASS, getRtti()->getClassName() );
	ptBack->setAttribute( "top", MInt::toString( ivTop ) );
	ptBack->setAttribute( "bottom", MInt::toString( ivBottom ) );
	ptBack->setAttribute( "left", MInt::toString( ivLeft ) );
	ptBack->setAttribute( "right", MInt::toString( ivRight ) );
	return ptBack;
}

MObject* MInsets::createInstance()
{
	return new MInsets();
}