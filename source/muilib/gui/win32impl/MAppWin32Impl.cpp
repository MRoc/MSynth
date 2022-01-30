#include "MAppWin32Impl.h"

#include "../MApp.h"
#include "MAppWin32Impl.h"
#include "../MTopWnd.h"
#include "../MWndManager.h"
#include "../event/MPositionEvent.h"
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
#include "../event/MNativeEvent.h"
#include "../event/MPositionEvent.h"
#include <framework/thread/IRunnable.h>
#include "../MFont.h"
#include <framework/io/MLogger.h>
#include "MTopWndWin32Impl.h"

#define WM_POST_WORK_TO_GUITHREAD ( WM_USER + 9345 )
#define WM_END_MODAL ( WM_USER + 9346 )

unsigned int MAppWin32Impl::gvMsgCount = 0;

/** constructor */
MAppWin32Impl::MAppWin32Impl() :
	ivHInstance( 0 )
{
}

/** destructor */
MAppWin32Impl::~MAppWin32Impl()
{
}

/** do the message process */
int MAppWin32Impl::work()
{
	MSG msg;
	while( GetMessage( &msg, NULL, 0, 0 ) == TRUE && msg.message != WM_QUIT ) 
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	switch( msg.message )
	{
	case WM_QUIT:
		PostQuitMessage( (int) msg.wParam );
		return (int) msg.wParam;
		break;
	default:
		return -1;
	}

	/*if( ivAccTable.isLoaded() &&
		MAppWin32Impl::getMainWnd() &&
		MAppWin32Impl::getMainWnd()->getHwnd() &&
		TranslateAccelerator(
			MAppWin32Impl::getMainWnd()->getHwnd(),
			ivAccTable.getHandle(),
			&msg ) )
	{ 
		TRACE( "accelerator\n" );
	}
	else
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}*/
}

/** the message loop for modal stack */
int MAppWin32Impl::modalWork()
{
	MSG msg;
	while(
		GetMessage( &msg, NULL, 0, 0 ) == TRUE &&
		msg.message != WM_QUIT &&
		msg.message != WM_END_MODAL ) 
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	switch( msg.message )
	{
	case WM_QUIT:
		PostQuitMessage( (int) msg.wParam );
	case WM_END_MODAL:
		return (int) msg.wParam;
		break;
	default:
		return -1;
	}
}

/** sets the native app object */
void MAppWin32Impl::setHandle( HINSTANCE hInstance )
{
	ivHInstance = hInstance;
}

/** returns the native app object */
HINSTANCE MAppWin32Impl::getHandle()
{
	return ivHInstance;
}

/** sets the quit flag on true */
void MAppWin32Impl::setQuit()
{
	PostQuitMessage( 0 );
}

/** the code behind pToRun will be invoked from ui thread */
void MAppWin32Impl::doWorkInGuiThread( IRunnable* pToRun )
{
	PostMessage( 0, WM_POST_WORK_TO_GUITHREAD, 0, (LPARAM) pToRun );
}

