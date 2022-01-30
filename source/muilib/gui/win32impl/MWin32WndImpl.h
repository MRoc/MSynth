#ifndef __MWin32WndImpl
#define __MWin32WndImpl

/**
 * a windows 32 window implementation
 */
class MWin32WndImpl
{
public:

	/**
	 * the window class
	 */
	static ATOM gvWndClass;

	/**
	 * hwnd 2 wnd map
	 */
	static Hwnd2OsWndMap gvMapHwnd2Wnd;

	/**
	 * the win32 handle to the window
	 */
	HWND ivHwnd;

	/**
	 * the offscreen image
	 */
	HBITMAP ivOffscreen;

	/** 
	 * the size of offscreen
	 */
	MSize ivOffscreenSize;

protected:

};


#endif