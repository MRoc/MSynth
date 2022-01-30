#include "MFrameWnd.h"
#include <framework/io/MLogger.h>
#include <framework/thread/IRunnable.h>
#include <framework/performance/MClock.h>
#include "MRootPane.h"
#include "../event/MMouseEvent.h"
#include "../event/MHoverEvent.h"
#include "../event/MKeyEvent.h"
#include "../event/MFocusEvent.h"
#include "../event/MCommandEvent.h"
#include "../event/MPaintEvent.h"
#include "../event/MActivateEvent.h"
#include "../event/MSizeEvent.h"
#include "../event/MDestroyEvent.h"
#include "../event/MCloseEvent.h"
#include "../event/MWin32ResizeRequest.h"
#include "../event/MPositionEvent.h"
#include "../MUiGraphics.h"
#include "../MWndUtils.h"
#include "../MApp.h"
#include "../win32impl/MMenuWin32Impl.h"

/** runtime type info */
MRtti MFrameWnd::gvRtti = MRtti( "MFrameWnd", MFrameWnd::createInstance );

/** constructor */
MFrameWnd::MFrameWnd() :
	MTopWnd(),
	ivPtRootPane( 0 ),
	ivPtMenu( 0 )
{
	ivPtRootPane = new MRootPane();
	MTopWnd::addChild( ivPtRootPane );
}

/** destructor */
MFrameWnd::~MFrameWnd()
{
	SAFE_DELETE( ivPtMenu );
}

/** creates the native window */
bool MFrameWnd::create( MRect rect, MTopWnd *parentWnd )
{
	bool back = ivImpl->create( MTopWndImpl::FRAME_WND, rect, this, parentWnd );
	if( back )
		doLayout();

	return back;
}

/** process a system event */
bool MFrameWnd::processEvent( IUiEvent* pEvent )
{
	switch( pEvent->getEventType() )
	{
	case IUiEvent::EVENT_PAINT:
		{
			ivImpl->paint( ivPtRootPane );
			return true;
		}
		break;
	case IUiEvent::EVENT_SIZE:
		{
			MSize newSize = ((MSizeEvent*)pEvent)->getNewSize();
			if( ivRect.getWidth() != newSize.getWidth() ||
				ivRect.getHeight() != newSize.getHeight() )
			{
				ivRect.setSize( newSize );
				doLayout();
			}
			return true;
		}
		break;
	case IUiEvent::EVENT_DESTROY:
		{
			ivImpl->destroy();
		}
		break;
	case IUiEvent::EVENT_CLOSE:
		{
			storeWindowPosition();

			unsigned int count = ivListeners.getListenerCount();
			for( unsigned int i=0;i<count;i++ )
				((IFrameWndListener*)ivListeners.getListener( i ))->onClose( this );

			onClose();

			switch( ivActionOnClose )
			{
				case NOTHING:
					break;
				case HIDE:
					setVisible( false );
					break;
				case DESTROY:
					delete this;
					break;
				case QUIT:
					delete this;
					MApp::getInstance()->setQuit();
					break;
			}
			return true;
		}
		break;
	case IUiEvent::EVENT_COMMAND:
		{
			MCommandEvent* pCommand = (MCommandEvent*) pEvent;
			onCommand( pCommand->getCommandId() );
		}
		break;
	case IUiEvent::EVENT_POSITION_CHANGED:
		{
			MPositionEvent* pPosEvent = (MPositionEvent*) pEvent;
			ivRect = pPosEvent->getNewRect();
		}
		break;
	case IUiEvent::MEventType::EVENT_RESIZE_REQUEST:
		{
			/*MResizeRequestEvent* pResizeEvent = (MResizeRequestEvent*) pEvent;
			MSize frameSize = getFrameBoundary();
			pResizeEvent->setMax(
				MSize(
					ivMaximumSize.getWidth() + frameSize.getWidth(),
					ivMaximumSize.getHeight() + frameSize.getHeight() ) );
			pResizeEvent->setMin(
				MSize(
					ivMinimumSize.getWidth() + frameSize.getWidth(),
					ivMinimumSize.getHeight() + frameSize.getHeight() ) );*/
			return true;
		}
		break;
	}

	return false;
}

/** adds a child to the window */
void MFrameWnd::addChild( MWnd* pWnd )
{
	ivPtRootPane->addChild( pWnd );
}

/** removes a child to the window */
void MFrameWnd::removeChild( MWnd* pWnd )
{
	ivPtRootPane->removeChild( pWnd );
}

/** returns the number of childs */
unsigned int MFrameWnd::getChildCount()
{
	return ivPtRootPane->getChildCount();
}

/** returns the number of childs */
MWnd* MFrameWnd::getChild( unsigned int index )
{
	return ivPtRootPane->getChild( index );
}

/** returns the client size of the window */
MSize MFrameWnd::getClientSize()
{
	return ivImpl->getClientSize();
}

/** sets the menu */
void MFrameWnd::setMenu( MMenu* pMenu )
{
	ivImpl->setMenu( pMenu );
}

/** return the menu */
MMenu* MFrameWnd::getMenu()
{
	return ivImpl->getMenu();
}

/** returns the runtime type info */
IRtti* MFrameWnd::getRtti() const
{
	return &gvRtti;
}

/** query interface */
void* MFrameWnd::getInterface( const String& className ) const
{
	if( className == "MFrameWnd" )
		return (void*) ((MFrameWnd*)this);
	else
		return MTopWnd::getInterface( className );
}

/** creates an instance of this class */
MObject* MFrameWnd::createInstance()
{
	return new MFrameWnd();
}

/** returns, if available, the container's namespace */
MWndNamespace* MFrameWnd::getNamespace()
{
	return ivPtRootPane->getNamespace();
}

/** sets the background color */
void MFrameWnd::setBkColor( MColor bkColor )
{
	MTopWnd::setBkColor( bkColor );
	ivPtRootPane->setBkColor( bkColor );
}

/** sets the rect */
void MFrameWnd::setRect( const MRect& rect )
{
	TRACE( "MFrameWnd::setRect\n" );
	MTopWnd::setRect( rect );
	doLayout();
}

/** sets the size */
void MFrameWnd::setSize( const MSize& size )
{
	TRACE( "MFrameWnd::setSize\n" );
	MTopWnd::setSize( size );
	doLayout();
}

/** layout components */
void MFrameWnd::doLayout()
{
	MTopWnd::doLayout();
	if( ivPtRootPane && ivImpl->isCreated() )
		ivPtRootPane->setRect( MRect( MPoint(), getClientSize() ) );
}