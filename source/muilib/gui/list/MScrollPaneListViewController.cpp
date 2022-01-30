#include "MScrollPaneListViewController.h"

INIT_RTTI( MScrollPaneListViewController, "MScrollPaneListViewController" );

/** constructor */
MScrollPaneListViewController::MScrollPaneListViewController(
	MListView* pListView,
	MScrollPane* pScrollPane )
	:
	ivPtListView( 0 ),
	ivPtScrollPane( 0 )
{
	setScrollPane( pScrollPane );
	setListView( pListView );
}

/** constructor */
MScrollPaneListViewController::MScrollPaneListViewController() :
	ivPtListView( 0 ),
	ivPtScrollPane( 0 )
{
}

/** set the scrollpane */
void MScrollPaneListViewController::setScrollPane( MScrollPane* pPane )
{
	ivPtScrollPane = pPane;
}

/** returns the scrollpane */
MScrollPane* MScrollPaneListViewController::getScrollPane()
{
	return ivPtScrollPane;
}

/** sets the listview */
void MScrollPaneListViewController::setListView( MListView* pView )
{
	if( ivPtListView )
	{
		ivPtListView->getModel()->removeObjectListListener( this );
		ivPtListView->removeListViewListener( this );
	}

	ivPtListView = pView;

	if( ivPtListView )
	{
		ivPtListView->getModel()->addObjectListListener( this );
		ivPtListView->addListViewListener( this );
	}
}

/** returns the listview */
MListView* MScrollPaneListViewController::getListView()
{
	return ivPtListView;
}

/** destructor */
MScrollPaneListViewController::~MScrollPaneListViewController()
{
	ivPtListView->getModel()->removeObjectListListener( this );
	ivPtListView->removeListViewListener( this );
}

/**
 * invoked when a object was added to list
 * (inheritanced from IObjectListListener)
 */
void MScrollPaneListViewController::objectAdded( MObject* obj )
{
	updateScrollPane();
}

/**
 * invoked when a object was removed from list
 * (inheritanced from IObjectListListener)
 */
void MScrollPaneListViewController::objectRemoved( MObject* obj )
{
	updateScrollPane();
}

/**
 * invoked when all objects were removed from list
 * (inheritanced from IObjectListListener)
 */
void MScrollPaneListViewController::allObjectsRemoved()
{
	updateScrollPane();
}

/**
 * invoked when selection chagned
 * (inheritanced from IListViewListener)
 */
void MScrollPaneListViewController::onSelection()
{
	updateScrollPosFromSelection();
}

/**
 * invoked when pre selection
 * (by mouse over) changed
 * (inheritanced from IListViewListener)
 */
void MScrollPaneListViewController::onPreSelection()
{
	updateScrollPosFromSelection();
}

/**
 * updates the scrollpane from the list view model
 */
void MScrollPaneListViewController::updateScrollPane()
{
	ivPtScrollPane->setScrollSize( ivPtListView->getPreferredSize() );
}

/** 
 * updates the scroll position when a selection was made
 * by keyboard.
 */
void MScrollPaneListViewController::updateScrollPosFromSelection()
{
	MScrollBarModel* pScrollModel = ivPtScrollPane->getVerticalScrollbar()->getModel();
	int top = ivPtListView->getPreSelection() * MListView::CY_SIZE;
	int bottom = top + MListView::CY_SIZE;
	int scrollPos = pScrollModel->getValue();
	if( top < scrollPos )
		pScrollModel->setValue( top );
	else if( bottom > scrollPos + pScrollModel->getInner() )
		pScrollModel->setValue(	bottom - pScrollModel->getInner() );
}