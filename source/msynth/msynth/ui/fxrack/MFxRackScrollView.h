/**
 * (C)2000-2003 MRoc
 */

#ifndef __MFxRackScrollView
#define __MFxRackScrollView

#include "MFxSlotWnd.h"
#include "../../model/MFxRack.h"
#include <gui/MWndCollection.h>
#include <gui/scrollbar/MScrollPane.h>

/**
 * implementation of a scroll view
 * embedding some MFxSlotWnds
 */
class MFxRackScrollView :
	public MWndCollection
{
protected:

	/**
	 * the height calculated when doLayout
	 */
	int	ivHeight;

	/**
	 * the width calculated when doLayout
	 */
	int ivWidth;

	/**
	 * the used fx rack
	 */
	MFxRack* ivPtFxRack;

	/**
	 * list of fx slot wnds
	 */
	MFxSlotWnd** ivPtSlotWnds;

public:

	/**
	 * constructor
	 */
	MFxRackScrollView();

	/**
	 * destructor
	 */
	virtual	~MFxRackScrollView();

	/**
	 * sets the used effect rack
	 */
	virtual void setFxRack( MFxRack *ptFxRack );

	/**
	 * returns the used effect rack
	 */
	virtual MFxRack* getFxRack();

	/**
	 * layouts all slots
	 */
	virtual void doLayout();

	/**
	 * returns the size required to display all slots
	 */
	virtual MSize getScrollViewSize();
};


#endif