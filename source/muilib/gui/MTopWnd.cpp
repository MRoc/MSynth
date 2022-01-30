#include "MTopWnd.h"

#include <framework/performance/MClock.h>
#include <framework/io/MLogger.h>
#include <framework/MNativeTypes.h>

#include "MApp.h"
#include "event/MMouseEvent.h"
#include "event/MHoverEvent.h"
#include "event/MKeyEvent.h"
#include "event/MFocusEvent.h"
#include "event/MCommandEvent.h"
#include "event/MPaintEvent.h"
#include "event/MActivateEvent.h"
#include "event/MSizeEvent.h"
#include "event/MDestroyEvent.h"
#include "event/MCloseEvent.h"
#include "event/MWin32ResizeRequest.h"
#include "event/MPositionEvent.h"
#include "MUiGraphics.h"
#include "MWndUtils.h"
#include "MGuiUtils.h"
#include "win32impl/MIconWin32Impl.h"
#include "win32impl/MTopWndWin32Impl.h"

/** runtime type info */
MRtti MTopWnd::gvRtti = MRtti( "MTopWnd", MTopWnd::createInstance );

/** contructor */
MTopWnd::MTopWnd() :
	ivImpl( 0 ),
	ivPtProperties( 0 ),
	ivActionOnClose( MTopWnd::ActionOnClose::QUIT )
{
	ivImpl = new MTopWndWin32Impl();
}

/** destructor */
MTopWnd::~MTopWnd()
{
	ivListeners.lock();
	unsigned int count = ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IFrameWndListener*)ivListeners.getListener( i ))->onDestroy( this );
	ivListeners.unlock();

	SAFE_DELETE( ivImpl);
}

/** creates the native window */
bool MTopWnd::create( MRect rect, MTopWnd *parentWnd )
{
	bool back = ivImpl->create( MTopWndImpl::SIMPLE_WND, rect, this, parentWnd );
	if( back )
	{
		setSize( rect.getSize() );
		doLayout();
	}
	return back;
}

