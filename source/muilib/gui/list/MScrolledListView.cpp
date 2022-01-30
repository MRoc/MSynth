#include "MScrolledListView.h"

INIT_RTTI_CREATE( MScrolledListView, "MScrolledListView" );

/** constructor */
MScrolledListView::MScrolledListView() :
	MScrollPane( new MListView() ),
	ivPtLVController( 0 )
{
	ivPtLVController = new MScrollPaneListViewController();
	ivPtLVController->setListView( (MListView*) ivPtScrollWnd );
	ivPtLVController->setScrollPane( this );
	((MListView*)ivPtScrollWnd)->setPreSelectionEnabled( false );
}

/** constructor */
MScrolledListView::MScrolledListView( MSelectableObjectList* pModel ) :
	MScrollPane( new MListView( pModel ) ),
	ivPtLVController( 0 )
{
	ivPtLVController = new MScrollPaneListViewController();
	ivPtLVController->setListView( (MListView*) ivPtScrollWnd );
	ivPtLVController->setScrollPane( this );
	((MListView*)ivPtScrollWnd)->setPreSelectionEnabled( false );
}

/** destructor */
MScrolledListView::~MScrolledListView()
{
	SAFE_DELETE( ivPtLVController );
}

/**
 * sets the list view model
 */
void MScrolledListView::setModel( MSelectableObjectList* pModel )
{
	if( ivPtLVController )
	{
		delete ivPtLVController;
		ivPtLVController = 0;
	}

	((MListView*)ivPtScrollWnd)->setModel( pModel );

	if( pModel )
		ivPtLVController = new MScrollPaneListViewController( ((MListView*)ivPtScrollWnd), this );
}

/**
 * returns the list view model
 */
MSelectableObjectList* MScrolledListView::getModel()
{
	return ((MListView*)ivPtScrollWnd)->getModel();
}

/** sets the scrol window (the list */
void MScrolledListView::setScollWnd( MWnd* pWnd )
{
	MScrollPane::setScollWnd( pWnd );
}

/** store ui state into treenode */
MTreeNode* MScrolledListView::save()
{
	MTreeNode* pBack = MWnd::save();
	return pBack;
}

/** load ui state from treenode */
void MScrolledListView::load( MTreeNode* pNode )
{
	MWnd::load( pNode );
	doLayout();
}