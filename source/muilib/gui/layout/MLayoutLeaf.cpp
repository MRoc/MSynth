#include "MLayoutLeaf.h"

MRtti MLayoutLeaf::gvRtti = MRtti( "MLayoutLeaf", MLayoutLeaf::createInstance );

MLayoutLeaf::MLayoutLeaf() :
	ivPtParent( 0 )
{
}

MLayoutLeaf::~MLayoutLeaf()
{
}

IRtti* MLayoutLeaf::getRtti() const
{
	return &gvRtti;
}

void* MLayoutLeaf::getInterface( const String &className ) const
{
	if( className == "MLayoutLeaf" )
		return (void*) ((MLayoutLeaf*)this);
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else
		return MObject::getInterface( className );
}

MLayoutLeaf::SizingType MLayoutLeaf::getSizing()
{
	return ivSizing;
}

void MLayoutLeaf::setParent( MLayoutLeaf* ptParent )
{
	ivPtParent = ptParent;
}

MLayoutLeaf* MLayoutLeaf::getParent()
{
	return ivPtParent;
}

MSize MLayoutLeaf::getSize( MWnd* ptWnd, MSize maxSize )
{
	return maxSize;
}

void MLayoutLeaf::doLayout( MWnd* ptWnd, MRect rect )
{
}

/**
 * loads from the given tree node
 */
void MLayoutLeaf::load( MTreeNode* ptNode )
{
	ASSERT( ptNode->getName() == MObject::ELEM_OBJECT );
	ivSizing = str2type( ptNode->getAttribute( "sizing" ) );
}

/**
 * stores as tree node
 */
MTreeNode* MLayoutLeaf::save()
{
	MTreeNode* ptBack = new MTreeNode( MObject::ELEM_OBJECT );
	ptBack->setAttribute( MObject::ATTR_CLASS, getRtti()->getClassName() );
	ptBack->setAttribute( "sizing", type2str( ivSizing ) );
	return ptBack;
}

MLayoutLeaf* MLayoutLeaf::hasSizing( SizingType type )
{
	if( this->ivSizing == type )
		return this;
	else
		return 0;
}

MLayoutLeaf::SizingType MLayoutLeaf::str2type( String type )
{
	if( type == "FIXED" )
		return FIXED;
	else if( type == "BOTH" )
		return BOTH;
	else if( type == "VERTICAL" )
		return VERTICAL;
	else if( type == "HORIZONTAL" )
		return HORIZONTAL;
	else
		return FIXED;
}

String MLayoutLeaf::type2str( MLayoutLeaf::SizingType type )
{
	switch( type )
	{
		case FIXED:
			return "FIXED";
		case BOTH:
			return "BOTH";
		case VERTICAL:
			return "VERTICAL";
		case HORIZONTAL:
			return "HORIZONTAL";
		default:
			return "FIXED";
	}
}

MObject* MLayoutLeaf::createInstance()
{
	return new MLayoutLeaf();
}

MLayoutLeaf* MLayoutLeaf::getRoot()
{
	MLayoutLeaf* pCur = this;
	while( pCur && pCur->getParent() )
		pCur = pCur->getParent();
	ASSERT( pCur );
	return pCur;
}