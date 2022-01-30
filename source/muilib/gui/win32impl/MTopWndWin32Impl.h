#ifndef __MTopWndWin32Impl
#define __MTopWndWin32Impl

#include <framework/MTypes.h>
#include <framework/MNativeTypes.h>
#include "../MTopWndImpl.h"

class MTopWnd;
typedef std::map<HWND, MTopWnd*> Hwnd2OsWndMap;

/**
 * a win32 implementation of a native window. the impl is bound to
 * the MTopWnd because it holds a hack reference.
 */
class MTopWndWin32Impl :
	public MTopWndImpl
{
protected:

	/** runtime type info */
	static MRtti gvRtti;

	/** the instance counter */
	static unsigned int gvInstanceCounter;

	/** the window class */
	static ATOM gvWndClass;

	/** hwnd 2 wnd map */
	static Hwnd2OsWndMap gvMapHwnd2Wnd;

	/** the win32 handle to the window */
	HWND ivHwnd;

	/** the offscreen image */
	HBITMAP ivOffscreen;

	/** the size of offscreen */
	MSize ivOffscreenSize;

	/** a native icon */
	MIcon* ivIcon;

	/** a native menu */
	MMenu* ivMenu;

	/** the owner of thie impl */
	MTopWnd* ivPtOwner;

public:

	/** constructor */
	MTopWndWin32Impl();

	/** destructor */
	virtual ~MTopWndWin32Impl();

	/** creates the native window */
	virtual bool create( NativeWindowType type, MRect rect, MTopWnd* pOwner, MTopWnd *parentWnd );

	/** invoked from owner to mark native impl as destroyed, !!!frees NO resources, it's more a forward for WM_DESTROY !!! */
	virtual void destroy();

	/** invoked from owner to immediately paint the window, forward for WM_PAINT */
	virtual void paint( MWnd* pRootComponent );

	/** makes the window visible */
	virtual void setVisible( bool visible );

	/** makes the window visible, but doesn't make it active */
	virtual void setVisibleNoActivate( bool visible );

	/** returns the visible state */
	virtual bool getVisible() const;

	/** repaints the given rectangle. the rect should be in window coordinates */
	virtual void repaint( const MRect& rect );

	/** sets the app icon */
	virtual void setIcon( MIcon* icon );

	/** returns the app icon */
	virtual MIcon* getIcon() const;

	/** sets the window menu */
	virtual void setMenu( MMenu* menu );

	/** returns the window menu icon */
	virtual MMenu* getMenu() const;

	/** sets the window's title */
	virtual void setText( const String& text );

	/** returns the windows's title */
	virtual String getText() const;

	/** sets the window's rect (x,y,w,h)*/
	virtual void setRect( const MRect &rect );

	/** sets the window's position (x/y) */
	virtual void setPoint( const MPoint& point );

	/** sets the window's size (w/h) */
	virtual void setSize( const MSize& size );

	/** returns the window's rectangle */
	virtual MRect getRect() const;

	/** is the window has a native border, this function returns the window's client size */
	virtual MSize getClientSize() const;

	/** returns the size of the window's frame boundary */
	virtual MSize getFrameBoundary();

	/** converts a point in the window to screen coordinates */
	virtual MPoint convertClient2Screen( const MPoint& point );

	/** returns the native window handle */
	virtual HWND getHandle();

	/** register window class */
	static bool init();

	/** unregisters the window class */
	static bool deinit();

	/** does a lookup in the static map and returns the windows stored under the hwnd */
	static MTopWnd* getWnd( HWND wnd );

	/** centers the window on screen */
	virtual void centerWindow();

	/** return if the native window is already created or not */
	virtual bool isCreated();
};

#endif