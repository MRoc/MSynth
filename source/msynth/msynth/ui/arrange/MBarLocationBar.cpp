
/*

  MBarLocationBar (C)2001 MRoc hifiShock

  This class displays a scrollable locationbar to display the location
  of a bar.
  It has a right marker and a left marker to indicate the loop markers.

*/

#include "MBarLocationBar.h"
#include <gui/MApp.h>

#define DEFAULT_BAR_COUNT 640
#define DEFAULT_BAR_WIDTH 20

#define UPDATE_STEP 1

INIT_RTTI( MBarLocationBar::MMarkerListener, "MBarLocationBar::MMarkerListener" );
INIT_RTTI( MBarLocationBar::MPlayPositionListener, "MBarLocationBar::MPlayPositionListener" );

MBarLocationBar::MBarLocationBar( int barCount )
	: MyInteractiveWnd(),
	ivPtUpdateThread( 0 ),
	ivPtTimeController( 0 ),
	ivLButtonDown( false ),
	ivRButtonDown( false ),
	ivPtMarkerListener( 0 ),
	ivPtPlayPositionListener( 0 )
{	
	setBarCount( barCount );
	setBarWidth( DEFAULT_BAR_WIDTH );

	ivBkColor = MColorMap::BK_COLOR3;

	ivCurrentPattern = 0;
	ivCurrentStep = 0;
}

MBarLocationBar::~MBarLocationBar()
{
	if( ivPtUpdateThread )
		setUpdateThread( 0 );
	if( ivPtTimeController )
		setTimeController( 0 );
}

void MBarLocationBar::paint( IGraphics* ptGraphics, const MRect &rect )
{
	MPoint target = getScrollPos();
   	MSize size = getSize();

	int width = size.getWidth();
	int height = size.getHeight();
	int halfHeight = height / 2;

	ivPolyLeft[0].setX( 0 );
	ivPolyLeft[0].setY( halfHeight );
	ivPolyLeft[1].setX( 0 );
	ivPolyLeft[1].setY( height );
	ivPolyLeft[2].setX( ivBarWidth / 4 );
	ivPolyLeft[2].setY( height );

	ivPolyRight[0].setX( ivBarWidth );
	ivPolyRight[0].setY( halfHeight );
	ivPolyRight[1].setX( ivBarWidth );
	ivPolyRight[1].setY( height );
	ivPolyRight[2].setX( ivBarWidth / 4 * 3 );
	ivPolyRight[2].setY( height );

	ivPolyPlayPos[0].setX( - ivBarWidth / 8 );
	ivPolyPlayPos[0].setY( halfHeight );
	ivPolyPlayPos[1].setX( ivBarWidth / 8 );
	ivPolyPlayPos[1].setY( halfHeight );
	ivPolyPlayPos[2].setX( 0 );
	ivPolyPlayPos[2].setY( height );

	ptGraphics->fillRect( 0, 0, width, height, ivBkColor );

	ptGraphics->setOffset( ptGraphics->getOffset() - target );

	// Draw vertical lines
	int xSrc = 0;
	int y1a = 0;
	int y1b = halfHeight;
	int y2 = height;
	int dX = ivBarWidth;

	int leftPos = xSrc + ivBarWidth * ivPtTimeController->getLeftMarker();
	int rightPos = xSrc + ivBarWidth * ivPtTimeController->getRightMarker();
	int currentPlayPos = int(xSrc + (ivCurrentPattern * ivBarWidth) + ( ivCurrentStep * ivBarWidth ) / 32.0f);

    int visibleIndexLeft = getScrollPos().getX() / ivBarWidth;
	int maxVisible = width / ivBarWidth;
	int end = visibleIndexLeft + maxVisible + 2;
	if( end > ivBarCount )
		end = ivBarCount;
	for( int i=visibleIndexLeft;i<end;i++)
	{
		int x = xSrc + i*dX;
		int y = y1b;
		if( i % 4 == 0 )
		{
			ptGraphics->drawText(
				MInt::toString( i+1 ),
				MRect( x + 2, 2, 20, 12 ),
				MColorMap::FG_COLOR3 );
			y = y1a;
		}
		ptGraphics->drawLine( x, y, x, y2, MColorMap::FG_COLOR3 );
	}

	ivPolyLeft[0].setX( ivPolyLeft[0].getX() + leftPos );
	ivPolyLeft[1].setX( ivPolyLeft[1].getX() + leftPos );
	ivPolyLeft[2].setX( ivPolyLeft[2].getX() + leftPos );
	ptGraphics->poly( ivPolyLeft, 3, MColorMap::FG_COLOR2, MColorMap::FG_COLOR3 );
	
	ivPolyRight[0].setX( ivPolyRight[0].getX() + rightPos );
	ivPolyRight[1].setX( ivPolyRight[1].getX() + rightPos );
	ivPolyRight[2].setX( ivPolyRight[2].getX() + rightPos );
	ptGraphics->poly( ivPolyRight, 3, MColorMap::FG_COLOR2, MColorMap::FG_COLOR3 );
	
	ivPolyPlayPos[0].setX( ivPolyPlayPos[0].getX() + currentPlayPos );
	ivPolyPlayPos[1].setX( ivPolyPlayPos[1].getX() + currentPlayPos );
	ivPolyPlayPos[2].setX( ivPolyPlayPos[2].getX() + currentPlayPos );
	ptGraphics->poly( ivPolyPlayPos, 3, MColorMap::FG_COLOR2, MColorMap::FG_COLOR3 );

	ptGraphics->setOffset( ptGraphics->getOffset() + target );

	ptGraphics->drawLine(
		0,
		rect.getHeight()-1,
		rect.getWidth(),
		rect.getHeight()-1,
		MColorMap::FG_COLOR3 );
}

