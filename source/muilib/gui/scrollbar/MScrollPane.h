#ifndef __MScrollPane
#define __MScrollPane

#include "MScrollBarController.h"

class MScrollPane :
	public MWndCollection
{
protected:

	MWnd* ivPtScrollWnd;
	MScrollBar* ivPtScrollbarHorizontal;
	MScrollBar* ivPtScrollbarVertical;
	MScrollBarController* ivPtController;

public:

	FULL_RTTI_SUPER( MScrollPane, "MScrollPane", MWndCollection );

	MScrollPane();
	MScrollPane( MWnd* ptScrollWnd );
	virtual ~MScrollPane();

	virtual void paint( IGraphics* pG, const MRect& rect );

	virtual void setScollWnd( MWnd* pWnd );
	virtual MWnd* getScrollWnd();

	virtual void setScrollSize( MSize size );
	virtual void doLayout();

	virtual MScrollBar* getVerticalScrollbar();
	virtual MScrollBar* getHorizontalScrollbar();

	virtual MScrollBarController* getController();

	/** store ui state into treenode */
	virtual MTreeNode* save();

	/** load ui state from treenode */
	virtual void load( MTreeNode* pNode );

	static MObject* createInstance(){ return new MScrollPane(); }
};

#endif