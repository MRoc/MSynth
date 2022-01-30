#include "MObjectLabel.h"
#include <framework/MUtil.h>

/** constructor */
MObjectLabel::MObjectLabel() :
	MWnd(),
	ivPtObject( 0 ),
	ivTextColor( 0x00000000 ),
	ivCenterText( true )
{
}

/** constructor */
MObjectLabel::MObjectLabel( MObject* pObj, MColor textColor, MColor backGroundColor ) :
	MWnd(),
	ivPtObject( pObj ),
	ivTextColor( textColor ),
	ivCenterText( true )
{
	setBkColor( backGroundColor );
}

/** destructor */
MObjectLabel::~MObjectLabel()
{
}

/** sets the foreground color */
void MObjectLabel::setFgColor( MColor color )
{
	ivTextColor = color;
}

/** returns the foreground color */
MColor MObjectLabel::getFgColor() const
{
	return ivTextColor;
}

/** sets the objects providing the text to display */
void MObjectLabel::setObject( MObject* ptObject )
{
	ivPtObject = ptObject;
	repaint();
}

/** returns the objects providing the text to display */
MObject* MObjectLabel::getObject()
{
	return ivPtObject;
}

/** paints the label */
void MObjectLabel::paint( IGraphics* ptGraphics, const MRect &rect )
{
	ptGraphics->fillRect( rect, ivBkColor );
	if( ivPtObject )
	{
		String t = ivPtObject->toString();
		MRect r( rect.getX() + 1, rect.getY() + 1, rect.getWidth() - 2, rect.getHeight() - 2 );
		if( ivCenterText )
			ptGraphics->drawCenteredText( t, r, ivTextColor );
		else
			ptGraphics->drawText( t, r, ivTextColor );
	}
}

/** sets the center text flag */
void MObjectLabel::setCenterText( bool center )
{
	ivCenterText = center;
	repaint();
}

/** returns the center text flag */
bool MObjectLabel::getCenterText() const
{
	return ivCenterText;
}

/** loads from the given tree node */
void MObjectLabel::load( MTreeNode* ptNode )
{
	MWnd::load( ptNode );
	ivTextColor = MUtil::parseHexString( ptNode->getAttribute( "fgcolor" ) );
	ivCenterText = ptNode->getAttribute( "centered" ) == "false" ? false : true;
}

/** stores as tree node */
MTreeNode* MObjectLabel::save()
{
	MTreeNode* pBack = MWnd::save();
	pBack->setAttribute( "fgcolor", MUtil::toHexString( ivTextColor ) );
	pBack->setAttribute( "centered", ivCenterText ? "true" : "false" );
	return pBack;
}

/** creates an instance of this class */
MObject* MObjectLabel::createInstance()
{
	return new MObjectLabel();
}