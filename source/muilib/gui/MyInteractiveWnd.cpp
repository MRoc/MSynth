#include "MyInteractiveWnd.h"
#include <framework/io/MLogger.h>
#include "MWndManager.h"

const unsigned int MyInteractiveWnd::TOOL_TIP_TIME		= 200;

const unsigned int MyInteractiveWnd::MOUSE_IN			= 8;
const unsigned int MyInteractiveWnd::MOUSE_DOWN_LEFT	= 16;
const unsigned int MyInteractiveWnd::MOUSE_DOWN_RIGHT	= 32;
const unsigned int MyInteractiveWnd::CAPTURING			= 128;
const unsigned int MyInteractiveWnd::TOOLTIP			= 256;

MyInteractiveWnd::MyInteractiveWnd() :
	MWnd(),
	ivBitState( 0 )
{
}

MyInteractiveWnd::~MyInteractiveWnd()
{
}

void MyInteractiveWnd::setMouseIn( bool value )
{
	if( getMouseIn() != value )
	{
		ivBitState ^= MOUSE_IN;
		repaint();
	}
}

bool MyInteractiveWnd::getMouseIn() const
{
	return (ivBitState & MOUSE_IN) != 0;
}

void MyInteractiveWnd::setMouseDown( MMouseEvent::MMouseButton button, bool value )
{
	switch( button )
	{
	case MMouseEvent::BUTTON1:
		setMouseDownLeft( value );
		break;
	case MMouseEvent::BUTTON2:
		setMouseDownRight( value );
		break;
	}
}

bool MyInteractiveWnd::getMouseDown( MMouseEvent::MMouseButton button ) const
{
	switch( button )
	{
		case MMouseEvent::BUTTON1:
			return this->getMouseDownLeft();
			break;
		case MMouseEvent::BUTTON2:
			return this->getMouseDownRight();
			break;
		default:
			ASSERT( false );
	}
	return false;
}

void MyInteractiveWnd::setMouseDownLeft( bool down )
{
	if( getMouseDownLeft() != down )
	{
		ivBitState ^= MOUSE_DOWN_LEFT;
		repaint();
	}
}

bool MyInteractiveWnd::getMouseDownLeft() const
{
	return (ivBitState & MOUSE_DOWN_LEFT ) != 0;
}

void MyInteractiveWnd::setMouseDownRight( bool down )
{
	if( getMouseDownRight() != down )
	{
		ivBitState ^= MOUSE_DOWN_RIGHT;
		repaint();
	}
}

bool MyInteractiveWnd::getMouseDownRight() const
{
	return (ivBitState & MOUSE_DOWN_RIGHT ) != 0;
}

void MyInteractiveWnd::setCapturing( bool value )
{
	if( getCapturing() != value )
		ivBitState ^= CAPTURING;
}

bool MyInteractiveWnd::getCapturing() const
{
	return (ivBitState & CAPTURING) != 0;
}

void MyInteractiveWnd::setShowingTooltip( bool value )
{
	if( getShowingTooltip() != value )
		ivBitState ^= TOOLTIP;
}

bool MyInteractiveWnd::getShowingTooltip() const
{
	return (ivBitState & TOOLTIP) != 0;
}

void MyInteractiveWnd::onToolTip( bool value )
{
	/*if( getToolTipText().GetLength() > 0 )
	{
		setShowingTooltip( value );
		if( value )
			MToolTipCtrl::getInstance()->showToolTipInfo( this, getToolTipText() );
		else
			MToolTipCtrl::getInstance()->hideToolTipInfo();
		repaint();
	}*/
}

bool MyInteractiveWnd::getIsHovering() const
{
	return getMouseIn() && (! (getMouseDownLeft()||getMouseDownRight()) );
}

void MyInteractiveWnd::onMouseDown( MMouseEvent* anEvent )
{
	MWndManager::getInstance()->setKeyFocus( this );
	MWndManager::getInstance()->setMouseCapture( this );
	setMouseDown( anEvent->getButton(), true );
	repaint();
}

