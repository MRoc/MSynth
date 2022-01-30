#include "MElementRenamer.h"


/**
 * runtime type info
 */
MRtti MElementRenamer::gvRtti = MRtti( "MElementRenamer", MElementRenamer::createInstance );

/**
 * constructor
 */
MElementRenamer::MElementRenamer()
{
}

/**
 * destructor
 */
MElementRenamer::~MElementRenamer()
{
}

/**
 * converts the given node
 */
void MElementRenamer::convert( MTreeNode* ptNode )
{
	ptNode->setName( this->ivNewName );
}

/**
 * queries for a interface
 */
void* MElementRenamer::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*)((MElementRenamer*)this);
	else if( className == "INodeConverter" )
		return (void*)((INodeConverter*)this);
	else if( className == "ISerializeable" )
		return (void*)((ISerializeable*)this);
	else
		return MObject::getInterface( className );
}

/**
 * returns the runtime type info
 */
IRtti* MElementRenamer::getRtti() const
{
	return &gvRtti;
}

/**
 * loading
 */
void MElementRenamer::load( MTreeNode* ptNode )
{
	ASSERT( ptNode->getName() == MObject::ELEM_OBJECT );
	ASSERT( ptNode->getAttribute( MObject::ATTR_CLASS ) == getRtti()->getClassName() );
	this->ivNewName = ptNode->getAttribute( "name" );
}

/**
 * storing
 */
MTreeNode* MElementRenamer::save()
{
	MTreeNode* ptBack = new MTreeNode( MObject::ELEM_OBJECT );
	ptBack->setAttribute( MObject::ATTR_CLASS, getRtti()->getClassName() );
	ptBack->setAttribute( "name", this->ivNewName );
	return ptBack;
}

/**
 * create a instanceof this class
 */
MObject* MElementRenamer::createInstance()
{
	return new MElementRenamer();
}
