#include "MScrollBarController.h"
#include <algorithm>
#include <framework/io/MLogger.h>


INIT_RTTI( MScrollBarController, "MScrollBarController" );

MScrollBarController::MScrollBarController() :
	ivPtScrollbarHorizontal( 0 ),
	ivPtScrollbarVertical( 0 ),
	ivAdditionalSize( MSize( 0, 0 ) )
{
}

MScrollBarController::MScrollBarController(
	MWnd* ptScrollWnd,
	MScrollBar* ptScrollbarHorizontal,
	MScrollBar* ptScrollbarVertical ) :
	ivPtScrollbarHorizontal( 0 ),
	ivPtScrollbarVertical( 0 ),
	ivAdditionalSize( MSize( 0, 0 ) )
{
	ASSERT( ptScrollWnd );
	ASSERT( ptScrollbarHorizontal );
	ASSERT( ptScrollbarVertical );
	addScrollWnd( ptScrollWnd );
	setHorizontalScrollbar( ptScrollbarHorizontal );
	setVerticalScrollbar( ptScrollbarVertical );
}

void MScrollBarController::setHorizontalScrollbar( MScrollBar* pScrollBar )
{
	if( ivPtScrollbarHorizontal )
		ivPtScrollbarHorizontal->getModel()->removeActionListener( this );
	ivPtScrollbarHorizontal = pScrollBar;
	if( ivPtScrollbarHorizontal )
		ivPtScrollbarHorizontal->getModel()->addActionListener( this );
}

void MScrollBarController::setVerticalScrollbar( MScrollBar* pScrollBar )
{
	if( ivPtScrollbarVertical )
		ivPtScrollbarVertical->getModel()->removeActionListener( this );
	ivPtScrollbarVertical = pScrollBar;
	if( ivPtScrollbarVertical )
		ivPtScrollbarVertical->getModel()->addActionListener( this );
}

MScrollBar* MScrollBarController::getHorizontalScrollbar()
{
	return ivPtScrollbarHorizontal;
}

MScrollBar* MScrollBarController::getVerticalScrollbar()
{
	return ivPtScrollbarVertical;
}

MScrollBarController::~MScrollBarController()
{
	ivScrollWnds.clear();
	ivHorizontalWnds.clear();
	ivVerticalWnds.clear();

	if( ivPtScrollbarHorizontal )
		ivPtScrollbarHorizontal->getModel()->removeActionListener( this );
	if( ivPtScrollbarVertical )
		ivPtScrollbarVertical->getModel()->removeActionListener( this );
}

void MScrollBarController::valueChanged( void* source )
{
	MScrollBarModel* ptSource = (MScrollBarModel*) source;
	if( ptSource == ivPtScrollbarHorizontal->getModel() )
	{
		int value = ivPtScrollbarHorizontal->getModel()->getValue();
		scrollBothHorizontal( value );
		scrollHorizontal( value );
	}
	else if( ptSource == ivPtScrollbarVertical->getModel() )
	{
		int value = ivPtScrollbarVertical->getModel()->getValue();
		scrollBothVertical( value );
		scrollVertical( value );
	}
}

void MScrollBarController::setScrollSizes( const MSize&  sizeTotal, const MSize&  sizePage )
{
	if( ivPtScrollbarHorizontal )
		ivPtScrollbarHorizontal->getModel()->setData( 0, (int)sizePage.getWidth() - (int)ivAdditionalSize.getWidth(), (int)sizeTotal.getWidth() );
	else
		MLogger::logError( "MScrollBarController::setScrollSizes failed: no horizontal scrollbar" );

	if( ivPtScrollbarVertical )
		ivPtScrollbarVertical->getModel()->setData( 0, (int)sizePage.getHeight() - (int)ivAdditionalSize.getHeight(), (int)sizeTotal.getHeight() );
	else
		MLogger::logError( "MScrollBarController::setScrollSizes failed: no vertical scrollbar" );
}

void MScrollBarController::setAdditionalSize( const MSize& addSize )
{
	ivAdditionalSize = addSize;
}

void MScrollBarController::addScrollWnd( MWnd* ptWnd )
{
	ASSERT( ptWnd );
	ASSERT( ! hasScrollWnd( ptWnd ) );
	ASSERT( ! hasScrollWndVertical( ptWnd ) );
	ASSERT( ! hasScrollWndHorizontal( ptWnd ) );
	ivScrollWnds.push_back( ptWnd );
}

