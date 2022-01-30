#include "MControlGui.h"
#include <gui/framewnd/MMenu.h>
#include "../../model/automation/MTargetRegistry.h"
#include <gui/win32impl/MMenuWin32Impl.h>
#include <gui/win32impl/MTopWndWin32Impl.h>
#include <gui/MWndManager.h>


INIT_RTTI( MControlGui, "MControlGui" );

const unsigned int MControlGui::INAUTOMATION = 512;


const unsigned int MControlGui::TOOL_TIP_TIME		= 200;

const unsigned int MControlGui::MOUSE_IN			= 8;
const unsigned int MControlGui::MOUSE_DOWN_LEFT	= 16;
const unsigned int MControlGui::MOUSE_DOWN_RIGHT	= 32;
const unsigned int MControlGui::CAPTURING			= 128;
const unsigned int MControlGui::TOOLTIP			= 256;

MControlGui::MControlGui( IControl* ptControl, MColor bkColor )
	: MWndCollection(),
	ivPtControl( 0 ),
	ivBitState( 0 )
{
	ASSERT( ptControl );
	setControl( ptControl );
	setBkColor( bkColor );
}

MControlGui::~MControlGui()
{
	setControl( 0 );
}

void MControlGui::setControl( IControl* ptControl )
{
	if( ivPtControl )
	{
		ivPtControl->removeControlListener( this );
		IEventTarget* pTarget = (IEventTarget*) ivPtControl->getInterface( "IEventTarget" );
		if( pTarget )
			pTarget->removeListener( this );
	}

	ivPtControl = ptControl;

	if( ivPtControl )
	{
		IEventTarget* pTarget = (IEventTarget*) ivPtControl->getInterface( "IEventTarget" );
		if( pTarget )
		{
			setInAutomation( pTarget->getAutomationState() == IEventTarget::ADDED_TO_AUTOMATION );
			pTarget->addListener( this );
		}

		ivPtControl->addControlListener( this );
	}
}

void MControlGui::paintHighLight(  IGraphics* ptGraphics, const MRect &rect  )
{
	MColor rgb =
		getInAutomation()
		?
		MColorMap::create( 255, 0, 0 ) : MColorMap::create( 255, 255, 0 );
	ptGraphics->drawRect( rect, rgb );
	//MWndCollection::paintHighLight( ptGraphics, rect, rgb );
}

void MControlGui::onMouseUp( MMouseEvent* ptEvent )
{
	if( getMouseDown( ptEvent->getButton() ) )
	{
		MWndManager::getInstance()->setMouseCapture( 0 );
		setMouseDown( ptEvent->getButton(), false );
		repaint();
	}

	if( ptEvent->getButton() == MMouseEvent::BUTTON2 )
	{
		IEventTarget* pTarget = (IEventTarget*) ivPtControl->getInterface( "IEventTarget" );
		if( pTarget && MTargetRegistry::getInstance()->getContext( pTarget ) )
		{
			MMenu menu;
			menu.load( IDR_CONTROL_CONTEXT );

			MMenuWin32Impl* pImpl = (MMenuWin32Impl*) menu.getImpl();
			HMENU popUp = GetSubMenu( pImpl->getHandle(), 0 );
			if( getInAutomation() )
			{
				EnableMenuItem( popUp, ID_ADDTOAUTOMATION, MF_DISABLED|MF_GRAYED );
			}
			else
			{
				EnableMenuItem( popUp, ID_REMOVEFROMAUTOMATION, MF_DISABLED|MF_GRAYED );
				EnableMenuItem( popUp, ID_MENU_REMOVEALLEVENTS, MF_DISABLED|MF_GRAYED );
			}

			POINT point;
			GetCursorPos( &point );
			TrackPopupMenu(
				popUp, // HMENU
				TPM_RIGHTBUTTON, // flags
				point.x, // x
				point.y, // y
				0, // reverved
				((MTopWndWin32Impl*)((MTopWnd*)getTopParent())->getImpl())->getHandle(), // HWND
				0 // rect, ignored
				);
		}
	}
}

void MControlGui::setInAutomation( bool value )
{
	if( getInAutomation() != value )
		ivBitState ^= INAUTOMATION;
}

bool MControlGui::getInAutomation()
{
	return (ivBitState & INAUTOMATION) != 0;
}

void MControlGui::updateTooltip()
{
	if( getShowingTooltip() )
		MToolTipCtrl::getInstance()->showToolTipInfo( this, getToolTipText() );
}

void MControlGui::setEnabled( bool bEnable )
{
	repaint();
	MWnd::setEnabled( bEnable );
}

String MControlGui::toString()
{
	return this->ivPtControl->getName();
}

