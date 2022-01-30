#ifndef __MRootPane
#define __MRootPane

#include "../MWndCollection.h"

/**
 * the root pane is the root wnd collection found
 * in a framewindow. this is required to offset
 * and shrink the pane because of the native borders
 * of the frame wnd.
 */
class MRootPane :
	public MWndCollection
{
public:

	/** the runtime type info */
	static MRtti gvRtti;

public:

	/** constructor */
	MRootPane();

	/** destructor */
	virtual ~MRootPane();

	/** returns no window, because this class is a fake toplevel window */
	virtual MWnd* getParent();

	/** returns the real parent of this window, the frame wnd */
	virtual MWnd* getRealParent();

	/** paints the parent window */
	virtual void paint( IGraphics* pG, const MRect& rect );

	/** repaints the given rectangle. the rect should be in window coordinates */
	virtual void repaint( const MRect& rect );

	/** repaints the whole window */
	virtual void repaint();

	/** returns the runtime type info */
	virtual IRtti* getRtti();
};

#endif