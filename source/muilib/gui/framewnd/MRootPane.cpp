#include "MRootPane.h"

/** the runtime type info */
MRtti MRootPane::gvRtti = MRtti( "MRootPane", 0 );

/** constructor */
MRootPane::MRootPane()
{
	ivPtNamespace = new MWndNamespace( this );
}

/** destructor */
MRootPane::~MRootPane()
{
}

/** returns no window, because this class is a fake toplevel window */
MWnd* MRootPane::getParent()
{
	return 0;
}

/** returns the real parent of this window, the frame wnd */
MWnd* MRootPane::getRealParent()
{
	return ivPtParent;
}

/** repaints the given rectangle. the rect should be in window coordinates */
void MRootPane::repaint( const MRect& rect )
{
	getRealParent()->repaint( rect );
}

/** repaints the whole window */
void MRootPane::repaint()
{
	MWnd::repaint();
}

/** returns the runtime type info */
IRtti* MRootPane::getRtti()
{
	return &gvRtti;
}

/** paints the parent window */
void MRootPane::paint( IGraphics* pG, const MRect& rect )
{
	pG->fillRect( rect, getRealParent()->getBkColor() );
}