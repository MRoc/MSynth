#ifndef __MWndManager
#define __MWndManager

#include "MWnd.h"

/**
 * stores pointers to all existing windows.
 * stores current focus, capture, mouse
 * and top level window.
 *
 * the window manager is a object dispatching a os event to a window.
 *
 */
class MWndManager
{
private:

	/** a set storing all existing windows */
	std::set<MWnd*> ivWnds;

	/** current focus window */
	MWnd* ivPtFocus;

	/** current mouse window */
	MWnd* ivPtMouse;

	/** current mouse capture control */
	MWnd* ivPtCapture;

	/** the current top level window */
	MWnd* ivPtTopLevel;

	/** the singleton instance */
	static MWndManager* gvInstance;

private:

	/** private constructor -> singleton */
	MWndManager();

	/** private destructor -> singleton */
	~MWndManager();

public:

	/** return singleton instance of class */
	static MWndManager* getInstance();

	/** release singleton instance */
	static void release();

	/** registers a instance of a window */
	void regWnd( MWnd* pWnd );

	/** unregisters a instance of a window */
	void unregWnd( MWnd* pWnd );

	/** validates the given pointer points to a existing window */
	bool isWnd( MWnd* pWnd );

	/** sets the current level top window */
	void setTopWnd( MWnd* pTopWnd );

	/** returns the current top level window */
	MWnd* getTopWnd();

	/**
	 * sets the current mouse over window, fires
	 * a mouse leave event to the old one if exists
	 * and a hover event to the new one
	 */
	void setMouse( MWnd* pMouseOver );

	/** returns the current mouse over window */
	MWnd* getMouse();

	/**
	 * sets the current key focus window
	 * fires a focus lost to the old one and focus
	 * gained for new one
	 */
	void setKeyFocus( MWnd* pFocusWnd );

	/** returns the current key focus window */
	MWnd* getKeyFocus();

	/** sets the window that currently captures the mouseevents (sending event)*/
	void setMouseCapture( MWnd* pCaptureWnd );

	/** returns the window that currently captures the mouseevents */
	MWnd* getMouseCapture();
};


#endif