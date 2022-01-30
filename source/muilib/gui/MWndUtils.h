#ifndef __MWndUtils
#define __MWndUtils

#include "MWnd.h"

class MWndUtils
{
private:

	static int recdepth;

	MWndUtils(){};
	virtual ~MWndUtils(){};

public:

	/**
	 * returns the given window's depth in window tree
	 */
	static int depth( MWnd* pWnd );

	/**
	 * returns the normalized offset of the component on
	 * the top level window.
	 */
	static MPoint getOffset( MWnd* pWnd );

	/**
	 * returns the normalized offset of the component on
	 * the screen level.
	 */
	static MPoint getScreenOffset( MWnd* pWnd );

	/**
	 * returns the window's rectangle in top level
	 * coordinates. does not imply a visibility.
	 */
	static MRect getOffsetRect( MWnd* pWnd );

	/**
	 * returns the window's rectangle in screen level
	 * coordinates. does not imply a visibility.
	 */
	static MRect getScreenOffsetRect( MWnd* pWnd );

	/**
	 * returns the clipping rectangle in top
	 * level coordinates.
	 */
	static MRect getClippingRect( MWnd* pWnd );

	/**
	 * checks if the given window is visible on
	 * the top frame and not scrolled away
	 */
	static bool isCurrentlyVisible( MWnd* pWnd ); 

	/**
	 * does a recursive paint of the given window into the
	 * given device context.
	 *
	 * @param pWnd is the window to paint
	 * @param g is the clipped and scrolled ui for the given window
	 * @param clipRect is the clipping rectangle for the given window
	 * @param is the original pG for debug purpose
	 */
	static void recursivePaint( MWnd* pWnd, IGraphics* g, const MRect& clipRect, IGraphics* origG );

	/**
	 * returns the child control of pRoot which is hit by the given point
	 */
	static MWnd* getHitControl( MWnd* pRoot, const MPoint& p );

public:
};

#endif