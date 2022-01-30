/**
 * (C)2000-2003 MRoc
 *
 * v.0.1
 * v.0.2 rewritten
 */
#include "MFxRackScrollView.h"

/**
 * constructor
 */
MFxRackScrollView::MFxRackScrollView() :
	MWndCollection(),
	ivPtFxRack( 0 ),
	ivWidth( 0 ),
	ivHeight( 0 ),
	ivPtSlotWnds( 0 )
{
	ivBkColor = MColorMap::BK_COLOR1;
}

/**
 * destructor
 */
MFxRackScrollView::~MFxRackScrollView()
{
	if( ivPtSlotWnds )
	{
		unsigned int slotCount = ivPtFxRack->getSlotCount();
		for( unsigned int i=0;i<slotCount;i++ )
		{
			removeChild( ivPtSlotWnds[ i ] );
			delete ivPtSlotWnds[ i ];
		}
		delete ivPtSlotWnds;
	}
}

/**
 * sets the used effect rack
 */
void MFxRackScrollView::setFxRack( MFxRack* ptFxRack )
{
	ivPtFxRack = ptFxRack;

	unsigned int slotCount = ivPtFxRack->getSlotCount();
	ivPtSlotWnds = new MFxSlotWnd*[ slotCount ];
	for( unsigned int i=0;i<slotCount;i++ )
	{
		ivPtSlotWnds[ i ] = new MFxSlotWnd( i + 1, ivPtFxRack->getSlot( i ) );
		addChild( ivPtSlotWnds[ i ] );
	}
	doLayout();
}

/**
 * returns the used effect rack
 */
MFxRack* MFxRackScrollView::getFxRack()
{
	return ivPtFxRack;
}

/**
 * layouts all slots
 */
void MFxRackScrollView::doLayout()
{
	if( ivPtSlotWnds )
	{
		ASSERT( ivPtFxRack );
		unsigned int i;

		unsigned int slotCount = ivPtFxRack->getSlotCount();

		// calculate maximal slot width
		unsigned int maxSlotWidth = 0;
		for( i=0;i<slotCount;i++ )
		{
			ASSERT( ivPtSlotWnds[ i ] );
			unsigned int sw = ivPtSlotWnds[ i ]->getPreferredSize().getWidth();
			if( sw > maxSlotWidth )
				maxSlotWidth = sw;
		}

		MSize size = getSize();

		int width = maxSlotWidth;
		int x = 2;
		int y = 2;
		for( i=0;i<slotCount;i++ )
		{
			MFxSlotWnd* ptSlot = ivPtSlotWnds[ i ];
			MSize slotSize = ptSlot->getSize();
			ptSlot->setRect( MRect( x, y, width, slotSize.getHeight() ) );
			y += slotSize.getHeight() + 2;
		}

		this->ivWidth = maxSlotWidth + 4;
		if( this->ivWidth < 250 )
			this->ivWidth = 250;
		this->ivHeight = y;
	}
    
	this->repaint();
}

/**
 * returns the size required to display all slots,
 * (calculated when doLayout is invoked!!!)
 */
MSize MFxRackScrollView::getScrollViewSize()
{
	return MSize( ivWidth, ivHeight );
}