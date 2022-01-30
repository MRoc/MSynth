/**
 * (C)2001-2003 MRoc
 */

#include "MFxRackFrame.h"
#include <gui/framewnd/MRootPane.h>

/**
 * constructor
 */
MFxRackFrame::MFxRackFrame() :
	MFrameWnd(),
	ivPtFxRack( 0 ),
	ivPtScrollView( 0 ),
	ivPtScrollPane( 0 )
{
	setActionOnClose( MFrameWnd::HIDE );

	ivPtScrollView = new MFxRackScrollView();
	ivPtScrollPane = new MScrollPane( ivPtScrollView );
}

/**
 * destructor
 */
MFxRackFrame::~MFxRackFrame()
{
	SAFE_DELETE( ivPtScrollPane );
	SAFE_DELETE( ivPtScrollView );
}

/**
 * creates the frame
 */
bool MFxRackFrame::create( MRect rect, MTopWnd* parentWnd )
{
	ASSERT( ivPtFxRack );
	bool back = MFrameWnd::create( rect, parentWnd );
	if( back )
	{
		setText( "Fx Rack" );
		ivPtScrollView->setFxRack( ivPtFxRack );
		addChild( ivPtScrollPane );
		ivPtScrollView->doLayout();
		autoSize();
	}
	return back;
}

/**
 * sets the used effect rack
 */
void MFxRackFrame::setFxRack( MFxRack* ptFxRack )
{
	ivPtFxRack = ptFxRack;
}

/**
 * returns the used effect rack
 */
MFxRack* MFxRackFrame::getFxRack()
{
	return ivPtFxRack;
}

/**
 * layouts all components
 */
void MFxRackFrame::doLayout()
{
	MTopWnd::doLayout();
	if( ivPtScrollPane )
		ivPtScrollPane->setScrollSize( ivPtScrollView->getScrollViewSize() );
}

/**
 * resizes the frame to fit the embedded scrollview
 */
void MFxRackFrame::autoSize()
{
	MSize prefSize = ivPtScrollView->getScrollViewSize();
	setMaximumSize( MSize( prefSize.getWidth()+16, prefSize.getHeight()+16 ) );
	setPreferredSize( MSize( prefSize.getWidth(), prefSize.getHeight() ) );
	ivPtRootPane->repaint();
}

