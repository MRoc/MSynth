#include "MUiEventDispatcher.h"
#include "event/MMouseEvent.h"
#include "event/MKeyEvent.h"
#include "MWndManager.h"
#include "MWndUtils.h"
#include "MTopWnd.h"

/** constructor */
MUiEventDispatcher::MUiEventDispatcher() : ivMsgCount( 0 )
{
}

/** destructor */
MUiEventDispatcher::~MUiEventDispatcher()
{
}

/** triggers a event into the ui dispatcher */
void MUiEventDispatcher::processUiEvent( MTopWnd* pTopWnd, IUiEvent* pEvent )
{
	ivMsgCount++;
	//TRACE( "%i: %s %s\n", ivMsgCount, pTopWnd->getRtti()->getClassName().getData(), pEvent->toString().getData() );
	switch( pEvent->getEventType() )
	{
	case IUiEvent::EVENT_MOUSE:
		{
			MMouseEvent* pMouse = (MMouseEvent*) pEvent;
			MWndManager::getInstance()->setTopWnd( pTopWnd );
			switch( pMouse->getType() )
			{
			case MMouseEvent::MMouseEventType::MOUSE_MOVE:
				{
					if( MWndManager::getInstance()->getMouseCapture() )
					{
						MWnd* pCaptureControl = MWndManager::getInstance()->getMouseCapture();
						MPoint newPoint = pMouse->getPoint() - MWndUtils::getOffset( pCaptureControl ) + pCaptureControl->getScrollPos();
						MMouseEvent newEvent(
							pMouse->getButton(),
							newPoint,
							pMouse->getType(),
							pMouse->getShiftPressed(),
							pMouse->getControlPressed() );
						pCaptureControl->processEvent( &newEvent );
					}
					else
					{
						MWnd* pHitControl = MWndUtils::getHitControl( pTopWnd, pMouse->getPoint() );
						if( pHitControl )
						{
							MWndManager::getInstance()->setMouse( pHitControl );
							MPoint newPoint = pMouse->getPoint() - MWndUtils::getOffset( pHitControl )  + pHitControl->getScrollPos();
							MMouseEvent newEvent(
								pMouse->getButton(),
								newPoint,
								pMouse->getType(),
								pMouse->getShiftPressed(),
								pMouse->getControlPressed() );
							pHitControl->processEvent( &newEvent );
						}
					}
				}
				break;
			case MMouseEvent::MMouseEventType::MOUSE_DOWN:
			case MMouseEvent::MMouseEventType::MOUSE_DOUBLECLICK:
				{
					MWnd* pHitControl = MWndUtils::getHitControl( pTopWnd, pMouse->getPoint() );
					if( pHitControl )
					{
						MWndManager::getInstance()->setMouse( pHitControl );
						MPoint newPoint = pMouse->getPoint() - MWndUtils::getOffset( pHitControl )  + pHitControl->getScrollPos();
						MMouseEvent newEvent(
							pMouse->getButton(),
							newPoint,
							pMouse->getType(),
							pMouse->getShiftPressed(),
							pMouse->getControlPressed() );
						pHitControl->processEvent( &newEvent );
					}
				}
				break;
			case MMouseEvent::MMouseEventType::MOUSE_UP:
				{
					MWnd* pHitControl = MWndManager::getInstance()->getMouse();
					if( pHitControl )
					{
						MWndManager::getInstance()->setMouse( pHitControl );
						MPoint newPoint = pMouse->getPoint() - MWndUtils::getOffset( pHitControl ) + pHitControl->getScrollPos();
						MMouseEvent newEvent(
							pMouse->getButton(),
							newPoint,
							pMouse->getType(),
							pMouse->getShiftPressed(),
							pMouse->getControlPressed() );
						pHitControl->processEvent( &newEvent );

						// if captured...
						MWndManager::getInstance()->setMouse( MWndUtils::getHitControl( pTopWnd, pMouse->getPoint() ) );
					}
					else
					{
						pHitControl = MWndUtils::getHitControl( pTopWnd, pMouse->getPoint() );
						MWndManager::getInstance()->setMouse( pHitControl );
						MPoint newPoint = pMouse->getPoint() - MWndUtils::getOffset( pHitControl ) + pHitControl->getScrollPos();
						MMouseEvent newEvent(
							pMouse->getButton(),
							newPoint,
							pMouse->getType(),
							pMouse->getShiftPressed(),
							pMouse->getControlPressed() );
						pHitControl->processEvent( &newEvent );
					}
				}
				break;
			default:
				{
					MWnd* pHitControl = MWndUtils::getHitControl( pTopWnd, pMouse->getPoint() );
					MWndManager::getInstance()->setMouse( pHitControl );
					pHitControl->processEvent( pMouse );
				}
				break;
			}
		}
		break;
	case IUiEvent::EVENT_KEY:
		{
			// forward key event to focused control
			if( MWndManager::getInstance()->getKeyFocus() )
				MWndManager::getInstance()->getKeyFocus()->processEvent( pEvent );
			else
				pTopWnd->processEvent( pEvent );
		}
		break;
	default:
		// all default messages are routed to the top level window
			pTopWnd->processEvent( pEvent );
		break;
	}
}