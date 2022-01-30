#ifndef __MTopWndImpl
#define __MTopWndImpl

class MTopWnd;
#include <framework/primitive/MRect.h>
#include "MWnd.h"
#include "framewnd/MMenu.h"
#include "framewnd/MIcon.h"

/** abstract baseclass of native top window implementation */
class MTopWndImpl
{
public:

	/** the types of native windows a subclass MUST implement */
	enum NativeWindowType
	{
		SIMPLE_WND,
		POPUP_WND,
		FRAME_WND
	};

public:

	/** constructor */
	MTopWndImpl(){}

	/** destructor */
	virtual ~MTopWndImpl(){}

	/** creates the native window */
	virtual bool create( NativeWindowType type, MRect rect, MTopWnd* pOwner, MTopWnd *parentWnd ) = 0;

	/** return if the native window is already created or not */
	virtual bool isCreated() = 0;

	/** invoked from owner to mark native impl as destroyed, !!!frees NO resources, it's more a forward for WM_DESTROY !!! */
	virtual void destroy() = 0;

	/** invoked from owner to immediately paint the window, forward for WM_PAINT */
	virtual void paint( MWnd* pRootComponent ) = 0;

	/** makes the window visible */
	virtual void setVisible( bool visible ) = 0;

	/** makes the window visible, but doesn't make it active */
	virtual void setVisibleNoActivate( bool visible ) = 0;

	/** returns the visible state */
	virtual bool getVisible() const = 0;

	/** repaints the given rectangle. the rect should be in window coordinates */
	virtual void repaint( const MRect& rect ) = 0;

	/** sets the app icon */
	virtual void setIcon( MIcon* icon ) = 0;

	/** returns the app icon */
	virtual MIcon* getIcon() const = 0;

	/** sets the window menu */
	virtual void setMenu( MMenu* menu ) = 0;

	/** returns the window menu icon */
	virtual MMenu* getMenu() const = 0;

	/** sets the window's title */
	virtual void setText( const String& text ) = 0;

	/** returns the windows's title */
	virtual String getText() const = 0;

	/** sets the window's rect (x,y,w,h)*/
	virtual void setRect( const MRect &rect ) = 0;

	/** sets the window's position (x/y) */
	virtual void setPoint( const MPoint& point ) = 0;

	/** sets the window's size (w/h) */
	virtual void setSize( const MSize& size ) = 0;

	/** returns the window's rectangle */
	virtual MRect getRect() const = 0;

	/** is the window has a native border, this function returns the window's client area */
	virtual MSize getClientSize() const = 0;

	/** returns the size of the window's frame boundary */
	virtual MSize getFrameBoundary() = 0;

	/** converts a point in the window to screen coordinates */
	virtual MPoint convertClient2Screen( const MPoint& point ) = 0;

	/** centers the window on screen */
	virtual void centerWindow() = 0;
};

#endif