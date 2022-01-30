#include "MTextFieldRenderer.h"
#include "MTextField.h"

/** runtime type info */
MRtti MTextFieldRenderer::gvRtti = MRtti( "MTextFieldRenderer", MTextFieldRenderer::createInstance );

/** constructor */
MTextFieldRenderer::MTextFieldRenderer() :
	ivBkColor( MColorMap::WHITE ),
	ivFgColor( MColorMap::BLACK ),
	ivFgColorSel( MColorMap::WHITE ),
	ivBgColorSel( MColorMap::BLUE ),
	ivBorderColor( MColorMap::GRAY ),
	ivCursorColor( MColorMap::BLACK ),
	ivInsets( MInsets( 3, 3, 1, 1 ) ),
	ivCursorVisible( false )
{
}

/** destructor */
MTextFieldRenderer::~MTextFieldRenderer()
{
}

/** does the paining for the given compoent */
void MTextFieldRenderer::paint( MWnd* pWnd, IGraphics* pG, const MRect& rect )
{
	MTextField* pTextField = (MTextField*) pWnd;
	String text = pTextField->getText();
	pG->fillRect( rect, ivBkColor );
	pG->drawRect( rect, ivBorderColor );
	pG->drawText( pTextField->getTextToDraw(), pTextField->getTextRect(), ivFgColor );

	if( pTextField->getModel()->hasSelection() )
	{
		int offset = pTextField->getOffset();
		int visChars = pTextField->getVisibleCharacters();

		int min = pTextField->getModel()->getSel1(), max = pTextField->getModel()->getSel2();

		if( min < offset ) min = offset;
		if( max < offset ) max = offset;

		if( min > offset + visChars )
			min = offset + visChars;
		if( max > offset + visChars )
			max = offset + visChars;

		int w1 = getTextWidth( text.Mid( offset, min-offset ) );
		int w2 = getTextWidth( text.Mid( offset, max-offset ) );
		pG->fillRect(
			MRect(
				getInsets().left() + w1,
				getInsets().top(),
				w2 - w1,
				pTextField->getHeight() - getInsets().height() ),
			ivBgColorSel );

		String toprint = text.Mid( min, max-min );
		pG->drawText(
			toprint,
			MRect(
				getInsets().left() + w1,
				getInsets().top(),
				w2-w1,
				pTextField->getHeight() - getInsets().height() ),
			ivFgColorSel );
	}

	if( getCursorVisible() &&
		pTextField->getModel()->getCursor() >= pTextField->getOffset() )
	{
		int x = getInsets().left() + getTextWidth( pTextField->getTextToCursor() );
		pG->drawLine(
			x,
			getInsets().top(),
			x,
			pTextField->getHeight() - getInsets().height(),
			ivCursorColor );
	}
}

/** sets the border insets */
void MTextFieldRenderer::setInsets( MInsets insets )
{
	ivInsets = insets;
}

/** returns the border insets */
MInsets MTextFieldRenderer::getInsets() const
{
	return ivInsets;
}

/** returns width of given text */
int MTextFieldRenderer::getTextWidth( const String& text ) const
{
	int back = 0;

	MFont* pFont = MFont::getFont();
	const char* pSrc = text.getData();
	const int* pWidths = pFont->getABCWidth();
	while( *pSrc )
	{
		back += pWidths[ (unsigned char) *pSrc ];
		pSrc++;
	}

	return back;
}

/** sets the cursor's visible state */
void MTextFieldRenderer::setCursorVisible( bool visible )
{
	ivCursorVisible = visible;
}

/** returns the cursor's visible flag */
bool MTextFieldRenderer::getCursorVisible()
{
	return ivCursorVisible;
}

/** toggles the cursor from on to off and from off to on */
void MTextFieldRenderer::toggleCursor()
{
	ivCursorVisible = ! ivCursorVisible;
}

/** query for superclass */
void* MTextFieldRenderer::getInterface( const String &className ) const
{
	if( className == "MTextFieldRenderer" )
		return (void*) ((MTextFieldRenderer*)this );
	else if( className == "IWndRenderer" )
		return (void*) ((IWndRenderer*)this );
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this );
	else
		return MObject::getInterface( className );
}

/** returns the runtime type info */
IRtti* MTextFieldRenderer::getRtti() const{ return &gvRtti; }

/** loads from the given tree node */
void MTextFieldRenderer::load( MTreeNode* ptNode )
{
	ivBkColor = MUtil::parseHexString( ptNode->getAttribute( "bgcolor", "0" ) );
	ivFgColor = MUtil::parseHexString( ptNode->getAttribute( "fgcolor", "0" ) );
	ivFgColorSel = MUtil::parseHexString( ptNode->getAttribute( "fgcolorsel", "0" ) );
	ivBgColorSel = MUtil::parseHexString( ptNode->getAttribute( "bgcolorsel", "0" ) );
	ivBorderColor = MUtil::parseHexString( ptNode->getAttribute( "bordercolor", "0" ) );
	ivCursorColor = MUtil::parseHexString( ptNode->getAttribute( "cursorcolor", "0" ) );
	ivInsets =
		MInsets(
			MInt::parse( ptNode->getAttribute( "left", "0" ) ),
			MInt::parse( ptNode->getAttribute( "right", "0" ) ),
			MInt::parse( ptNode->getAttribute( "top", "0" ) ),
			MInt::parse( ptNode->getAttribute( "bottom", "0" ) ) );
}

/** stores as tree node */
MTreeNode* MTextFieldRenderer::save()
{
	MTreeNode* pBack = new MTreeNode( ELEM_OBJECT );
	pBack->setAttribute( ATTR_CLASS, getRtti()->getClassName() );
	pBack->setAttribute( "bgcolor", MUtil::toHexString( ivBkColor ) );
	pBack->setAttribute( "fgcolor", MUtil::toHexString( ivFgColor ) );
	pBack->setAttribute( "fgcolorsel", MUtil::toHexString( ivFgColorSel ) );
	pBack->setAttribute( "bgcolorsel", MUtil::toHexString( ivBgColorSel ) );
	pBack->setAttribute( "bordercolor", MUtil::toHexString( ivBorderColor ) );
	pBack->setAttribute( "cursorcolor", MUtil::toHexString( ivCursorColor ) );
	pBack->setAttribute( "left", MInt::toString( ivInsets.left() ) );
	pBack->setAttribute( "right", MInt::toString( ivInsets.right() ) );
	pBack->setAttribute( "top", MInt::toString( ivInsets.top() ) );
	pBack->setAttribute( "bottom", MInt::toString( ivInsets.bottom() ) );
	return pBack;
}

/** creates an instance of this class */
MObject* MTextFieldRenderer::createInstance()
{
	return new MTextFieldRenderer();
}