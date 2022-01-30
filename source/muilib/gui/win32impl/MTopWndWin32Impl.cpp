#include "MTopWndWin32Impl.h"

#include <framework/performance/MClock.h>
#include <framework/io/MLogger.h>

#include "../MApp.h"
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
#include "../MGuiUtils.h"
#include "MMenuWin32Impl.h"
#include "MIconWin32Impl.h"
#include "MAppWin32Impl.h"

/**
 * runtime type info
 */
MRtti MTopWndWin32Impl::gvRtti = MRtti( "MTopWndWin32Impl", 0 );

/**
 * the instance counter
 */
unsigned int MTopWndWin32Impl::gvInstanceCounter = 0;

/**
 * the window class
 */
ATOM MTopWndWin32Impl::gvWndClass;

/**
 * hwnd 2 wnd map
 */
Hwnd2OsWndMap MTopWndWin32Impl::gvMapHwnd2Wnd;

/**
 * contructor
 */
MTopWndWin32Impl::MTopWndWin32Impl() :
	ivHwnd( 0 ),
	ivOffscreen( 0 ),
	ivPtOwner( 0 ),
	ivIcon( 0 ),
	ivMenu( 0 )
{
	if( gvInstanceCounter == 0 )
		init();
	gvInstanceCounter++;
}

/**
 * destructor
 */
MTopWndWin32Impl::~MTopWndWin32Impl()
{
	SAFE_DELETE( ivMenu );
	SAFE_DELETE( ivIcon );

	if( ivOffscreen )
	{
		BOOL res = DeleteObject( ivOffscreen );
		ASSERT( res == TRUE );
		ivOffscreen = 0;
	}

	if( ivHwnd )
	{
		if( DestroyWindow( ivHwnd ) == FALSE )
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
				"MTopWndWin32Impl::~MTopWndWin32Impl: DestroyWindow failed:\n%s",
				lpMsgBuf );
			LocalFree( lpMsgBuf );
		}
		ivHwnd = 0;
	}

	gvInstanceCounter--;
	if( gvInstanceCounter == 0 )
		deinit();
}

/**
 * creates the native window
 */
bool MTopWndWin32Impl::create( NativeWindowType type, MRect rect, MTopWnd* pOwner, MTopWnd* parentWnd )
{
	ASSERT( pOwner );
	ASSERT( ((MAppWin32Impl*)MApp::getInstance()->getImpl())->getHandle() );

	ivPtOwner = pOwner;

	if( type == POPUP_WND )
		ivHwnd =
			CreateWindowEx(
				WS_EX_TOPMOST,
				"MTopWndWin32Impl",
				NULL,
				WS_POPUP,
				0,
				0,
				10,
				10,
				0, //parentWnd ? ((MTopWndWin32Impl*)parentWnd->getImpl())->getHandle() : NULL, // HWND PARENT
				0, //ivMenu ? ((MMenuWin32Impl*)ivMenu)->getHandle() : NULL, // HMENU
				((MAppWin32Impl*)MApp::getInstance()->getImpl())->getHandle(),
				NULL );
	else
		ivHwnd =
			CreateWindow(
				"MTopWndWin32Impl",
				"MTopWndWin32Impl",
				type == FRAME_WND ?
					(WS_OVERLAPPEDWINDOW) :
					(WS_POPUP),
				rect.getX(),
				rect.getY(),
				rect.getWidth(),
				rect.getHeight(),
				0, //parentWnd ? ((MTopWndWin32Impl*)parentWnd->getImpl())->getHandle() : NULL, // HWND PARENT
				0, //ivMenu ? ((MMenuWin32Impl*)ivMenu)->getHandle() : NULL, // HMENU
				((MAppWin32Impl*)MApp::getInstance()->getImpl())->getHandle(),
				0 );

	if( ivHwnd != NULL )
	{
		gvMapHwnd2Wnd.insert( Hwnd2OsWndMap::value_type( ivHwnd, pOwner ) );
		if( parentWnd && parentWnd->getIcon() )
			setIcon( new MIcon( *( parentWnd->getIcon() ) ) );
	}
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

	return ivHwnd != NULL;
}

