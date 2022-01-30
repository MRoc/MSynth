/*(C)2000 MRoc hifiShock*/

#include "MFlatTextButtonRenderer.h"
#include "MButton.h"
#include "../MGuiUtils.h"

/** runtime type info */
MRtti MFlatTextButtonRenderer::gvRtti = MRtti( "MFlatTextButtonRenderer", MFlatTextButtonRenderer::createInstance );

/** constructor */
MFlatTextButtonRenderer::MFlatTextButtonRenderer() :
	ivTextColor( 0 )
{
}

/** constructor */
MFlatTextButtonRenderer::MFlatTextButtonRenderer( String text, MColor textCol ) :
	ivText( text ),
	ivTextColor( textCol )
{
}

/** destructor */
MFlatTextButtonRenderer::~MFlatTextButtonRenderer()
{
}

/** pints the button */
void MFlatTextButtonRenderer::paint( MWnd* pWnd, IGraphics* ptGraphics, const MRect &rect )
{
//	TRACE( "MFlatTextButtonRenderer::paint text=\"%s\"\n", getText().getData() );
	MButton* pButton = (MButton*) pWnd->getInterface( "MButton" );
	if( pButton )
	{
		ptGraphics->fillRect(
			rect,
			pButton->isSunken() ?
				MGuiUtils::multiplyColor( pButton->getBkColor(), 1.2f ) :
				pButton->getBkColor() );

		if( ivText.GetLength() > 0 )
		{
			ptGraphics->drawCenteredText(
				ivText,
				rect,
				ivTextColor );
		}
	}
}

/** sets the text to render */
void MFlatTextButtonRenderer::setText(String text){ ivText = text; }

/** returns the text to render */
String MFlatTextButtonRenderer::getText() const { return ivText; }

/** sets the text color */
void MFlatTextButtonRenderer::setTextColor( MColor color ){ ivTextColor = color; }

/** returns the text color */
MColor MFlatTextButtonRenderer::getTextColor() const{ return ivTextColor; }

/** query for superclass */
void* MFlatTextButtonRenderer::getInterface( const String &className ) const
{
	if( className == "MFlatTextButtonRenderer" )
		return (void*) ((MFlatTextButtonRenderer*)this );
	else if( className == "IWndRenderer" )
		return (void*) ((IWndRenderer*)this );
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this );
	else
		return MObject::getInterface( className );
}

/** returns the runtime type info */
IRtti* MFlatTextButtonRenderer::getRtti() const{ return &gvRtti; }

/** loads from the given tree node */
void MFlatTextButtonRenderer::load( MTreeNode* ptNode )
{
	ivText = ptNode->getAttribute( "text" );
	ivTextColor = MUtil::parseHexString( ptNode->getAttribute( "textcolor", "0" ) );
}

/** stores as tree node */
MTreeNode* MFlatTextButtonRenderer::save()
{
	MTreeNode* pBack = new MTreeNode( ELEM_OBJECT );
	pBack->setAttribute( ATTR_CLASS, getRtti()->getClassName() );
	pBack->setAttribute( "text", ivText );
	pBack->setAttribute( "textcolor", MUtil::toHexString( ivTextColor ) );
	return pBack;
}

/** creates an instance of this class */
MObject* MFlatTextButtonRenderer::createInstance()
{
	return new MFlatTextButtonRenderer();
}