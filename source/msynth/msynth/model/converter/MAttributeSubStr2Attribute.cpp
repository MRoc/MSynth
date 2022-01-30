#include "MAttributeSubStr2Attribute.h"

/**
 * the runtime type info
 */
MRtti MAttributeSubStr2Attribute::gvRtti = MRtti( "MAttributeSubStr2Attribute", MAttributeSubStr2Attribute::createInstance );

/**
 * constructor
 */
MAttributeSubStr2Attribute::MAttributeSubStr2Attribute() :
	ivIndex( 0 ),
	ivMatchingType( MAttributeMatch::END )
{
}

/**
 * destructor
 */
MAttributeSubStr2Attribute::~MAttributeSubStr2Attribute()
{
}

/**
 * returns the runtime type info
 */
IRtti* MAttributeSubStr2Attribute::getRtti() const
{
	return &gvRtti;
}

/**
 * converts the given node
 */
void MAttributeSubStr2Attribute::convert( MTreeNode* ptNode )
{
	String str = ptNode->getAttribute( ivAttributeName );
	String newContent = "";
	switch( ivMatchingType )
	{
		case MAttributeMatch::BEGIN:
			newContent = str.Left( this->ivIndex );
			break;
		case MAttributeMatch::END:
			newContent = str.Right( str.GetLength() - this->ivIndex );
			break;
		default:
			newContent = str;
			break;
	}
	ptNode->setAttribute( ivNewAttributeName, newContent );
}

/**
 * loading
 */
void MAttributeSubStr2Attribute::load( MTreeNode* ptNode )
{
	ivIndex = (unsigned int)MInt::parse( ptNode->getAttribute( "index" ), 0 );
	ivMatchingType = MAttributeMatch::getMatchingType( ptNode->getAttribute( "matchingtype" ) );
	ivAttributeName = ptNode->getAttribute( "attributename" );
	ivNewAttributeName = ptNode->getAttribute( "newattributename" );
}

/**
 * storing
 */
MTreeNode* MAttributeSubStr2Attribute::save()
{
	MTreeNode* ptBack = new MTreeNode( MObject::ELEM_OBJECT );
	ptBack->setAttribute( MObject::ATTR_CLASS, getRtti()->getClassName() );
	ptBack->setAttribute( "index", MInt::toString( (int)ivIndex ) );
	ptBack->setAttribute( "matchingtype", MAttributeMatch::getMatchingTypeAsString( ivMatchingType ) );
	ptBack->setAttribute( "attributename", ivAttributeName );
	ptBack->setAttribute( "newattributename", ivNewAttributeName );
	return ptBack;
}

/**
 * creates a instance of this class
 */
MObject* MAttributeSubStr2Attribute::createInstance()
{
	return new MAttributeSubStr2Attribute();
}

/**
 * query for a interface
 */
void* MAttributeSubStr2Attribute::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MAttributeSubStr2Attribute*)this);
	else if( className == "INodeConverter" )
		return (void*) ((INodeConverter*)this);
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else
		return MObject::getInterface( className );
}