/** invoked from owner to mark native impl as destroyed, !!!frees NO resources, it's more a forward for WM_DESTROY !!! */
void MTopWndWin32Impl::destroy()
{
	if( ivHwnd )
	{
		ASSERT( gvMapHwnd2Wnd.find( ivHwnd ) != gvMapHwnd2Wnd.end() );
		gvMapHwnd2Wnd.erase( gvMapHwnd2Wnd.find( ivHwnd ) );
	}
	else
		ASSERT( false );
	ivHwnd = 0;
}

/** invoked from owner to immediately paint the window, forward for WM_PAINT */
void MTopWndWin32Impl::paint( MWnd* pRootComponent )
{
	ASSERT( ivHwnd );

	MClock clock0;
	MClock clock;
	clock0.start();

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint( ivHwnd, &ps );
	ASSERT( hdc );
	MSize clientSize = getClientSize();
	HDC dcMemory = CreateCompatibleDC( hdc );
	ASSERT( dcMemory );

	if( ivOffscreen == 0 ||
		ivOffscreenSize.getWidth() != clientSize.getWidth() ||
		ivOffscreenSize.getHeight() != clientSize.getHeight() )
	{
		if( ivOffscreen )
		{
			BOOL res = DeleteObject( ivOffscreen );
			ASSERT( res == TRUE );
		}
		ivOffscreen = CreateCompatibleBitmap( hdc, clientSize.getWidth(), clientSize.getHeight() );
		ASSERT( ivOffscreen );
		ivOffscreenSize = MSize( clientSize.getWidth(), clientSize.getHeight() );
	}

	HGDIOBJ oldBitmap = SelectObject( dcMemory, ivOffscreen );

	{
		MRect totalClip =
			MRect(
				ps.rcPaint.left,
				ps.rcPaint.top,
				ps.rcPaint.right - ps.rcPaint.left,
				ps.rcPaint.bottom - ps.rcPaint.top );

		// render control
		MRect r( 0, 0, clientSize.getWidth(), clientSize.getHeight() );
		MUiGraphics g( dcMemory, hdc );

		clock.start();
		MWndUtils::recursivePaint( pRootComponent, &g, totalClip, &g );
		clock.stop();

		BOOL resBitBlt =
			BitBlt(
				hdc,
				ps.rcPaint.left, ps.rcPaint.top, totalClip.getWidth(), totalClip.getHeight(),
				dcMemory,
				ps.rcPaint.left, ps.rcPaint.top,
				SRCCOPY );
		ASSERT( resBitBlt == TRUE );
	}

	SelectObject( dcMemory, oldBitmap );

	BOOL resDeleteDC = DeleteDC( dcMemory );
	ASSERT( resDeleteDC == TRUE );

	EndPaint( ivHwnd, &ps );

	clock0.stop();
	//TRACE( "paint:%f recpaint:%f\n", clock0.getLastTimeSpan() ,clock.getLastTimeSpan() );
}

/**
 * repaints the given rectangle.
 * the rect should be in window coordinates
 */
void MTopWndWin32Impl::repaint( const MRect& rect )
{
	RECT r;
	r.left = rect.getX();
	r.top = rect.getY();
	r.bottom = rect.getBottom();
	r.right = rect.getRight();
	if(
		InvalidateRect(
			ivHwnd,
			&r,
			false ) == FALSE )
	{
		MLogger::logError(
			"MTopWndWin32Impl::repaint: InvalidateRect failed" );
	}
}

/**
 * register window class
 */
