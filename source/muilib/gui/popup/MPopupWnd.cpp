#include "MPopupWnd.h"
#include <framework/io/MLogger.h>
#include "../MApp.h"
#include "../event/MActivateEvent.h"

/** runtime type info */
MRtti MPopupWnd::gvRtti = MRtti( "MPopupWnd", MPopupWnd::createInstance );

/** constructor */
MPopupWnd::MPopupWnd()
{
}

/** destructor */
MPopupWnd::~MPopupWnd()
{
	ivListeners.clear();
}

/** creates the window */
bool MPopupWnd::create( MRect rect, MTopWnd* ptParent )
{
	return ivImpl->create( MTopWndImpl::POPUP_WND, rect, this, ptParent );
	/*ivHwnd =
		CreateWindowEx(
			WS_EX_TOPMOST,
			"MTopWnd",
			"MTopWnd",
			WS_POPUP, // WINDOW STYLE
			rect.getX(),
			rect.getY(),
			rect.getWidth(),
			rect.getHeight(),
			ptParent ? ((MTopWnd*)ptParent->getTopParent())->getHwnd() : 0, // HWND PARENT
			NULL, // HMENU
			MApp::getInstance()->getHInstance(),
			0 );
	if( ivHwnd != NULL )
		gvMapHwnd2Wnd.insert( Hwnd2OsWndMap::value_type( ivHwnd, this ) );
	else
	{
		DWORD lastError = GetLastError();
		LPVOID lpMsgBuf;
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			lastError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL 
		);
		MLogger::logError(
			"MyDoubleBufferedWnd::create: CreateWindow failed: \"%s\" (no. %i))", lpMsgBuf, lastError );
		LocalFree( lpMsgBuf );
	}

	return ivHwnd != NULL;*/

}

/** adds a popup listener */
void MPopupWnd::addListener( IPopupListener* ptListener )
{
	ivListeners.addListener( ptListener );
}

/** removes a popup listener */
void MPopupWnd::removeListener( IPopupListener* ptListener )
{
	ivListeners.removeListener( ptListener );
}

/** fires a notification when the popup changed */
void MPopupWnd::firePopupChanged( bool visible )
{
	unsigned int count = ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IPopupListener*)ivListeners.getListener( i ))->onPopupChanged( this, visible );
}

/** paints the content */
void MPopupWnd::paint( IGraphics* ptDc, const MRect &rect )
{
	ptDc->fillRect( rect, MColorMap::BK_COLOR1 );
}

bool MPopupWnd::processEvent( IUiEvent* pEvent )
{
	switch( pEvent->getEventType() )
	{
	case IUiEvent::EVENT_ACTIVATE:
		{
			if( ((MActivateEvent*)pEvent)->getType() == MActivateEvent::ACTIVATION )
			{
				doLayout();
			}
			else if( ((MActivateEvent*)pEvent)->getType() == MActivateEvent::DEACTIVATION )
			{
				firePopupChanged( false );
				setVisible( false );
			}
			return true;
		}
		break;
	}
	return MTopWnd::processEvent( pEvent );
}

/** returns the runtime type info */
IRtti* MPopupWnd::getRtti() const
{
	return &gvRtti;
}

/** returns an instance of this class */
MObject* MPopupWnd::createInstance()
{
	return new MPopupWnd();
}