/**
 * (C)2001-2003 MRoc
 * TODO: replace fuckin window msgs by listeners
 */

#include "MCountControl.h"

#define BWIDTH 9
#define BHEIGHT 12

INIT_RTTI_CREATE( MCountControl, "MCountControl" );

MCountControl::MCountControl() :
	MWndCollection(),
	ivPtLabel( 0 ),
	ivPtSpinbutton( 0 ),
	ivPtCounter( 0 ),
	ivRange( 3 ),
	ivTextLabelWidth( 0 )
{
	ivBkColor = MColorMap::WHITE;

	ivPtLabel = new MLabel( "TEXT", MColorMap::BLACK, ivBkColor );
	ivPtLabel->setCenterText( false );
	ivPtLabel->setSize( MSize( ivTextLabelWidth, BHEIGHT ) );
	ivPtLabel->setBkColor( ivBkColor );
	ivPtLabel->setFgColor( MColorMap::BLACK );
	ivPtCounter = new MCounter( ivRange );
	ivPtCounter->setBkColor( ivBkColor );
	ivPtSpinbutton = new MSpinButton( MSpinButton::HORIZONTAL );
	ivPtSpinbutton->setBkColor( ivBkColor );

	addChild( ivPtLabel );
	addChild( ivPtCounter );
	addChild( ivPtSpinbutton );

	ivPtSpinbutton->getModel()->setMin( 0 );
	ivPtSpinbutton->getModel()->setMax( 999 );
	ivPtSpinbutton->getModel()->setValue( 0 );

	MSize size = ivPtCounter->getPreferredSize();
	setSize( MSize( ivRange * BWIDTH + 34 + 80, BHEIGHT ) );

	ivPtSpinbutton->getModel()->addListener( this );
}

MCountControl::MCountControl(
	unsigned int range,
	int min,
	int max,
	MColor bgColor,
	MColor fgColor,
	String text,
	int labelWidth )
	:
	MWndCollection(),
	ivPtLabel( 0 ),
	ivPtSpinbutton( 0 ),
	ivPtCounter( 0 ),
	ivRange( range ),
	ivTextLabelWidth( labelWidth )
{
	ivBkColor = bgColor;

	ivPtLabel = new MLabel( text, fgColor, ivBkColor );
	ivPtLabel->setCenterText( false );
	ivPtLabel->setSize( MSize( ivTextLabelWidth, BHEIGHT ) );
	ivPtLabel->setBkColor( ivBkColor );
	ivPtLabel->setFgColor( fgColor );
	ivPtCounter = new MCounter( ivRange );
	ivPtCounter->setBkColor( ivBkColor );
	ivPtSpinbutton = new MSpinButton( MSpinButton::HORIZONTAL );
	ivPtSpinbutton->setBkColor( ivBkColor );

	addChild( ivPtLabel );
	addChild( ivPtCounter );
	addChild( ivPtSpinbutton );

	ivPtSpinbutton->getModel()->setMin( min );
	ivPtSpinbutton->getModel()->setMax( max );
	ivPtSpinbutton->getModel()->setValue( min );

	MSize size = ivPtCounter->getPreferredSize();
	setSize( MSize( ivRange * BWIDTH + 34 + ivTextLabelWidth, BHEIGHT ) );

	ivPtSpinbutton->getModel()->addListener( this );
}

MCountControl::~MCountControl()
{
	ivPtSpinbutton->getModel()->removeListener( this );
}

void MCountControl::doLayout()
{
	MSize counterSize = ivPtCounter->getPreferredSize();
	ivPtLabel->setRect( MRect( 0, 0, ivTextLabelWidth, getHeight() ) );
	ivPtCounter->setRect( MRect( ivTextLabelWidth, 0, counterSize.getWidth(), getHeight() ) );
	ivPtSpinbutton->setRect( MRect( ivTextLabelWidth + counterSize.getWidth() + 2, 0, 32, getHeight() ) );
}

int MCountControl::getPos()
{
	return ivPtSpinbutton->getModel()->getValue();
}

void MCountControl::setPos( int pos )
{
	ivPtSpinbutton->getModel()->setValue( pos );
	ivPtCounter->setValue( pos );
}

void MCountControl::valueChanged( MSpinButtonModel* pModel )
{
	ivPtCounter->setValue( getPos() );
}

MSpinButtonModel* MCountControl::getModel()
{
	return ivPtSpinbutton->getModel();
}

void MCountControl::paint( IGraphics* pG, const MRect& rect )
{
	pG->fillRect( rect, ivBkColor );
}

/** store ui state into treenode */
MTreeNode* MCountControl::save()
{
	MTreeNode* pBack = MWnd::save();
	pBack->setAttribute( "fgcolor", MUtil::toHexString( ivPtLabel->getFgColor() ) );
	pBack->setAttribute( "range", MInt::toString( ivPtCounter->getRange() ) );
	pBack->setAttribute( "text", ivPtLabel->getText() );
	pBack->setAttribute( "labelwidth", MInt::toString( ivTextLabelWidth ) );
	return pBack;
}

/** load ui state from treenode */
void MCountControl::load( MTreeNode* pNode )
{
	MWnd::load( pNode );
	ivPtLabel->setBkColor( ivBkColor );
	ivPtLabel->setFgColor( MUtil::parseHexString( pNode->getAttribute( "fgcolor", "0" ) ) );
	ivPtLabel->setText( pNode->getAttribute( "text" ) );
	ivPtCounter->setRange( MInt::parse( pNode->getAttribute( "range", 3 ) ) );
	ivPtCounter->setBkColor( ivBkColor );
	ivTextLabelWidth =
		pNode->hasAttribute( "labelwidth" ) ?
			MInt::parse( pNode->getAttribute( "labelwidth" ) ) :
			MFont::getFont()->getTextSize( ivPtLabel->getText() ).getWidth() + 5;
	doLayout();
}