#include "MWndManager.h"
#include "event/MMouseEvent.h"
#include "event/MFocusEvent.h"
#include "event/MHoverEvent.h"
#include "event/MCaptureEvent.h"
#include "MWndNamespace.h"

/** the singleton instance */
MWndManager* MWndManager::gvInstance = 0;

/** private constructor -> singleton */
MWndManager::MWndManager() :
	ivPtFocus( 0 ),
	ivPtMouse( 0 ),
	ivPtCapture( 0 ),
	ivPtTopLevel( 0 )
{
}

/** private destructor -> singleton */
MWndManager::~MWndManager()
{
	ivPtFocus = 0;
	ivPtMouse = 0;
	ivPtCapture = 0;
	ivPtTopLevel = 0;
	ASSERT( ivWnds.size() == 0 );
}

/** return singleton instance of class */
MWndManager* MWndManager::getInstance()
{
	if( gvInstance == 0 )
		gvInstance = new MWndManager();
	return gvInstance;
}

/** release singleton instance */
void MWndManager::release()
{
	if( gvInstance )
	{
		delete gvInstance;
		gvInstance = 0;
	}
}

/** registers a instance of a window */
void MWndManager::regWnd( MWnd* pWnd )
{
	ivWnds.insert( pWnd );
}

/** unregisters a instance of a window */
void MWndManager::unregWnd( MWnd* pWnd )
{
	MWndNamespace::unregObjNS( pWnd, pWnd );
	ivWnds.erase( pWnd );
	if( pWnd == ivPtFocus )
		ivPtFocus = 0;
	if( pWnd == ivPtMouse )
		ivPtMouse = 0;
	if( pWnd == ivPtCapture )
		ivPtCapture = 0;
	if( pWnd == ivPtTopLevel )
		ivPtTopLevel = 0;
}

/** validates the given pointer points to a existing window */
bool MWndManager::isWnd( MWnd* pWnd )
{
	std::set<MWnd*>::const_iterator i = ivWnds.find( pWnd );
	return i != ivWnds.end();
}

/** sets the current level top window */
void MWndManager::setTopWnd( MWnd* pTopWnd )
{
	if( ivPtTopLevel != pTopWnd )
	{
		setMouse( 0 );
		setMouseCapture( 0 );
		setKeyFocus( 0 );
		ivPtTopLevel = pTopWnd;
	}
}

/** returns the current top level window */
MWnd* MWndManager::getTopWnd()
{
	return ivPtTopLevel;
}

/**
* sets the current mouse over window, fires
* a mouse leave event to the old one if exists
* and a hover event to the new one
*/
void MWndManager::setMouse( MWnd* pMouseOver )
{
	if( pMouseOver != ivPtMouse )
	{
		if( isWnd( ivPtMouse ) )
		{
			MHoverEvent anEvent( MHoverEvent::MHoverEventType::HOVER_LOST );
			ivPtMouse->processEvent( &anEvent );
		}
		ivPtMouse = pMouseOver;
		if( isWnd( ivPtMouse ) )
		{
			MHoverEvent anEvent( MHoverEvent::MHoverEventType::HOVER_GAINED );
			ivPtMouse->processEvent( &anEvent );
		}
	}
}

/** returns the current mouse over window */
MWnd* MWndManager::getMouse()
{
	return ivPtMouse;
}

/**
* sets the current key focus window
* fires a focus lost to the old one and focus
* gained for new one
*/
void MWndManager::setKeyFocus( MWnd* pFocusWnd )
{
	if( isWnd( ivPtFocus ) )
	{
		MFocusEvent anEvent( MFocusEvent::MFocusEventType::FOCUS_LOST );
		ivPtFocus->processEvent( &anEvent );
	}
	ivPtFocus = pFocusWnd;
	if( isWnd( ivPtFocus ) )
	{
		MFocusEvent anEvent( MFocusEvent::MFocusEventType::FOCUS_GAINED );
		ivPtFocus->processEvent( &anEvent );
	}
}

/** returns the current key focus window */
MWnd* MWndManager::getKeyFocus()
{
	return ivPtFocus;
}

/** sets the window that currently captures the mouseevents */
void MWndManager::setMouseCapture( MWnd* pCaptureWnd )
{
	if( isWnd( ivPtCapture ) )
	{
		MCaptureEvent anEvent( MCaptureEvent::MCaptureEventType::CAPTURE_LOST );
		ivPtCapture->processEvent( &anEvent );
	}
	ivPtCapture = pCaptureWnd;
	if( isWnd( ivPtCapture ) )
	{
		MCaptureEvent anEvent( MCaptureEvent::MCaptureEventType::CAPTURE_GAINED );
		ivPtCapture->processEvent( &anEvent );
	}
}

/** returns the window that currently captures the mouseevents */
MWnd* MWndManager::getMouseCapture()
{
	return ivPtCapture;
}