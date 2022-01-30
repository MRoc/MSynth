#include "MTextButtonRenderer.h"
#include "MButton.h"
#include "../MGuiUtils.h"

/** runtime type info */
MRtti MTextButtonRenderer::gvRtti = MRtti( "MTextButtonRenderer", MTextButtonRenderer::createInstance );

/** default constructor */
MTextButtonRenderer::MTextButtonRenderer() :
	ivTextColor( 0 ),
	ivTextColorDisabled( 0 )
{
}

/** constructor */
MTextButtonRenderer::MTextButtonRenderer( String text, MColor textColor, MColor textColorDisabled ) :
	ivText( text ),
	ivTextColor( textColor ),
	ivTextColorDisabled( textColorDisabled )
{
}

/** destructor */
MTextButtonRenderer::~MTextButtonRenderer()
{
}

/** does the paining for the given compoent */
void MTextButtonRenderer::paint( MWnd* pWnd, IGraphics* pG, const MRect& rect )
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
		pG->drawCenteredText(
			ivText,
			rect + offset,
			pWnd->getEnabled() ? ivTextColor : ivTextColorDisabled  );
	}
}

/** query for superclass */
void* MTextButtonRenderer::getInterface( const String &className ) const
{
	if( className == "MTextButtonRenderer" )
		return (void*) ((MTextButtonRenderer*)this );
	else if( className == "IWndRenderer" )
		return (void*) ((IWndRenderer*)this );
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this );
	else
		return MObject::getInterface( className );
}

/** returns the runtime type info */
IRtti* MTextButtonRenderer::getRtti() const
{
	return &gvRtti;
}

/** loads from the given tree node */
void MTextButtonRenderer::load( MTreeNode* ptNode )
{
	ivText = ptNode->getAttribute( "text" );
	ivTextColor = MUtil::parseHexString( ptNode->getAttribute( "textcolor", "0" ) );
	ivTextColorDisabled = MUtil::parseHexString( ptNode->getAttribute( "textcolordisabled", "0" ) );
}

/** stores as tree node */
MTreeNode* MTextButtonRenderer::save()
{
	MTreeNode* pBack = new MTreeNode( ELEM_OBJECT );
	pBack->setAttribute( ATTR_CLASS, getRtti()->getClassName() );
	pBack->setAttribute( "text", ivText );
	pBack->setAttribute( "textcolor", MUtil::toHexString( ivTextColor ) );
	pBack->setAttribute( "textcolordisabled", MUtil::toHexString( ivTextColorDisabled ) );
	return pBack;
}

/** creates an instance of this class */
MObject* MTextButtonRenderer::createInstance()
{
	return new MTextButtonRenderer();
}