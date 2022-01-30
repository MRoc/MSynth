#include "MScrollBarBed.h"
#include "../MGuiUtils.h"

#include <framework/io/MLogger.h>

/**
 * the sleep time used in the inc/dec thread
 */
const int MScrollBarBed::SLEEP_TIME = 100;

INIT_RTTI( MScrollBarBed, "MScrollBarBed" );

MScrollBarBed::MScrollBarBed( MScrollBarModel* ptModel, ScrollBarType type ) :
	ivPtModel( ptModel ),
	ivType( type ),
	ivCurrentComponent( NO_COMPONENT ),
	ivTrackingMode( NO_TRACKING ),
	ivLastSliderPosition( 0 )
{
	ivPtModel->addActionListener( this );
	ivTimer.addListener( this );
}

MScrollBarBed::~MScrollBarBed()
{
	ivPtModel->removeActionListener( this );
	ivTimer.removeListener( this );
}

void MScrollBarBed::doLayout()
{
	MSize size = getSize();
	float vF = ((float)ivPtModel->getValue()) / ((float)ivPtModel->getMax());
	float iF = ((float)ivPtModel->getInner()) / ((float)ivPtModel->getMax());

	switch( ivType )
	{
		case VERTICAL:
			if( size.getHeight() > 0 && ivPtModel->getMax() > ivPtModel->getInner() )
			{
				ivSlider.setX( 0 );
				ivSlider.setY( (int)(size.getHeight() * vF) );
				ivSlider.setHeight( (int)(size.getHeight() * iF ) );
				if( ivSlider.getHeight() < 5 )
					ivSlider.setHeight( 5 );
			}
			else if( ivPtModel->getMax() <= ivPtModel->getInner() )
			{
				ivSlider.setX( 0 );
				ivSlider.setY( 0 );
				ivSlider.setHeight( size.getHeight() );
			}
			ivSlider.setWidth( size.getWidth()-1 );
			break;
		case HORIZONTAL:
			if( size.getWidth() > 0 && ivPtModel->getMax() > ivPtModel->getInner() )
			{
				ivSlider.setX( (int)(size.getWidth() * vF) );
				ivSlider.setY( 0 );
				ivSlider.setWidth( (int)(size.getWidth() * iF ) );
				if( ivSlider.getWidth() < 5 )
					ivSlider.setWidth( 5 );
			}
			else if( ivPtModel->getMax() <= ivPtModel->getInner() )
			{
				ivSlider.setX( 0 );
				ivSlider.setY( 0 );
				ivSlider.setWidth( size.getWidth() );
			}
			ivSlider.setHeight( size.getHeight()-1 );
			break;
	}
}

void MScrollBarBed::onMouseDown( MMouseEvent* ptEvent )
{
	MyInteractiveWnd::onMouseDown( ptEvent );

	ivButtonDownPoint = MPoint( ptEvent->getX(), ptEvent->getY() );

	ivLastSliderPosition = 0;
	if( ivType == VERTICAL )
		ivLastSliderPosition = ivSlider.getY();
	else if( ivType == HORIZONTAL )
		ivLastSliderPosition = ivSlider.getX();

	if( ! ivSlider.contains( ivButtonDownPoint ) )
	{
		switch( ivType )
		{
			case VERTICAL:
				this->ivTrackingMode = 
					ivButtonDownPoint.getY() < ivSlider.getY() ?
						DECREMENTING : INCREMENTING;
				break;
			case HORIZONTAL:
				this->ivTrackingMode = 
					ivButtonDownPoint.getX() < ivSlider.getX() ?
						DECREMENTING : INCREMENTING;
				break;
		}
		
		ivCurrentComponent = BED;
		ivTimer.startTimer( SLEEP_TIME );
	}
	else
	{
		ivCurrentComponent = SLIDER;
	}
	repaint();
}

void MScrollBarBed::onMouseUp( MMouseEvent* ptEvent )
{
	MyInteractiveWnd::onMouseUp( ptEvent );
	if( ivTimer.isRunnning() )
		ivTimer.stopTimer();
	ivTrackingMode = MScrollBarBed::NO_TRACKING;
}

void MScrollBarBed::onMouseMove( MMouseEvent* ptEvent )
{
	MyInteractiveWnd::onMouseMove( ptEvent );

	MPoint
		point(
			ptEvent->getX(),
			ptEvent->getY() );

	if( getMouseDown( MMouseEvent::BUTTON1 ) &&
		ivCurrentComponent == SLIDER )
	{ // pressed in the inner bed -> it was not a button who was dragged
		if( ivType == VERTICAL )
		{
			int diff = ivLastSliderPosition + (point.getY() - ivButtonDownPoint.getY());
			if( getSize().getHeight() != (int)ivSlider.getHeight() )
			{
    			float divF = diff / ((float)(getSize().getHeight() - ivSlider.getHeight()));
    			ivPtModel->setNormalizedValue( divF );
			}
		}
		else if( ivType == HORIZONTAL )
		{
			int diff = ivLastSliderPosition + (point.getX() - ivButtonDownPoint.getX());
			if( getSize().getWidth() != (int)ivSlider.getWidth() )
            {
    			float divF = diff / ((float)(getSize().getWidth() - ivSlider.getWidth()));
    			ivPtModel->setNormalizedValue( divF );
			}
		}
	}
	
	if( ! getMouseDown( MMouseEvent::BUTTON1) && updateMouseComponent( point ) )
		repaint();
}

void MScrollBarBed::onMouseHover( MHoverEvent* anEvent )
{
	MyInteractiveWnd::onMouseHover( anEvent );
}

void MScrollBarBed::onMouseLeave( MHoverEvent* anEvent )
{
	MyInteractiveWnd::onMouseLeave( anEvent );
	ivCurrentComponent = NO_COMPONENT;
}

bool MScrollBarBed::updateMouseComponent( MPoint p )
{
	Component oldC = ivCurrentComponent;
	ivCurrentComponent = ivSlider.contains( p ) ? SLIDER : BED;
	return oldC != ivCurrentComponent;
}

void MScrollBarBed::paint( IGraphics* ptGraphics, const MRect &rect )
{
	MGuiUtils::paintSunkenBorder( ptGraphics, rect, MColorMap::BK_COLOR2 );

	if( getMouseDown( MMouseEvent::BUTTON1 ) )
		MGuiUtils::paintRaisedBorder(
			ptGraphics,
			ivSlider,
			ivCurrentComponent == SLIDER ?
				MGuiUtils::multiplyColor( MColorMap::BK_COLOR3, 0.9f ) :
				MColorMap::BK_COLOR3 );
	else
		MGuiUtils::paintRaisedBorder(
			ptGraphics,
			ivSlider,
			ivCurrentComponent == SLIDER ?
				MGuiUtils::multiplyColor( MColorMap::BK_COLOR3, 1.1f ) :
				MColorMap::BK_COLOR3 );
}

void MScrollBarBed::onActionPerformed( void* ptSrc )
{
	if( ivTimer.getTimerCycles() == 1 )
	{
		if( ivTrackingMode == DECREMENTING )
			ivPtModel->decPage();
		else if( ivTrackingMode == INCREMENTING )
			ivPtModel->incPage();
	}
	else if( ivTimer.getTimerCycles() > 5 )
	{
		if( ivTrackingMode == DECREMENTING )
			ivPtModel->decPage();
		else if( ivTrackingMode == INCREMENTING )
			ivPtModel->incPage();
	}
}

void MScrollBarBed::valueChanged( void* source )
{
	doLayout();
	repaint();
}