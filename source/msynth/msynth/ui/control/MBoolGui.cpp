#include "MBoolGui.h"
#include <gui/MGuiUtils.h>

INIT_RTTI( MBoolGui, "MBoolGui" );
INIT_RTTI( MBoolGui::MControlListener, "MBoolGui::MControlListener" );

MBoolGui::MBoolGui( MBoolControl* ptControl, MColor bkColor )
	: MControlGui( ptControl, bkColor ),
	ivPtListener( 0 )
{
	ASSERT( ptControl );
	ivPtListener = new MControlListener( this );
}

MBoolGui::~MBoolGui()
{
	SAFE_DELETE( ivPtListener );
}


MSize MBoolGui::getPreferredSize()
{
	return MFont::getFont()->getTextSize( ivPtControl->getName() ).add( 10, 10 );
}

void MBoolGui::paint( IGraphics* ptGraphics, const MRect &rect )
{
	MRect pRect( 1, 1, rect.getWidth() - 3, rect.getHeight() - 3 );

	// draw a sunnken button ?
	bool drawSunken = ((MBoolControl*)ivPtControl)->getValue();
	if( getCapturing() && (getMouseDownLeft()||getMouseDownRight()) && getMouseIn() )
		drawSunken = ! drawSunken;

	// select colors
	MColor bkColor = drawSunken ?
		(getIsHovering() ?
			MGuiUtils::multiplyColor( ivBkColor, 0.95f ) :
			MGuiUtils::multiplyColor( ivBkColor, 0.9f )):
				(getIsHovering() ?
					MGuiUtils::multiplyColor( ivBkColor, 1.1f ) :
					ivBkColor );

	MColor textColor =
		getEnabled() ?
			MGuiUtils::multiplyColor( ivBkColor, (FP)0.3 ) :
			MGuiUtils::multiplyColor( ivBkColor, (FP)0.6 );

	// draw background
	if( drawSunken )
		MGuiUtils::paintSunkenBorder( ptGraphics, pRect, bkColor );
	else
		MGuiUtils::paintRaisedBorder( ptGraphics, pRect, bkColor );

	// draw text
	MRect tRect( 3, 3, rect.getWidth() - 6, rect.getHeight() - 6 );
	ptGraphics->drawCenteredText(
		ivPtControl->getName(),
		tRect,
		textColor );

	// draw border if nescessary
	/*if( getShowingTooltip() )
		paintHighLight( ptGraphics, rect );*/
}

void MBoolGui::onMouseUp( MMouseEvent* ptEvent )
{
	MControlGui::onMouseUp( ptEvent );
	if( getEnabled() )
	{
		if( this->getMouseIn() )
			((MBoolControl*)ivPtControl)->setValue(
				! ((MBoolControl*)ivPtControl)->getValue() );
	}
}

bool MBoolGui::isPtInRect( MPoint point )
{
	return getRect().contains( point );
}

String MBoolGui::getToolTipText()
{
	MBoolControl* ptControl = (MBoolControl*) ivPtControl;
	String back("");
	back.Format(
		"%s %s %s", 
		ptControl->getName().getData(),
		"is turned",
		ptControl->getValue() ? "on" : "off" );

	return back;
}

MBoolGui::MControlListener::MControlListener( MBoolGui* ptGui ) :
	ivPtGui( ptGui )
{
	ivPtGui->ivPtControl->addControlListener( this );
}

MBoolGui::MControlListener::~MControlListener()
{
	ivPtGui->ivPtControl->removeControlListener( this );
}

void MBoolGui::MControlListener::valueChanged( MControlListenerEvent *anEvent )
{
	ivPtGui->updateTooltip();
	ivPtGui->repaint();
}