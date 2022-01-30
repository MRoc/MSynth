#include "MLayoutWnd.h"
#include <framework/MDefaultFactory.h>
#include <framework/io/MLogger.h>
#include "MLayoutRoot.h"

MRtti MLayoutWnd::gvRtti = MRtti( "MLayoutWnd", MLayoutWnd::createInstance );

MLayoutWnd::MLayoutWnd()
{
}

MLayoutWnd::~MLayoutWnd()
{
}

IRtti* MLayoutWnd::getRtti() const
{
	return &gvRtti;
}

void* MLayoutWnd::getInterface( const String &className ) const
{
	if( className == "MLayoutWnd" )
		return (void*) ((MLayoutWnd*)this);
	else
		return MLayoutNode::getInterface( className );
}

MSize MLayoutWnd::getSize( MWnd* ptWnd, MSize maxSize )
{
	MWnd* pWnd = ((MLayoutRoot*)getRoot())->getWnd( ivControlId );
	if( pWnd )
		return pWnd->getSize();
	else
		return ivSize;
}

void MLayoutWnd::doLayout( MWnd* ptWnd, MRect rect )
{
	MLayoutRoot* root = (MLayoutRoot*) getRoot()->getInterface( "MLayoutRoot" );
	if( root )
	{
		MWnd* pWnd = root->getWnd( ivControlId );
		if( pWnd )
			pWnd->setRect( rect );
		else
			MLogger::logWarning(
				"warning: void MLayoutWnd::doLayout failed (id=%s)", ivControlId.getData() );

		MLayoutNode::doLayout( ptWnd, rect );
	}
	else
		MLogger::logWarning( "MLayoutWnd::doLayout root->getInterface failed (id=%s)", ivControlId.getData() );
}

/**
 * loads from the given tree node
 */
void MLayoutWnd::load( MTreeNode* ptNode )
{
	MLayoutNode::load( ptNode );

	ivControlId = ptNode->getAttribute( "id" );

	if( ptNode->hasAttribute( "width" ) )
		ivSize.setWidth( MInt::parse( ptNode->getAttribute( "width" ) ) );
	if( ptNode->hasAttribute( "height" ) )
		ivSize.setHeight( MInt::parse( ptNode->getAttribute( "height" ) ) );
}

/**
 * stores as tree node
 */
MTreeNode* MLayoutWnd::save()
{
	MTreeNode* ptBack = MLayoutNode::save();
	ptBack->setAttribute( "id", ivControlId );
	return ptBack;
}

MObject* MLayoutWnd::createInstance()
{
	return new MLayoutWnd();
}

String MLayoutWnd::getId()
{
	return ivControlId;
}

MSize MLayoutWnd::getLoadedSize()
{
	return ivSize;
}