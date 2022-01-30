/**
 * (C)2000-2003 MRoc
 */

#include "MSplitBar.h"
#include "../event/MMouseEvent.h"
#include <framework/MNativeTypes.h>
#include <framework/io/MLogger.h>

MSplitBar::MSplitBar( Mode mode ) :
	MWnd(),
	ivMode( mode ),
	ivIsSelected( false )
{
	setFgColor( RGB( 100, 100, 100 ) );
	setBkColor( RGB( 200, 200, 200 ) );
}

MSplitBar::~MSplitBar()
{
	ivListenerList.clear();
}

void MSplitBar::paint( IGraphics* ptGraphics, const MRect &rect )
{
	MColor bkColor;
	MColor fgColor;

	if( ivIsSelected )
	{
		bkColor = ivFgColor;
		fgColor = ivBkColor;
	}
	else
	{
		bkColor = ivBkColor;
		fgColor = ivFgColor;
	}

	ptGraphics->fillRect( rect, bkColor );
	ptGraphics->drawRect( rect, fgColor );
}

void MSplitBar::setFgColor( MColor color )
{
	ivFgColor = color;
}

MColor MSplitBar::getFgColor()
{
	return ivFgColor;
}

void MSplitBar::setSelected( bool value )
{
	ivIsSelected = value;
}

bool MSplitBar::getSelected()
{
	return ivIsSelected;
}

bool MSplitBar::processEvent( IUiEvent* pEvent )
{
	switch( pEvent->getEventType() )
	{
	case IUiEvent::EVENT_MOUSE:
		{
			MMouseEvent* pMouseEvent = (MMouseEvent*) pEvent;
			if( pMouseEvent->getType() == MMouseEvent::MMouseEventType::MOUSE_DOWN ) 
			{
				setSelected( true );
				repaint();
				fireActionPerformed( (void*) this );
			}
		}
		break;
	}
	return MWnd::processEvent( pEvent );
}

bool MSplitBar::onSetCursor()
{
	HCURSOR c = 
		LoadCursor(
			NULL,
			ivMode == HORIZONTAL ? MAKEINTRESOURCE( IDC_SIZEWE ) : MAKEINTRESOURCE( IDC_SIZENS ) );
	if( ! c )
		MLogger::logError(
			"MSplitBar::onSetCursor: LoadCursor failed" );
	else
		SetCursor( c );

	return c != 0;
}

void MSplitBar::addActionListener( IActionListener* ptListener )
{
	this->ivListenerList.addListener( ptListener );
}

void MSplitBar::removeActionListener( IActionListener* ptListener )
{
	this->ivListenerList.removeListener( ptListener );
}

void MSplitBar::fireActionPerformed( void* ptSrc )
{
	this->ivListenerList.fireActionPerformed( (void*) this );
}