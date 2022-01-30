#ifndef __MScrollPaneListViewController
#define __MScrollPaneListViewController

#include "MListView.h"
#include "../scrollbar/MScrollPane.h"

/**
 * object list listener for updating the
 * scrollpane when object list changed.
 */
class MScrollPaneListViewController :
	public MObject,
	public IObjectListListener,
	public IListViewListener
{
protected:

	/** the observed object list */
	MListView* ivPtListView;

	/** the scroll pane */
	MScrollPane* ivPtScrollPane;

public:

	FULL_RTTI( MScrollPaneListViewController, "MScrollPaneListViewController" );

	/** constructor */
	MScrollPaneListViewController();

	/** constructor */
	MScrollPaneListViewController( MListView* pListView, MScrollPane* pScrollPane );

	/** destructor */
	virtual ~MScrollPaneListViewController();

	/** set the scrollpane */
	virtual void setScrollPane( MScrollPane* pPane );

	/** returns the scrollpane */
	virtual MScrollPane* getScrollPane();

	/** sets the listview */
	virtual void setListView( MListView* pView );

	/** returns the listview */
	virtual MListView* getListView();

	/** invoked when a object was added to list (inheritanced from IObjectListListener) */
	virtual void objectAdded( MObject* obj );

	/** invoked when a object was removed from list (inheritanced from IObjectListListener) */
	virtual void objectRemoved( MObject* obj );

	/** invoked when all objects were removed from list (inheritanced from IObjectListListener) */
	virtual void allObjectsRemoved();

	/** invoked when selection chagned (inheritanced from IListViewListener) */
	virtual void onSelection();

	/** invoked when pre selection (by mouse over) changed (inheritanced from IListViewListener) */
	virtual void onPreSelection();

protected:

	/** updates the scrollpane from the list view model */
	void updateScrollPane();

	/** updates the scroll position when a selection was made by keyboard. */
	void updateScrollPosFromSelection();
};

#endif