void MControlGui::onCommand( unsigned int id )
{
	IEventTarget* pTarget = (IEventTarget*) ivPtControl->getInterface( "IEventTarget" );
	if( pTarget )
		switch( id )
		{
		case ID_RESETTODEFAULT:
			TRACE( "<void MControlGui::onResetDefault()>\n" );
			ivPtControl->resetToDefault();
			break;
		case ID_ADDTOAUTOMATION:
			TRACE( "<void MControlGui::onAddAutomation()/>\n" );
			MTargetRegistry::getInstance()->addToAutomation( pTarget );
			break;
		case ID_REMOVEFROMAUTOMATION:
			TRACE( "<void MControlGui::onRemoveAutomation()/>\n" );
			MTargetRegistry::getInstance()->removeFromAutomation( pTarget );
			break;
		case ID_MENU_REMOVEALLEVENTS:
			TRACE( "<void MControlGui::onRemoveAllEvents()>\n" );
			MTargetRegistry::getInstance()->removeAllEvents( pTarget );
			break;
		}
}

void MControlGui::onEventTargetChange( IEventTarget* pTarget, Type type )
{
	switch( type )
	{
	case IEventTargetListener::ET_ADDED_TO_AUTOMATION:
		setInAutomation( true );
		repaint();
		break;
	case IEventTargetListener::ET_REMOVED_FROM_AUTOMATION:
		setInAutomation( false );
		repaint();
		break;
	}
}

void MControlGui::valueChanged( MControlListenerEvent *anEvent )
{
}

void MControlGui::setMouseIn( bool value )
{
	if( getMouseIn() != value )
	{
		ivBitState ^= MOUSE_IN;
		repaint();
	}
}

bool MControlGui::getMouseIn() const
{
	return (ivBitState & MOUSE_IN) != 0;
}

void MControlGui::setMouseDown( MMouseEvent::MMouseButton button, bool value )
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

bool MControlGui::getMouseDown( MMouseEvent::MMouseButton button ) const
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

void MControlGui::setMouseDownLeft( bool down )
{
	if( getMouseDownLeft() != down )
	{
		ivBitState ^= MOUSE_DOWN_LEFT;
		repaint();
	}
}

bool MControlGui::getMouseDownLeft() const
{
	return (ivBitState & MOUSE_DOWN_LEFT ) != 0;
}

void MControlGui::setMouseDownRight( bool down )
{
	if( getMouseDownRight() != down )
	{
		ivBitState ^= MOUSE_DOWN_RIGHT;
		repaint();
	}
}

bool MControlGui::getMouseDownRight() const
{
	return (ivBitState & MOUSE_DOWN_RIGHT ) != 0;
}

void MControlGui::setCapturing( bool value )
{
	if( getCapturing() != value )
		ivBitState ^= CAPTURING;
}

bool MControlGui::getCapturing() const
{
	return (ivBitState & CAPTURING) != 0;
}

void MControlGui::setShowingTooltip( bool value )
{
	if( getShowingTooltip() != value )
		ivBitState ^= TOOLTIP;
}

bool MControlGui::getShowingTooltip() const
{
	return (ivBitState & TOOLTIP) != 0;
}

void MControlGui::onToolTip( bool value )
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

bool MControlGui::getIsHovering() const
{
	return getMouseIn() && (! (getMouseDownLeft()||getMouseDownRight()) );
}

void MControlGui::onMouseDown( MMouseEvent* anEvent )
{
	MWndManager::getInstance()->setKeyFocus( this );
	MWndManager::getInstance()->setMouseCapture( this );
	setMouseDown( anEvent->getButton(), true );
	repaint();
}

void MControlGui::onMouseDoubleClick( MMouseEvent* anEvent )
{
}

void MControlGui::onMouseMove( MMouseEvent* anEvent )
{
	if( getEnabled() )
	{
		MRect cRect = MRect( MPoint(), getRect().getSize() );
		setMouseIn( cRect.contains(	anEvent->getPoint() ) );
	}
}

void MControlGui::onMouseHover( MHoverEvent* anEvent )
{
	onToolTip( true );
	repaint();
}

void MControlGui::onMouseLeave( MHoverEvent* anEvent )
{
	setMouseIn( false );
	onToolTip( false );
	repaint();
}

void MControlGui::onKeyDown( MKeyEvent* anEvent )
{
}

void MControlGui::onKeyUp( MKeyEvent* anEvent )
{
}

/**
* the virtual window proc
*/
bool MControlGui::processEvent( IUiEvent* pEvent )
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

String MControlGui::getToolTipText()
{
	return ivToolTip;
}

void MControlGui::setToolTipText( String toolTip )
{
	ivToolTip = toolTip;	
}