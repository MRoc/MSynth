#include "MCheckBoxRenderer.h"

#include "MButton.h"
#include "../MGuiUtils.h"

/** runtime type info */
MRtti MCheckBoxRenderer::gvRtti = MRtti( "MCheckBoxRenderer", MCheckBoxRenderer::createInstance );

/** default constructor */
MCheckBoxRenderer::MCheckBoxRenderer() :
	ivTextColor( 0 ),
	ivTextColorDisabled( 0 )
{
}

/** constructor */
MCheckBoxRenderer::MCheckBoxRenderer( String text, MColor textColor, MColor textColorDisabled ) :
	ivText( text ),
	ivTextColor( textColor ),
	ivTextColorDisabled( textColorDisabled )
{
}

/** destructor */
MCheckBoxRenderer::~MCheckBoxRenderer()
{
}

/** does the paining for the given compoent */
void MCheckBoxRenderer::paint( MWnd* pWnd, IGraphics* pG, const MRect& rect )
{
	MButton* pButton = (MButton*) pWnd->getInterface( "MButton" );
	if( pButton )
	{
		pG->fillRect( rect, pButton->getBkColor() );

		bool pressed = pButton->isSunken();

		int qw = rect.getHeight() - 1;
		MGuiUtils::paintRaisedBorder( pG, MRect( 0, 0, qw, qw ), pButton->getBkColor() );
		pG->fillRect( MRect( 3, 3, qw - 6, qw - 6 ), MColorMap::WHITE );
        if( pressed )
		{
			pG->drawLine( 3, 3, qw-3, qw-3, MColorMap::BLACK );
			pG->drawLine( 4, 3, qw-2, qw-3, MColorMap::BLACK );

			pG->drawLine( 3, qw-4, qw-3, 2, MColorMap::BLACK );
			pG->drawLine( 4, qw-4, qw-2, 2, MColorMap::BLACK );
		}
		pG->drawText(
			ivText,
			MRect( qw+5, 0, rect.getWidth() - qw+5, rect.getHeight() ),
			pWnd->getEnabled() ? ivTextColor : ivTextColorDisabled  );
	}
}

/** query for superclass */
void* MCheckBoxRenderer::getInterface( const String &className ) const
{
	if( className == "MCheckBoxRenderer" )
		return (void*) ((MCheckBoxRenderer*)this );
	else if( className == "IWndRenderer" )
		return (void*) ((IWndRenderer*)this );
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this );
	else
		return MObject::getInterface( className );
}

/** returns the runtime type info */
IRtti* MCheckBoxRenderer::getRtti() const
{
	return &gvRtti;
}

/** loads from the given tree node */
void MCheckBoxRenderer::load( MTreeNode* ptNode )
{
	ivText = ptNode->getAttribute( "text" );
	ivTextColor = MUtil::parseHexString( ptNode->getAttribute( "textcolor", "0" ) );
	ivTextColorDisabled = MUtil::parseHexString( ptNode->getAttribute( "textcolordisabled", "0" ) );
}

/** stores as tree node */
MTreeNode* MCheckBoxRenderer::save()
{
	MTreeNode* pBack = new MTreeNode( ELEM_OBJECT );
	pBack->setAttribute( ATTR_CLASS, getRtti()->getClassName() );
	pBack->setAttribute( "text", ivText );
	pBack->setAttribute( "textcolor", MUtil::toHexString( ivTextColor ) );
	pBack->setAttribute( "textcolordisabled", MUtil::toHexString( ivTextColorDisabled ) );
	return pBack;
}

/** creates an instance of this class */
MObject* MCheckBoxRenderer::createInstance()
{
	return new MCheckBoxRenderer();
}

/** sets the text to render */
void MCheckBoxRenderer::setText(String text){ ivText = text; }

/** returns the text to render */
String MCheckBoxRenderer::getText() const { return ivText; }

/** sets the text color */
void MCheckBoxRenderer::setTextColor( MColor color ){ ivTextColor = color; }

/** returns the text color */
MColor MCheckBoxRenderer::getTextColor() const{ return ivTextColor; }

/** sets the text color */
void MCheckBoxRenderer::setTextColorDisabled( MColor color ){ ivTextColorDisabled = color; }

/** returns the text color */
MColor MCheckBoxRenderer::getTextColorDisabled() const{ return ivTextColorDisabled; }