void MBarLocationBar::setTimeController( MTimeController* ptTimeController )
{
	if( ivPtTimeController )
	{
		ivPtTimeController->removeMarkerListener( ivPtMarkerListener );
		SAFE_DELETE( ivPtMarkerListener );
	}

	ivPtTimeController = ptTimeController;

	if( ivPtTimeController )
	{
		ivPtMarkerListener = new MMarkerListener( this );
		ivPtTimeController->addMarkerListener( ivPtMarkerListener );
		if( getVisible() )
			repaint();
	}
}

void MBarLocationBar::setUpdateThread( MUpdateThread* ptUpdateThread )
{
	if( ivPtUpdateThread )
		SAFE_DELETE( ivPtPlayPositionListener );
	ivPtUpdateThread = ptUpdateThread;
	if( ivPtUpdateThread )
		ivPtPlayPositionListener = new MPlayPositionListener( this, ivPtUpdateThread );
}

MTimeController* MBarLocationBar::getTimeController()
{
	return ivPtTimeController;
}

void MBarLocationBar::setBarCount( int barCount )
{
	ivBarCount = barCount;
}

int	MBarLocationBar::getBarCount()
{
	return ivBarCount;
}

void MBarLocationBar::setBarWidth( int barWidth )
{
	ivBarWidth = barWidth;
}

int MBarLocationBar::getBarWidth()
{
	return ivBarWidth;
}

void MBarLocationBar::onMouseUp( MMouseEvent* anEvent )
{
	MyInteractiveWnd::onMouseUp( anEvent );
	if( anEvent->getButton() == MMouseEvent::BUTTON1 )
	{
		ivPtTimeController->setLeftMarker( getPosition( anEvent->getX() ) );
		ivLButtonDown = false;
		repaint();
	}
	else if( anEvent->getButton() == MMouseEvent::BUTTON2 )
	{
		ivPtTimeController->setRightMarker( getPosition( anEvent->getX()  ) );
		ivRButtonDown = false;
		repaint();
	}
}

void MBarLocationBar::onMouseDoubleClick( MMouseEvent* anEvent )
{
	MyInteractiveWnd::onMouseDoubleClick( anEvent );
	if( anEvent->getButton() == MMouseEvent::BUTTON1 )
	{
		ivCurrentPattern = getPosition( anEvent->getX() );
		TRACE( "<MBarLocationBar::OnLButtonDblClk pattern=\"%i\"/>\n", ivCurrentPattern );
		ivPtTimeController->setCurrentPatternEx( ivCurrentPattern );
		repaint();
	}
}

int MBarLocationBar::getPosition( int xInPixel )
{
	return (xInPixel) / ivBarWidth;
}

MBarLocationBar::MMarkerListener::MMarkerListener( MBarLocationBar* ptOwner ) :
	ivPtOwner( ptOwner )
{
}

MBarLocationBar::MMarkerListener::~MMarkerListener()
{
}

void MBarLocationBar::MMarkerListener::markerChanged()
{
	ivPtOwner->repaint();
}

MBarLocationBar::MPlayPositionListener::MPlayPositionListener(
	MBarLocationBar* ptView,
	MUpdateThread* ptModel ) :
	ivPtView( ptView ),
	ivPtModel( ptModel )
{
	ivPtModel->addPlayPositionListener( this );
}

MBarLocationBar::MPlayPositionListener::~MPlayPositionListener()
{
	ivPtModel->removePlayPositionListener( this );
}

/**
 * invoked from the update thread when play position changed
 */
void MBarLocationBar::MPlayPositionListener::playPositionChanged( int pattern, int step )
{
	ivPtView->ivCurrentPattern = pattern;
	ivPtView->ivCurrentStep = step;
	MApp::getInstance()->doWorkInGuiThread( this );
}

void MBarLocationBar::MPlayPositionListener::run()
{
	ivPtView->repaint();
}