bool MTopWndWin32Impl::init()
{
	MLogger::logMessage( "<mtopwnd::init/>\n" );

	ASSERT( ((MAppWin32Impl*)MApp::getInstance()->getImpl())->getHandle() );
	ASSERT( LoadCursor( NULL, IDC_ARROW ) );

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.lpfnWndProc = (WNDPROC) MAppWin32Impl::staticWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = ((MAppWin32Impl*)MApp::getInstance()->getImpl())->getHandle();
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = NULL;
	wc.lpszMenuName = 0;
	wc.lpszClassName = "MTopWndWin32Impl";

	if( wc.hCursor == 0 )
		MLogger::logError( "MTopWndWin32Impl::init: LoadCursor failed" );

	gvWndClass = RegisterClass( &wc );
	if( gvWndClass == 0 )
		MLogger::logError( "MTopWndWin32Impl::init: RegisterClass failed" );

	return gvWndClass ? true : false;
}

/**
 * unregisters the window class
 */
bool MTopWndWin32Impl::deinit()
{
	MLogger::logMessage( "<mtopwnd::deinit/>\n" );
	if( UnregisterClass( "MTopWndWin32Impl", ((MAppWin32Impl*)MApp::getInstance()->getImpl())->getHandle() ) == FALSE )
	{
		MLogger::logError( "MTopWndWin32Impl::deinit: UnregisterClass failed" );
		return false;
	}
	else
		return true;
}

/** 
 * does a lookup in the static hwnd2CITWin32Window
 * and returns the windows stored under the hwnd
 */
MTopWnd* MTopWndWin32Impl::getWnd( HWND hWnd )
{
	Hwnd2OsWndMap::iterator i = gvMapHwnd2Wnd.find( hWnd );
	if( i != gvMapHwnd2Wnd.end() )
		return i->second;
	else
		return 0;
}

HWND MTopWndWin32Impl::getHandle()
{
	return ivHwnd;
}

void MTopWndWin32Impl::centerWindow()
{
	ASSERT( ivHwnd );
	RECT wRect;
	GetWindowRect( ivHwnd, &wRect );
	int w = wRect.right - wRect.left;
	int h = wRect.bottom - wRect.top;
	int sW = GetSystemMetrics( SM_CXFULLSCREEN );
	int sH = GetSystemMetrics( SM_CYFULLSCREEN );
	int x = (sW-w)/2;
	int y = (sH-h)/2;
	setRect( MRect( x, y, w, h ) );
}

void MTopWndWin32Impl::setVisible( bool visible )
{
	if( ivHwnd )
	{
		ShowWindow( ivHwnd, visible ? SW_SHOWNORMAL : SW_HIDE );
		if( visible && UpdateWindow( ivHwnd ) == FALSE )
			MLogger::logError( "MTopWndWin32Impl::setVisible: UpdateWindow failed" );
	}
}

/** makes the window visible, but doesn't make it active */
void MTopWndWin32Impl::setVisibleNoActivate( bool visible )
{
	ASSERT( ivHwnd );

	ShowWindow( ivHwnd, visible ? SW_SHOWNOACTIVATE : SW_HIDE );
	if( visible && UpdateWindow( ivHwnd ) == FALSE )
		MLogger::logError( "MTopWndWin32Impl::setVisible: UpdateWindow failed" );
}

/** returns the visible state */
bool MTopWndWin32Impl::getVisible() const
{
	if( ivHwnd )
		return IsWindowVisible( ivHwnd ) == TRUE;
	else
		return false;
}

MIcon* MTopWndWin32Impl::getIcon() const
{
	return ivIcon;
}

void MTopWndWin32Impl::setIcon( MIcon* icon )
{
	ivIcon = icon;
	if( ivHwnd && icon )
	{
		SendMessage(
			ivHwnd,              // handle to destination window 
			WM_SETICON,          // message to send
			(WPARAM) ICON_SMALL,          // icon type
			(LPARAM) ((MIconWin32Impl*)icon->getImpl())->getIcon() // handle to icon (HICON)
		);
		SendMessage(
			ivHwnd,              // handle to destination window 
			WM_SETICON,          // message to send
			(WPARAM) ICON_BIG,          // icon type
			(LPARAM) ((MIconWin32Impl*)icon->getImpl())->getIcon() // handle to icon (HICON)
		);
	}
}

/** returns the window menu icon */
MMenu* MTopWndWin32Impl::getMenu() const
{
	return ivMenu;
}

