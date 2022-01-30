/**
 * MImageWnd (C)2000-2003 MRoc
 */

#include "MImageWnd.h"
#include <framework/primitive/MObjString.h>

/** runtime type info */
MRtti MImageWnd::gvRtti = MRtti( "MImageWnd", MImageWnd::createInstance ); 

/** constructor */
MImageWnd::MImageWnd()
	: MWnd()
{
}

/** destructor */
MImageWnd::~MImageWnd()
{
}

/** loads from the given tree node */
void MImageWnd::load( MTreeNode* ptNode )
{
	MWnd::load( ptNode );
	ivImage.loadImage( ptNode->getAttribute( "image" ) );
}

/** stores as tree node */
MTreeNode* MImageWnd::save()
{
	MTreeNode* pBack = MWnd::save();
	return pBack;
}

/** creates an instance of this class */
MObject* MImageWnd::createInstance()
{
	return new MImageWnd();
}

/** returns the runtime type info */
IRtti* MImageWnd::getRtti() const
{
	return &gvRtti;
}

/** query interface */
void* MImageWnd::getInterface( const String& className ) const
{
	if( className == "MImageWnd" )
		return (void*) ((MImageWnd*)this);
	else
		return MWnd::getInterface( className );
}

/** paints */
void MImageWnd::paint( IGraphics* ptGraphics, const MRect &rect )
{
	ptGraphics->drawImage( &ivImage, 0, 0 );
}