void MScrollBarController::addVerticalScrollWnd( MWnd* ptWnd )
{
	ASSERT( ptWnd );
	ASSERT( ! hasScrollWnd( ptWnd ) );
	ASSERT( ! hasScrollWndVertical( ptWnd ) );
	ASSERT( ! hasScrollWndHorizontal( ptWnd ) );
	ivVerticalWnds.push_back( ptWnd );
}

void MScrollBarController::addHorizontalScrollWnd( MWnd* ptWnd )
{
	ASSERT( ptWnd );
	ASSERT( ! hasScrollWnd( ptWnd ) );
	ASSERT( ! hasScrollWndVertical( ptWnd ) );
	ASSERT( ! hasScrollWndHorizontal( ptWnd ) );
	ivHorizontalWnds.push_back( ptWnd );
}

void MScrollBarController::setScrollPosition( const MPoint& point )
{
	scrollBoth( point );
	scrollHorizontal( point.getX() );
	scrollVertical( point.getY() );
}

void MScrollBarController::scrollBoth( const MPoint& point )
{
	ScrollWndListIter iter = ivScrollWnds.begin();
	for( ;iter!=ivScrollWnds.end();++iter )
	{
		(*iter)->setScrollPos( point );
		(*iter)->repaint();
	}
}

void MScrollBarController::scrollBothHorizontal( int x )
{
	ScrollWndListIter iter = ivScrollWnds.begin();
	for( ;iter!=ivScrollWnds.end();++iter )
	{
		(*iter)->setScrollPos( MPoint( x, (*iter)->getScrollPos().getY() ) );
		(*iter)->repaint();
	}
}

void MScrollBarController::scrollBothVertical( int y )
{
	ScrollWndListIter iter = ivScrollWnds.begin();
	for( ;iter!=ivScrollWnds.end();++iter )
	{
		(*iter)->setScrollPos( MPoint( (*iter)->getScrollPos().getX(), y ) );
		(*iter)->repaint();
	}
}

void MScrollBarController::scrollHorizontal( int x )
{
	ScrollWndListIter iter = ivHorizontalWnds.begin();
	for( ;iter!=ivHorizontalWnds.end();++iter )
	{
		(*iter)->setScrollPos( MPoint( x, (*iter)->getScrollPos().getY() ) );
		(*iter)->repaint();
	}
}

void MScrollBarController::scrollVertical( int y )
{
	ScrollWndListIter iter = ivVerticalWnds.begin();
	for( ;iter!=ivVerticalWnds.end();++iter )
	{
		(*iter)->setScrollPos( MPoint( (*iter)->getScrollPos().getX(), y ) );
		(*iter)->repaint();
	}
}

bool MScrollBarController::hasScrollWnd( MWnd* pWnd )
{
	return std::find( ivScrollWnds.begin(), ivScrollWnds.end(), pWnd ) != ivScrollWnds.end();
}

bool MScrollBarController::hasScrollWndHorizontal( MWnd* pWnd )
{
	return std::find( ivHorizontalWnds.begin(), ivHorizontalWnds.end(), pWnd ) != ivHorizontalWnds.end();
}

bool MScrollBarController::hasScrollWndVertical( MWnd* pWnd )
{
	return std::find( ivVerticalWnds.begin(), ivVerticalWnds.end(), pWnd ) != ivVerticalWnds.end();
}

void MScrollBarController::removeScrollWnd( MWnd* pWnd )
{
	ScrollWndListIter i = std::find( ivScrollWnds.begin(), ivScrollWnds.end(), pWnd );
	if( i != ivScrollWnds.end() )
		ivScrollWnds.erase( i );
}

void MScrollBarController::removeVerticalScrollWnd( MWnd* pWnd )
{
	ScrollWndListIter i = std::find( ivVerticalWnds.begin(), ivVerticalWnds.end(), pWnd );
	if( i != ivVerticalWnds.end() )
		ivVerticalWnds.erase( i );
}

void MScrollBarController::removeHorizontalScrollWnd( MWnd* pWnd )
{
	ScrollWndListIter i = std::find( ivHorizontalWnds.begin(), ivHorizontalWnds.end(), pWnd );
	if( i != ivHorizontalWnds.end() )
		ivHorizontalWnds.erase( i );
}