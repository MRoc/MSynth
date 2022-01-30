#ifndef __MScrolledListView
#define __MScrolledListView

#include "../scrollbar/MScrollPane.h"
#include "MListView.h"
#include "MScrollPaneListViewController.h"

/**
 * a scrollpane with embedded listview
 */
class MScrolledListView :
	public MScrollPane
{
protected:

	/**
	 * controller updating scrollpane when
	 * object list changed.
	 */
	MScrollPaneListViewController* ivPtLVController;

public:

	FULL_RTTI_SUPER( MScrolledListView, "MScrolledListView", MScrollPane );

	/** constructor */
	MScrolledListView();

	/** constructor */
	MScrolledListView( MSelectableObjectList* pModel );

	/** destructor */
	virtual ~MScrolledListView();

	/** sets the list view model */
	virtual void setModel( MSelectableObjectList* pModel );

	/** returns the list view model */
	virtual MSelectableObjectList* getModel();

	/** sets the scrol window (the list */
	virtual void setScollWnd( MWnd* pWnd );

	/** store ui state into treenode */
	virtual MTreeNode* save();

	/** load ui state from treenode */
	virtual void load( MTreeNode* pNode );

	static MObject* createInstance(){ return new MScrolledListView(); }
};

#endif