#include "MArrowButtonRenderer.h"
#include "MButton.h"
#include "../MGuiUtils.h"

/** runtime type info */
MRtti MArrowButtonRenderer::gvRtti = MRtti( "MArrowButtonRenderer", MArrowButtonRenderer::createInstance );

/** constructor */
MArrowButtonRenderer::MArrowButtonRenderer() :
	ivDirection( MArrowRenderer::DOWN )
{
}

/** constructor */
MArrowButtonRenderer::MArrowButtonRenderer( MArrowRenderer::Direction direction ) :
	ivDirection( direction )
{
}

/** destructor */
MArrowButtonRenderer::~MArrowButtonRenderer()
{
}

/** does the paining for the given compoent */
void MArrowButtonRenderer::paint( MWnd* pWnd, IGraphics* pG, const MRect& rect )
{
	MButton* pButton = (MButton*) pWnd->getInterface( "MButton" );
	if( pButton )
	{
		bool pressed = pButton->isSunken();
		MColor bkColor = pressed ?
			(pButton->getIsHovering() ?
				MGuiUtils::multiplyColor( pButton->getBkColor(), 0.95f ) :
				MGuiUtils::multiplyColor( pButton->getBkColor(), 0.9f )) :
					(pButton->getIsHovering() ?
						MGuiUtils::multiplyColor( pButton->getBkColor(), 1.1f ) :
						pButton->getBkColor() );

		if( pressed )
			MGuiUtils::paintSunkenBorder( pG, rect, bkColor );
		else
			MGuiUtils::paintRaisedBorder( pG, rect, bkColor );

		MPoint offset( 0, 0 );
		if( pressed )
			offset = MPoint( 1, 1 );
		MArrowRenderer::paintArrow( pG, rect, offset, ivDirection );
	}
}

/** query for superclass */
void* MArrowButtonRenderer::getInterface( const String &className ) const
{
	if( className == "MArrowButtonRenderer" )
		return (void*) ((MArrowButtonRenderer*) this );
	else if( className == "IWndRenderer" )
		return (void*) ((IWndRenderer*) this );
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*) this );
	else
		return MObject::getInterface( className );
}

/** returns the runtime type info */
IRtti* MArrowButtonRenderer::getRtti() const
{
	return &gvRtti;
}

/** loads from the given tree node */
void MArrowButtonRenderer::load( MTreeNode* ptNode )
{
	ivDirection = MArrowRenderer::parse( ptNode->getAttribute( "direction" ).getData() );
}

/** stores as tree node */
MTreeNode* MArrowButtonRenderer::save()
{
	MTreeNode* pBack = new MTreeNode( ELEM_OBJECT );
	pBack->setAttribute( ATTR_CLASS, getRtti()->getClassName() );
	pBack->setAttribute( "direction", MArrowRenderer::toString( ivDirection ) );
	return pBack;
}

/** creates an instance of this class */
MObject* MArrowButtonRenderer::createInstance()
{
	return new MArrowButtonRenderer();
}