void MyInteractiveWnd::onMouseUp( MMouseEvent* anEvent )
{
	if( getMouseDown( anEvent->getButton() ) )
	{
		MWndManager::getInstance()->setMouseCapture( 0 );
		setMouseDown( anEvent->getButton(), false );
		repaint();
	}
}

void MyInteractiveWnd::onMouseDoubleClick( MMouseEvent* anEvent )
{
}

void MyInteractiveWnd::onMouseMove( MMouseEvent* anEvent )
{
	if( getEnabled() )
	{
		MRect cRect = MRect( MPoint(), getRect().getSize() );
		setMouseIn( cRect.contains(	anEvent->getPoint()-getScrollPos() ) );
	}
}

void MyInteractiveWnd::onMouseHover( MHoverEvent* anEvent )
{
	setMouseIn( true );
	onToolTip( true );
	repaint();
}

void MyInteractiveWnd::onMouseLeave( MHoverEvent* anEvent )
{
	setMouseIn( false );
	onToolTip( false );
	repaint();
}

void MyInteractiveWnd::onKeyDown( MKeyEvent* anEvent )
{
}

void MyInteractiveWnd::onKeyUp( MKeyEvent* anEvent )
{
}

/**
* the virtual window proc
*/
bool MyInteractiveWnd::processEvent( IUiEvent* pEvent )
{
	switch( pEvent->getEventType() )
	{
	case IUiEvent::EVENT_MOUSE:
		{
			MMouseEvent* pMouseEvent = (MMouseEvent*) pEvent;
			switch( pMouseEvent->getType() )
			{
			case MMouseEvent::MMouseEventType::MOUSE_MOVE: onMouseMove( pMouseEvent ); break;
			case MMouseEvent::MMouseEventType::MOUSE_DOUBLECLICK: onMouseDoubleClick( pMouseEvent ); break;
			case MMouseEvent::MMouseEventType::MOUSE_UP: onMouseUp( pMouseEvent ); break;
			case MMouseEvent::MMouseEventType::MOUSE_DOWN: onMouseDown( pMouseEvent ); break;
			}
			return true;
		}
		break;
	case IUiEvent::EVENT_HOVER:
		{
			MHoverEvent* pHoverEvent = (MHoverEvent*) pEvent;
			switch( pHoverEvent->getType() )
			{
			case MHoverEvent::MHoverEventType::HOVER_GAINED: onMouseHover( pHoverEvent ); break;
			case MHoverEvent::MHoverEventType::HOVER_LOST: onMouseLeave( pHoverEvent ); break;
			}
			return true;
		}
		break;
	case IUiEvent::EVENT_KEY:
		{
			MKeyEvent* pKeyEvent = (MKeyEvent*) pEvent;
			switch( pKeyEvent->getType() )
			{
			case MKeyEvent::MKeyEventType::KEY_DOWN: onKeyDown( pKeyEvent ); break;
			case MKeyEvent::MKeyEventType::KEY_INTERMEDIATE: onKeyDown( pKeyEvent ); break;
			case MKeyEvent::MKeyEventType::KEY_UP: onKeyUp( pKeyEvent ); break;
			}
			return true;
		}
		break;
	default: return MWnd::processEvent( pEvent ); break;
	}
}

String MyInteractiveWnd::getToolTipText()
{
	return ivToolTip;
}

void MyInteractiveWnd::setToolTipText( String toolTip )
{
	ivToolTip = toolTip;	
}

/** loads from the given tree node */
void MyInteractiveWnd::load( MTreeNode* ptNode )
{
	if( ptNode )
	{
		MWnd::load( ptNode );
		setToolTipText( ptNode->getAttribute( "tooltip" ) );
	}
	else
		MLogger::logWarning( "MyInteractiveWnd::load failed: no node provided" );
}

/** stores as tree node */
MTreeNode* MyInteractiveWnd::save()
{
	MTreeNode* pBack = MWnd::save();
	pBack->setAttribute( "tooltip", getToolTipText() );
	return pBack;
}