/** process a system event */
bool MTopWnd::processEvent( IUiEvent* pEvent )
{
	switch( pEvent->getEventType() )
	{
	case IUiEvent::EVENT_PAINT:
		{
			ivImpl->paint( this );
			return true;
		}
		break;
	case IUiEvent::EVENT_SIZE:
		{
			TRACE( "MTopWnd::processEvent::IUiEvent::EVENT_SIZE\n" );
			MSize newSize = ((MSizeEvent*)pEvent)->getNewSize();
			if( ivRect.getWidth() != newSize.getWidth() ||
				ivRect.getHeight() != newSize.getHeight() )
			{
				setSize( newSize );
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

/** repaints the given rectangle. the rect should be in window coordinates */
void MTopWnd::repaint( const MRect& rect )
{
	ivImpl->repaint( rect );
}

/** returns the runtime type info */
IRtti* MTopWnd::getRtti() const
{
	return &gvRtti;
}

/** query interface */
void* MTopWnd::getInterface( const String& className ) const
{
	if( className == "MTopWnd" )
		return (void*) ((MTopWnd*)this);
	else
		return MWndCollection::getInterface( className );
}

/** returns the native implementation */
MTopWndImpl* MTopWnd::getImpl()
{
	return ivImpl;
}

/** set the window's properties */
void MTopWnd::setProperties( MProperties* ptProps )
{
	ivPtProperties = ptProps;
}

/** invoked when windows closed */
void MTopWnd::onClose()
{
}

/** centers the window on screen */
void MTopWnd::centerWindow()
{
	ivImpl->centerWindow();
}

/** visibles the native window */
void MTopWnd::setVisible( bool visible )
{
	if( ! getVisible() && visible )
	{
		if( ! restoreWindowPosition() )
			centerWindow();
		doLayout();
	}

	ivImpl->setVisible( visible );
}

/** sets the minimum size */
void MTopWnd::setMinimumSize( MSize size )
{
	ivMinimumSize = size;
}

/** sets the maximum size */
void MTopWnd::setMaximumSize( MSize size )
{
	ivMaximumSize = size;
}

/** sets the preferred size */
void MTopWnd::setPreferredSize( MSize size )
{
	ivPreferredSize = size;
	setPreferredClientSize( ivPreferredSize );
}

/** sets the preferred client size */
void MTopWnd::setPreferredClientSize( MSize size )
{
	MRect wRect = ivImpl->getRect();
	MSize border = getFrameBoundary();
	MRect preferredWindowRect(
		wRect.getX(),
		wRect.getX(),
		size.getWidth() + border.getWidth(),
		size.getHeight() + border.getHeight() );
	MGuiUtils::clipWindowRect( preferredWindowRect );
	setSize( preferredWindowRect.getSize() );
}

/** adds a listener to the window */
void MTopWnd::addListener( IFrameWndListener* pListener )
{
	ivListeners.addListener( pListener );
}

/** removes a listener from the window */
void MTopWnd::removeListener( IFrameWndListener* pListener )
{
	ivListeners.removeListener( pListener );
}

/** sets the action on close */
void MTopWnd::setActionOnClose( ActionOnClose actionOnClose )
{
	ivActionOnClose = actionOnClose;
}

/** returns the action on close */
MTopWnd::ActionOnClose MTopWnd::getActionOnClose()
{
	return ivActionOnClose;
}

/** returns the native icon */
MIcon* MTopWnd::getIcon()
{
	return ivImpl->getIcon();;
}

/** sets a native icon */
void MTopWnd::setIcon( MIcon* icon )
{
	ivImpl->setIcon( icon );
}

/** invoked when native command reached */
void MTopWnd::onCommand( unsigned int id )
{
}

/** stores the window position in properties */
void MTopWnd::storeWindowPosition()
{
	if( ivPtProperties )
		ivPtProperties->store( "windowposition", new MRect( ivImpl->getRect() ) );
}

/** returns the storred and clipped rect from properties */
MRect MTopWnd::getStoredClippedRect()
{
	MRect rect( 0, 0, 640, 480 );
	if( ivImpl->isCreated() )
		rect = ivImpl->getRect();
	if( ivPtProperties )
	{
		MRect* ptRect = (MRect*) ivPtProperties->get( "windowposition" );

		if( ptRect )
		{
			int left = ptRect->getX();
			int right = ptRect->getRight();
			int bottom = ptRect->getBottom();
			int top = ptRect->getY();

			if( left != -1 &&
				top != -1 &&
				right != -1 &&
				bottom != -1 )
			{	
				int screenWidth = GetSystemMetrics( SM_CXSCREEN );
				int screenHeight = GetSystemMetrics( SM_CYSCREEN );
				int width = ptRect->getWidth();
				int height = ptRect->getHeight();
				int x = left;
				int y = top;
			
				if( x + width > screenWidth )
				{
					x = screenWidth - width;
					if( x < 0 )
						x = 0;
				}
				else if( x < 0 )
					x = 0;
			    
				if( y + height > screenHeight )
				{
					y = screenHeight - height;
					if( y < 0 )
						y = 0;
				}
				else if( y < 0 )
					y = 0;

				rect.setX( x );
				rect.setY( y );
				rect.setWidth( width );
				rect.setHeight( height );
			}
		}
	}
	return rect;
}

/** returns the window position from propertiens */
bool MTopWnd::restoreWindowPosition()
{
	bool back = false;
	if( ivPtProperties )
	{
		MRect* ptRect = (MRect*) ivPtProperties->get( "windowposition" );

		if( ptRect )
		{
			MRect ePos = *ptRect;
			if( ePos.getX() != -1 &&
				ePos.getY() != -1 )
			{	
				int screenWidth = GetSystemMetrics( SM_CXSCREEN );
				int screenHeight = GetSystemMetrics( SM_CYSCREEN );
				int width = ePos.getWidth();
				int height = ePos.getHeight();
				int x = ePos.getX();
				int y = ePos.getY();
			
				if( x + width > screenWidth )
				{
					x = screenWidth - width;
					if( x < 0 )
						x = 0;
				}
				else if( x < 0 )
					x = 0;
			    
				if( y + height > screenHeight )
				{
					y = screenHeight - height;
					if( y < 0 )
						y = 0;
				}
				else if( y < 0 )
					y = 0;

				ePos.setX( x );
				ePos.setY( y );
				ePos.setWidth( width );
				ePos.setHeight( height );
				
				setRect( ePos );
				back = true;
			}
		}
	}
	return back;
}

/** returns size of boundary of the native frame, if available */
MSize MTopWnd::getFrameBoundary()
{
	return ivImpl->getFrameBoundary();
}

/** sets the window's rect */
void MTopWnd::setRect( const MRect &rect )
{
	TRACE( "MTopWnd::setRect\n" );
	MWndCollection::setRect( rect );
	ivImpl->setRect( rect );
}

/** sets the window's position */
void MTopWnd::setPoint( const MPoint& point )
{
	MWndCollection::setPoint( point );
	ivImpl->setPoint( point );
}

/** sets the window's size */
void MTopWnd::setSize( const MSize& size )
{
	TRACE( "MTopWnd::setSize\n" );
	MWndCollection::setSize( size );
	ivImpl->setSize( size );
}

/** converts a client coordinate to screen */
MPoint MTopWnd::convertClient2Screen( const MPoint& point )
{
	return ivImpl->convertClient2Screen( point );
}

/** sets the window's title */
void MTopWnd::setText( const String& text )
{
	ivImpl->setText( text );
}

/** returns the windows's title */
String MTopWnd::getText() const
{
	return ivImpl->getText();
}