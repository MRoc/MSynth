#include "MWndUtils.h"
#include "MWndCollection.h"
#include "MUiGraphics.h"
#include "MTopWnd.h"

int MWndUtils::recdepth = 0;

//#define PERFORMANCE_TRACE

#ifdef PERFORMANCE_TRACE
#include <framework/MUtil.h>
#include <framework/performance/MClock.h>
#endif

/**
 * returns the given window's depth in window tree
 */
int MWndUtils::depth( MWnd* pWnd )
{
	int back = 0;
	MWnd* pParent = 0;
	while( (pParent=pWnd->getParent()) != 0 )
	{
		pWnd = pParent;
		back++;
	}
	return back;
}

/**
 * returns the normalized offset of the component on
 * the top level window.
 */
MPoint MWndUtils::getOffset( MWnd* pWnd )
{
	MPoint back;

	MWnd* pParent = 0;
	while( (pParent=pWnd->getParent()) != 0 )
	{
		back += pWnd->getRect().getPoint() - pParent->getScrollPos();
		pWnd = pParent;
	}

	return back;
}

/**
 * returns the normalized offset of the component on
 * the top level window.
 */
MPoint MWndUtils::getScreenOffset( MWnd* pWnd )
{
	MPoint back;

	while( pWnd && pWnd->getParent() )
	{
		back += pWnd->getRect().getPoint() - pWnd->getParent()->getScrollPos();
		pWnd = pWnd->getParent();
	}

	if( pWnd->getInterface( "MTopWnd" ) )
	{
		MTopWnd* pTopWnd = (MTopWnd*) pWnd->getInterface( "MTopWnd" );
		back = pTopWnd->convertClient2Screen( back );
	}

	return back;
}

/**
 * returns the window's rectangle in top level
 * coordinates. does not imply a visibility.
 */
MRect MWndUtils::getOffsetRect( MWnd* pWnd )
{
	ASSERT( pWnd );
	return MRect( getOffset( pWnd ), pWnd->getRect().getSize() );
}

/**
 * returns the window's rectangle in screen level
 * coordinates. does not imply a visibility.
 */
MRect MWndUtils::getScreenOffsetRect( MWnd* pWnd )
{
	return MRect( getScreenOffset( pWnd ), pWnd->getRect().getSize() );
}

/**
 * returns the clipping rectangle in top
 * level coordinates.
 */
MRect MWndUtils::getClippingRect( MWnd* pWnd )
{
	MWnd* pParent = 0;
	MRect normalizedRect = getOffsetRect( pWnd );
	while( (pParent=pWnd->getParent()) != 0 )
	{
		if( pWnd->getVisible() )
		{
			MRect off = getOffsetRect( pWnd );
			if( off.overlap( normalizedRect ) )
			{
				normalizedRect = normalizedRect * off;
				pWnd = pParent;
			}
			else
				return MRect( 0, 0, 0, 0 );
		}
		else
			return MRect( 0, 0, 0, 0 );
	}
	return normalizedRect;
}

/**
 * checks if the given window is visible on
 * the top frame and not scrolled away
 */
bool MWndUtils::isCurrentlyVisible( MWnd* pWnd )
{
	MRect r = getClippingRect( pWnd );
	return r.getWidth() != 0 && r.getHeight() != 0;
}

/**
 * does a recursive paint of the given window into the
 * given device context.
 *
 * @param pWnd is the window to paint
 * @param g is the clipped and scrolled ui for the given window
 * @param clipRect is the clipping rectangle for the given window
 * @param is the original pG for debug purpose
 */
void MWndUtils::recursivePaint( MWnd* pWnd, IGraphics* pG, const MRect& clipRect, IGraphics* origG  )
{
	recdepth++;

	// paint current window...
	if( pWnd->getVisible() )
	{
#ifdef PERFORMANCE_TRACE
		MClock renderClock, recClock;
		recClock.start();
		renderClock.start();
#endif
		pG->setClippingRect( clipRect );
		pWnd->paint( pG, MRect( MPoint(), pWnd->getRect().getSize() ) );

#ifdef PERFORMANCE_TRACE
		renderClock.stop();
#endif


		MWndCollection* col = 0;
		unsigned int childs = 0;
		// is a container... ?
		if( (col=(MWndCollection*)pWnd->getInterface( "MWndCollection" )) )
		{
			childs = col->getChildCount();
			for( unsigned int i=0;i<childs;i++ )
			{
				// recursive...
				MWnd* pChild = col->getChild( i );
				MRect childTop = getOffsetRect( pChild );
				if( clipRect.overlap( childTop ) )
				{
					// clone graphics...
					MUiGraphics ui( *((MUiGraphics*)pG) );

					// calculate next ui offset...
					ui.setOffset(
						ui.getOffset() +
						pChild->getRect().getPoint() -
						pWnd->getScrollPos() );

					// recursive call...
					recursivePaint( pChild, &ui, childTop * clipRect, origG );
				}
			}
		}
#ifdef PERFORMANCE_TRACE
		recClock.stop();
		renderClock.stop();
		TRACE(
			"%s%s:\tchildS:%u\trendering:%f\tfullrendering:%f msec\n",
			MUtil::getTabs( recdepth ).getData(),
			pWnd->getRtti()->getClassName().getData(),
			childs,
			renderClock.getLastTimeSpan(),
			recClock.getLastTimeSpan() );
#endif
	}

	recdepth--;
}

/**
 * returns the child control of pRoot which is hit by the given point
 */
MWnd* MWndUtils::getHitControl( MWnd* pRoot, const MPoint& p )
{
	MWnd* pBack = pRoot;

	if( pBack->getInterface( "MWndCollection" ) )
	{
		MWndCollection* pContainer = (MWndCollection*) pBack->getInterface( "MWndCollection" );
		int count = (int)pContainer->getChildCount();
		for( int i=0;i<count;i++ )
		{
			MWnd* pChild = pContainer->getChild( i );
			if( pChild->getVisible() )
			{
				if( getOffsetRect( pChild ).contains( p ) )
				{
					pBack = pChild;
					if( pBack->getInterface( "MWndCollection" ) )
					{
						pContainer = (MWndCollection*) pBack->getInterface( "MWndCollection" );
						count = pContainer->getChildCount();
						i = -1;
					}
					else
						break;
				}
			}
		}
	}

	if( ! pBack )
		TRACE( "MTestWindow::getHitControl failed\n" );

	return pBack;
}