/** the window proc */
LRESULT MAppWin32Impl::staticWindowProc(
	HWND wnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam )
{
	gvMsgCount++;

	if( message == WM_POST_WORK_TO_GUITHREAD )
	{
		((IRunnable*)lParam)->run();
		return 0;
	}

	MTopWnd* pWnd = MTopWndWin32Impl::getWnd( wnd );
	if( ! pWnd )
	{
		TRACE( "%i: unknown window dispatch\n", gvMsgCount );
		return DefWindowProc( wnd, message, wParam, lParam );
	}

	if( pWnd->isNative() )
	{
		MNativeEvent anEvent( wnd, message, wParam, lParam );
		MApp::getInstance()->getDisptacher()->processUiEvent( pWnd, &anEvent );
		return anEvent.getReturnCode();
	}

	switch( message )
	{
	case WM_PAINT:
		{
			MPaintEvent anEvent;
			MApp::getInstance()->getDisptacher()->processUiEvent( pWnd, &anEvent );
			return 0;
		}
		break;
	/*case WM_SETCURSOR:
		{
			ASSERT( wnd );
			if( onSetCursor() )
				return TRUE;
			else
				return DefWindowProc( wnd, message, wParam, lParam );
		}
		break;*/
	case WM_POST_WORK_TO_GUITHREAD:
		{
			IRunnable* ptRunnable = (IRunnable*) lParam;
			ptRunnable->run();
			return TRUE;
		}
		break;
	/*case WM_SIZING:
	case WM_SIZE:
		{
			RECT rect;
			GetClientRect( wnd, &rect );
			MSizeEvent anEvent( MSize( rect.right - rect.left, rect.bottom - rect.top ) );
			MAppWin32Impl::getInstance()->ivDispatcher.processUiEvent( pWnd, &anEvent );
		}
		break;*/
	/*case WM_SIZING:
		{
			RECT* lpRect = (RECT*) (&lParam);
			MSizeEvent anEvent(
				MSize(
					lpRect->right - lpRect->left,
					lpRect->bottom - lpRect->top ) );
			MAppWin32Impl::getInstance()->ivDispatcher.processUiEvent( pWnd, &anEvent );
		}
		break;*/
	case WM_MOUSEMOVE:
		{
			if( ! MApp::getInstance()->getModal() || MApp::getInstance()->getModal() == pWnd )
			{
				int iXMouse=(short)(lParam&0x0000ffff);
				int iYMouse=(short)((lParam&0xffff0000)>>16);
				MMouseEvent anEvent(
					MMouseEvent::NEUTRAL,
					MPoint( iXMouse, iYMouse ),
					MMouseEvent::MOUSE_MOVE, (wParam&MK_SHIFT)!=0, (wParam&MK_CONTROL)!=0 );
				MApp::getInstance()->getDisptacher()->processUiEvent( pWnd, &anEvent );
				return true;
			}
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		{
			if( ! MApp::getInstance()->getModal() || MApp::getInstance()->getModal() == pWnd )
			{
				SetCapture(wnd);
				SetFocus(wnd);
				int iXMouse=(short)(lParam&0x0000ffff);
				int iYMouse=(short)((lParam&0xffff0000)>>16);
				MMouseEvent anEvent(
					message == WM_LBUTTONDOWN ? MMouseEvent::BUTTON1 : MMouseEvent::BUTTON2,
					MPoint( iXMouse, iYMouse ),
					MMouseEvent::MOUSE_DOWN,
					(wParam&MK_SHIFT)!=0,
					(wParam&MK_CONTROL)!=0 );
				MApp::getInstance()->getDisptacher()->processUiEvent( pWnd, &anEvent );
				return true;
			}
			else
				MessageBeep( 0 );
		}
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		{
			if( ! MApp::getInstance()->getModal() || MApp::getInstance()->getModal() == pWnd )
			{
				ReleaseCapture();
				int iXMouse=(short)(lParam&0x0000ffff);
				int iYMouse=(short)((lParam&0xffff0000)>>16);
				MMouseEvent anEvent(
					message == WM_LBUTTONUP ? MMouseEvent::BUTTON1 : MMouseEvent::BUTTON2,
					MPoint( iXMouse, iYMouse ),
					MMouseEvent::MOUSE_UP,
					(wParam&MK_SHIFT)!=0,
					(wParam&MK_CONTROL)!=0 );
				MApp::getInstance()->getDisptacher()->processUiEvent( pWnd, &anEvent );
				return true;
			}
			else
				MessageBeep( 0 );
		}
		break;
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
		{
			if( ! MApp::getInstance()->getModal() || MApp::getInstance()->getModal() == pWnd )
			{
				int iXMouse=(short)(lParam&0x0000ffff);
				int iYMouse=(short)((lParam&0xffff0000)>>16);
				MMouseEvent anEvent(
					message == WM_LBUTTONDBLCLK ? MMouseEvent::BUTTON1 : MMouseEvent::BUTTON2,
					MPoint( iXMouse, iYMouse ),
					MMouseEvent::MOUSE_DOUBLECLICK,
					(wParam&MK_SHIFT)!=0,
					(wParam&MK_CONTROL)!=0 );
				MApp::getInstance()->getDisptacher()->processUiEvent( pWnd, &anEvent );
				return true;
			}
			else
				MessageBeep( 0 );
		}
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		{
			if( ! MApp::getInstance()->getModal() || MApp::getInstance()->getModal() == pWnd )
			{
				BYTE keyBoardState[ 257 ];
				if( GetKeyboardState( keyBoardState ) == TRUE )
				{
					unsigned int repeatCount = (unsigned int)(lParam&0xFF);
					unsigned int scanCode = (unsigned int)((lParam>>16)&0xF);
					
					WORD buffer[3];
					int res =
						ToAscii(
							(unsigned int)wParam,
							scanCode,
							keyBoardState,
							buffer,
							0 );

					MKeyEvent anEvent(
						( message == WM_KEYDOWN || message == WM_SYSKEYDOWN ) ?
							( repeatCount == 1 ? MKeyEvent::KEY_DOWN : MKeyEvent::KEY_INTERMEDIATE ) :
							( MKeyEvent::KEY_UP ),
						MKeyEvent::convert( (unsigned int)wParam ),
						res ? buffer[0] : 0 );

					MApp::getInstance()->getDisptacher()->processUiEvent( pWnd, &anEvent );
				}
				else
					MLogger::logError(
						"MAppWin32Impl::windowProc: GetKeyboardState in WM_KEY failed" );
			}
			else
				MessageBeep( 0 );
		}
		break;
	case WM_ACTIVATE:
		{
			if( ! MApp::getInstance()->getModal() || MApp::getInstance()->getModal() == pWnd )
			{
				if( wParam == WA_INACTIVE )
				{
					MActivateEvent anEvent( MActivateEvent::MActivateEventType::DEACTIVATION );
					MApp::getInstance()->getDisptacher()->processUiEvent( pWnd, &anEvent );
				}
				else if( wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE )
				{
					MActivateEvent anEvent( MActivateEvent::MActivateEventType::ACTIVATION );
					MApp::getInstance()->getDisptacher()->processUiEvent( pWnd, &anEvent );
				}
				return true;
			}
			else
			{
				SetActiveWindow( ((MTopWndWin32Impl*)MApp::getInstance()->getModal()->getImpl())->getHandle() );
				MessageBeep( 0 );
			}
		}
		break;
	case WM_WINDOWPOSCHANGED:
		{
			WINDOWPOS* pWP = (WINDOWPOS*)lParam;
			MRect rect( pWP->x, pWP->y, pWP->cx, pWP->cy );
			MPositionEvent anEvent( rect );
			//MAppWin32Impl::getInstance()->ivDispatcher.processUiEvent( pWnd, &anEvent );

			if( rect.getWidth() != pWnd->getWidth() ||
				rect.getHeight() != pWnd->getHeight() )
			{
				MSizeEvent anEvent( rect.getSize() );
				MApp::getInstance()->getDisptacher()->processUiEvent( pWnd, &anEvent );
			}
			return 0;
		}
		break;
	case WM_COMMAND:
		{
			MCommandEvent anEvent( (unsigned int) (LOWORD( wParam )) );
			MApp::getInstance()->getDisptacher()->processUiEvent( pWnd, &anEvent );
			return 0;
		}
		break;
	case WM_SETFOCUS:
		{
			MFocusEvent anEvent( MFocusEvent::MFocusEventType::FOCUS_GAINED );
			MApp::getInstance()->getDisptacher()->processUiEvent( pWnd, &anEvent );
		}
		break;
	case WM_KILLFOCUS:
		{
			MFocusEvent anEvent( MFocusEvent::MFocusEventType::FOCUS_LOST );
			MApp::getInstance()->getDisptacher()->processUiEvent( pWnd, &anEvent );
		}
		break;
	case WM_GETMINMAXINFO:
		{
			MWin32ResizeRequest anEvent( (MINMAXINFO*)lParam );;
			MApp::getInstance()->getDisptacher()->processUiEvent( pWnd, &anEvent );
		}
		break;
	case WM_DESTROY:
		{
			MDestroyEvent anEvent;
			MApp::getInstance()->getDisptacher()->processUiEvent( pWnd, &anEvent );
		}
		break;
	case WM_CLOSE:
		{
			MCloseEvent anEvent;
			MApp::getInstance()->getDisptacher()->processUiEvent( pWnd, &anEvent );
		}
		break;
	case WM_MOUSELEAVE:
		{
			MHoverEvent anEvent( MHoverEvent::MHoverEventType::HOVER_LOST );
			MApp::getInstance()->getDisptacher()->processUiEvent( pWnd, &anEvent );
		}
		break;
	case WM_MOUSEHOVER:
		{
			MHoverEvent anEvent( MHoverEvent::MHoverEventType::HOVER_GAINED );
			MApp::getInstance()->getDisptacher()->processUiEvent( pWnd, &anEvent );
		}
		break;
	default:
		return DefWindowProc( wnd, message, wParam, lParam );
		break;
	}
	return DefWindowProc( wnd, message, wParam, lParam );
}