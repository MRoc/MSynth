#include "mtreedocument.h"

/**
 * the runtime type info of this class
 */
MRtti MTreeDocument::gvRtti = MRtti( "MTreeDocument", 0 );

/**
 * constructor
 */
MTreeDocument::MTreeDocument() :
	ivPtRoot( 0 )
{
}

/**
 * destructor
 */
MTreeDocument::~MTreeDocument()
{
	if( ivPtRoot )
		delete ivPtRoot;
}

/**
 * generates the xml
 */
void MTreeDocument::toXml( IPrintStream* ptOut )
{
	ivPtRoot->toXml( ptOut );
}

/**
 * return the root node of this tree
 */
MTreeNode* MTreeDocument::getRoot()
{
	return this->ivPtRoot;
}

/**
 * sets the root node of this tree
 */
void MTreeDocument::setTreeNode( MTreeNode* ptNode )
{
	if( ivPtRoot )
		delete ivPtRoot;
	ivPtRoot = ptNode;
}

/**
 * returns the runtime type info
 */
IRtti* MTreeDocument::getRtti() const
{
	return &gvRtti;
}

/**
 * queries for a specified interface
 */
void* MTreeDocument::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return ((MTreeDocument*)this);
	else
		return MObject::getInterface( className );
}