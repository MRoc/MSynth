#include "MLayoutNode.h"

MRtti MLayoutNode::gvRtti = MRtti( "MLayoutNode", 0 );

MLayoutNode::MLayoutNode()
{
}

MLayoutNode::~MLayoutNode()
{
	this->ivChilds.deleteAll();
}

IRtti* MLayoutNode::getRtti() const
{
	return &gvRtti;
}

void* MLayoutNode::getInterface( const String &className ) const
{
	if( className == "MLayoutNode" )
		return (void*)((MLayoutNode*)this);
	else
		return MLayoutLeaf::getInterface( className );
}

void MLayoutNode::addChild( MLayoutLeaf* ptChild )
{
	ptChild->setParent( this );
	this->ivChilds.add( ptChild );
}

void MLayoutNode::removeChild( MLayoutLeaf* ptChild )
{
	ivChilds.remove( ptChild );
}

unsigned int MLayoutNode::getChildCount()
{
	return this->ivChilds.getLength();
}

MLayoutLeaf* MLayoutNode::getChild( unsigned int index )
{
	return (MLayoutLeaf*) ivChilds.get( index );
}

MLayoutLeaf* MLayoutNode::hasSizing( SizingType type )
{
	if( ivSizing == type )
		return this;
	else
	{
		MLayoutLeaf* ptBack = 0;
		unsigned int childCount = this->getChildCount();
		for( unsigned int i=0;i<childCount&&ptBack==0;i++ )
			ptBack = getChild( i )->hasSizing( type );

		return ptBack;
	}
}