/** sets the window menu */
void MTopWndWin32Impl::setMenu( MMenu* menu )
{
	ivMenu = menu;
	if( ivHwnd )
		SetMenu( ivHwnd, ((MMenuWin32Impl*)ivMenu->getImpl())->getHandle() );
}

/** sets the window's title */
void MTopWndWin32Impl::setText( const String& text )
{
	ASSERT( ivHwnd );

	SetWindowText( ivHwnd, text );
}

/** returns the windows's title */
String MTopWndWin32Impl::getText() const
{
	ASSERT( ivHwnd );

	char buffer[512];
	GetWindowText( ivHwnd, buffer, 512 );
	return String( buffer );
}

MSize MTopWndWin32Impl::getFrameBoundary()
{
	ASSERT( ivHwnd );

	MSize back( 0, 0 );
	
	LONG dwStyle = GetWindowLongPtr( ivHwnd, GWL_STYLE );
	LONG dwExStyle = GetWindowLongPtr( ivHwnd, GWL_EXSTYLE );
	/*DWORD dwStyle = GetStyle();
	DWORD dwExStyle = GetExStyle();*/
	if (!(dwStyle & WS_CHILD))
	{
		if (dwStyle & WS_CAPTION)
			back.addY( GetSystemMetrics(SM_CYCAPTION) );
		if (::GetMenu( ivHwnd ))
			back.addY( GetSystemMetrics(SM_CYMENU) );
	}
	if (dwStyle & WS_THICKFRAME)
	{
		back.addX( 2*GetSystemMetrics(SM_CXSIZEFRAME) );
		back.addY( 2*GetSystemMetrics(SM_CYSIZEFRAME) );
	}
	else if (dwStyle & WS_BORDER)
	{
		back.addX( 2*GetSystemMetrics(SM_CXBORDER) );
		back.addY( 2*GetSystemMetrics(SM_CYBORDER) );
	}
	if (dwExStyle & WS_EX_CLIENTEDGE)
	{
		back.addX( 2*GetSystemMetrics(SM_CXEDGE) );
		back.addY( 2*GetSystemMetrics(SM_CYEDGE) );
	}

	return back;
}

void MTopWndWin32Impl::setRect( const MRect &rect )
{
	if( ivHwnd )
		MoveWindow( ivHwnd, rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight(), true );
}

void MTopWndWin32Impl::setPoint( const MPoint& point )
{
	ASSERT( ivHwnd );

	RECT r;
	if( GetWindowRect( ivHwnd, &r ) == FALSE )
		MLogger::logWarning( "MTopWndWin32Impl::setPoint: GetWindowRect failed" );
	else 
	{
		MoveWindow(
			ivHwnd,
			point.getX(),
			point.getY(),
			r.right - r.left,
			r.bottom - r.top,
			true );
	}
}

void MTopWndWin32Impl::setSize( const MSize& size )
{
	ASSERT( ivHwnd );

	MRect r = getRect();
	MoveWindow( ivHwnd, r.getX(), r.getY(), size.getWidth(), size.getHeight(), true );
}

/** returns the window's rectangle */
MSize MTopWndWin32Impl::getClientSize() const
{
	ASSERT( ivHwnd );

	RECT r;
	GetClientRect( ivHwnd, &r );
	return MSize( r.right - r.left, r.bottom - r.top );
}

MRect MTopWndWin32Impl::getRect() const
{
	ASSERT( ivHwnd );

	RECT r;
	GetWindowRect( ivHwnd, &r );
	return MRect( r.left, r.top, r.right-r.left, r.bottom-r.top );
}

MPoint MTopWndWin32Impl::convertClient2Screen( const MPoint& point )
{
	ASSERT( ivHwnd );

	POINT p;
	p.x = point.getX();
	p.y = point.getY();
	ClientToScreen( ivHwnd, &p );

	return MPoint( p.x, p.y );
}

/** return if the native window is already created or not */
bool MTopWndWin32Impl::isCreated()
{
	return ivHwnd != 0;
}