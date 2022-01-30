#include "MAttributeAdder.h"

/**
 * the runtime type info
 */
MRtti MAttributeAdder::gvRtti = MRtti( "MAttributeAdder", MAttributeAdder::createInstance );

/**
 * constructor
 */
MAttributeAdder::MAttributeAdder()
{
}

/**
 * destructor
 */
MAttributeAdder::~MAttributeAdder()
{
}

/**
 * returns the runtime type info
 */
IRtti* MAttributeAdder::getRtti() const
{
	return &gvRtti;
}

/**
 * query for a interface
 */
void* MAttributeAdder::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MAttributeAdder*)this);
	else if( className == "INodeConverter" )
		return (void*) ((INodeConverter*)this);
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else
		return MObject::getInterface( className );
}

/**
 * converts the given node
 */
void MAttributeAdder::convert( MTreeNode* ptNode )
{
	ptNode->setAttribute( ivAttributeName, ivAttributeContent );
}

/**
 * loading
 */
void MAttributeAdder::load( MTreeNode* ptNode )
{
	this->ivAttributeName = ptNode->getAttribute( "name" );
	this->ivAttributeContent = ptNode->getAttribute( "content" );
}

/**
 * storing
 */
MTreeNode* MAttributeAdder::save()
{
	MTreeNode* ptBack = new MTreeNode( MObject::ELEM_OBJECT );
	ptBack->setAttribute( MObject::ATTR_CLASS, getRtti()->getClassName() );
	ptBack->setAttribute( "name", ivAttributeName );
	ptBack->setAttribute( "content", ivAttributeContent );
	return ptBack;
}

/**
 * creates a instance of this class
 */
MObject* MAttributeAdder::createInstance()
{
	return new MAttributeAdder();
}