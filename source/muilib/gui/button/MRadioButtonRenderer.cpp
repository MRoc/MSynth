#include "MRadioButtonRenderer.h"

#include "MButton.h"
#include "../MGuiUtils.h"

/** runtime type info */
MRtti MRadioButtonRenderer::gvRtti = MRtti( "MRadioButtonRenderer", MRadioButtonRenderer::createInstance );

/** default constructor */
MRadioButtonRenderer::MRadioButtonRenderer() :
	ivTextColor( 0 ),
	ivTextColorDisabled( 0 )
{
}

/** constructor */
MRadioButtonRenderer::MRadioButtonRenderer( String text, MColor textColor, MColor textColorDisabled ) :
	ivText( text ),
	ivTextColor( textColor ),
	ivTextColorDisabled( textColorDisabled )
{
}

/** destructor */
MRadioButtonRenderer::~MRadioButtonRenderer()
{
}

/** does the paining for the given compoent */
void MRadioButtonRenderer::paint( MWnd* pWnd, IGraphics* pG, const MRect& rect )
{
	MButton* pButton = (MButton*) pWnd->getInterface( "MButton" );
	if( pButton )
	{
		pG->fillRect( rect, pButton->getBkColor() );

		bool pressed = pButton->isSunken();

		int qw = rect.getHeight() - 1;
		pG->fillEllipse( MRect( 0, 0, qw, qw ), MColorMap::WHITE );
        if( pressed )
			pG->fillEllipse( MRect( 3, 3, qw-6, qw-6 ), MColorMap::BLACK );
		pG->drawText(
			ivText,
			MRect( qw+5, 0, rect.getWidth() - qw+5, rect.getHeight() ),
			pWnd->getEnabled() ? ivTextColor : ivTextColorDisabled  );
	}
}

/** query for superclass */
void* MRadioButtonRenderer::getInterface( const String &className ) const
{
	if( className == "MRadioButtonRenderer" )
		return (void*) ((MRadioButtonRenderer*)this );
	else if( className == "IWndRenderer" )
		return (void*) ((IWndRenderer*)this );
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this );
	else
		return MObject::getInterface( className );
}

/** returns the runtime type info */
IRtti* MRadioButtonRenderer::getRtti() const
{
	return &gvRtti;
}

/** loads from the given tree node */
void MRadioButtonRenderer::load( MTreeNode* ptNode )
{
	ivText = ptNode->getAttribute( "text" );
	ivTextColor = MUtil::parseHexString( ptNode->getAttribute( "textcolor", "0" ) );
	ivTextColorDisabled = MUtil::parseHexString( ptNode->getAttribute( "textcolordisabled", "0" ) );
}

/** stores as tree node */
MTreeNode* MRadioButtonRenderer::save()
{
	MTreeNode* pBack = new MTreeNode( ELEM_OBJECT );
	pBack->setAttribute( ATTR_CLASS, getRtti()->getClassName() );
	pBack->setAttribute( "text", ivText );
	pBack->setAttribute( "textcolor", MUtil::toHexString( ivTextColor ) );
	pBack->setAttribute( "textcolordisabled", MUtil::toHexString( ivTextColorDisabled ) );
	return pBack;
}

/** creates an instance of this class */
MObject* MRadioButtonRenderer::createInstance()
{
	return new MRadioButtonRenderer();
}

/** sets the text to render */
void MRadioButtonRenderer::setText(String text){ ivText = text; }

/** returns the text to render */
String MRadioButtonRenderer::getText() const { return ivText; }

/** sets the text color */
void MRadioButtonRenderer::setTextColor( MColor color ){ ivTextColor = color; }

/** returns the text color */
MColor MRadioButtonRenderer::getTextColor() const{ return ivTextColor; }

/** sets the text color */
void MRadioButtonRenderer::setTextColorDisabled( MColor color ){ ivTextColorDisabled = color; }

/** returns the text color */
MColor MRadioButtonRenderer::getTextColorDisabled() const{ return ivTextColorDisabled; }