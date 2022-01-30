#ifndef __MScrollBarController
#define __MScrollBarController

#include "MScrollBar.h"
#include "IScrollBarListener.h"
#include "MSimpleScrollWnd.h"

typedef std::vector<MWnd*> ScrollWndList;
typedef ScrollWndList::iterator ScrollWndListIter;

class MScrollBarController :
	public MObject,
	public IScrollBarListener
{
protected:
	MScrollBar* ivPtScrollbarHorizontal;
	MScrollBar* ivPtScrollbarVertical;
	MSize ivAdditionalSize;
	ScrollWndList ivScrollWnds;
	ScrollWndList ivHorizontalWnds;
	ScrollWndList ivVerticalWnds;
public:
	FULL_RTTI( MScrollBarController, "MScrollBarController" );
	MScrollBarController();
	MScrollBarController(
		MWnd* ptScrollWnd,
		MScrollBar* ptScrollBarHorz,
		MScrollBar* ptScrollBarVert );
	virtual ~MScrollBarController();
	virtual void valueChanged( void* source );
	void setHorizontalScrollbar( MScrollBar* pScrollBar );
	void setVerticalScrollbar( MScrollBar* pScrollBar );
	MScrollBar* getHorizontalScrollbar();
	MScrollBar* getVerticalScrollbar();
	void setAdditionalSize( const MSize& addSize );
	void setScrollSizes( const MSize& sizeTotal, const MSize& sizePage );
	void addScrollWnd( MWnd* pWnd );
	void removeScrollWnd( MWnd* pWnd );
	void addVerticalScrollWnd( MWnd* pWnd );
	void removeVerticalScrollWnd( MWnd* pWnd );
	void addHorizontalScrollWnd( MWnd* pWnd );
	void removeHorizontalScrollWnd( MWnd* pWnd );
	void setScrollPosition( const MPoint& point );
protected:
	bool hasScrollWnd( MWnd* pWnd );
	bool hasScrollWndHorizontal( MWnd* pWnd );
	bool hasScrollWndVertical( MWnd* pWnd );
	void scrollBoth( const MPoint& point );
	void scrollHorizontal( int x );
	void scrollVertical( int y );
	void scrollBothHorizontal( int x );
	void scrollBothVertical( int y );
};

#endif