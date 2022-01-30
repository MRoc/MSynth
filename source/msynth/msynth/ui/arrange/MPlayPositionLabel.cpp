#include "MPlayPositionLabel.h"
#include <gui/counter/MCountControl.h>
#include <gui/MApp.h>

#define HEIGHT 14
#define WIDTH 150
#define BL_WIDTH 45

INIT_RTTI( MPlayPositionLabel::MPlayPositionListener, "MPlayPositionLabel::MPlayPositionListener" );

MPlayPositionLabel::MPlayPositionLabel(
	MColor bkColor,
	MColor fontColor,
	const String& text )
	:
	MWndCollection(),
	ivPtUpdateThread( 0 ),
	ivPtPlayPositionListener( 0 ),
	ivPtLabel( 0 ),
	ivPtCounterPattern( 0 ),
	ivPtCounterStep( 0 )
{
	ivBkColor = bkColor;
	ivPtLabel = new MLabel( text, fontColor, ivBkColor );
	ivPtLabel->setCenterText( false );
	ivPtCounterPattern = new MCounter( 3 );
	ivPtCounterStep = new MCounter( 2 );
	addChild( ivPtLabel );
	addChild( ivPtCounterPattern );
	addChild( ivPtCounterStep );
}

void MPlayPositionLabel::setUpdateThread( MUpdateThread* ptThread )
{
	if( ivPtPlayPositionListener )
		SAFE_DELETE( ivPtPlayPositionListener );
	if( ptThread )
		ivPtPlayPositionListener = new MPlayPositionListener( this, ptThread );
}

void MPlayPositionLabel::setFgColor( MColor color )
{
	ivPtLabel->setFgColor( color );
}

void MPlayPositionLabel::setText( const String& text )
{
	ivPtLabel->setText( text );
}

MPlayPositionLabel::~MPlayPositionLabel()
{
	SAFE_DELETE( ivPtPlayPositionListener );
}

void MPlayPositionLabel::doLayout()
{
	ivPtLabel->setRect( MRect( 0, 0, 95, 12 ) );

	ivPtCounterPattern->setRect(
		MRect(
			100,
			0,
			ivPtCounterPattern->getWidth(),
			ivPtCounterPattern->getHeight() ) );

	ivPtCounterStep->setRect(
		MRect(
			130,
			0,
			ivPtCounterStep->getWidth(),
			ivPtCounterStep->getHeight() ) );
}

MPlayPositionLabel::MPlayPositionListener::MPlayPositionListener(
	MPlayPositionLabel* ptView,
	MUpdateThread* ptModel ) :
	ivPtView( ptView ),
	ivPtModel( ptModel ),
	ivPattern( 0 ),
	ivStep( 0 )
{
	ivPtModel->addPlayPositionListener( this );
}

MPlayPositionLabel::MPlayPositionListener::~MPlayPositionListener()
{
	ivPtModel->removePlayPositionListener( this );
}

void MPlayPositionLabel::MPlayPositionListener::playPositionChanged( int pattern, int step )
{
	ivPattern = pattern + 1;
	ivStep = step + 1;
	MApp::getInstance()->doWorkInGuiThread( this );
}

void MPlayPositionLabel::MPlayPositionListener::run()
{
	ivPtView->ivPtCounterPattern->setValue( ivPattern );
	ivPtView->ivPtCounterStep->setValue( ivStep );
	ivPtView->repaint();
}

void MPlayPositionLabel::paint( IGraphics* pG, const MRect &rect )
{
	pG->fillRect( rect, ivBkColor );
}