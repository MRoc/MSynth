/**
 * (C)2001-2003 MRoc
 */

#ifndef __MFxRackFrame
#define __MFxRackFrame

#include "MFxRackScrollView.h"
#include "../../model/MFxRack.h"
#include <gui/scrollbar/MScrollPane.h>
#include <gui/framewnd/MFrameWnd.h>

/**
 * implementation of a frame for the effect rack
 */
class MFxRackFrame : public MFrameWnd
{
protected:

	/**
	 * the effect rack
	 */
	MFxRack* ivPtFxRack;

	/**
	 * the scroll view holding the MFxSlotWnd
	 */
	MFxRackScrollView* ivPtScrollView;

	/**
	 * the scroll pane embedding the scrollview
	 */
	MScrollPane* ivPtScrollPane;

public:

	/**
	 * constructor
	 */
	MFxRackFrame();

	/**
	 * destructor
	 */
	virtual ~MFxRackFrame();

	/** 
	 * creates the frame
	 */
	virtual bool create( MRect rect, MTopWnd* parentWnd );

	/**
	 * sets the used effect rack
	 */
	virtual void setFxRack( MFxRack* ptFxRack );

	/**
	 * returns the used effect rack
	 */
	virtual MFxRack* getFxRack();

	/**
	 * layouts all components
	 */
	virtual void doLayout();

	/**
	 * resizes the frame to fit the embedded scrollview
	 */
	virtual void autoSize();
};

#endif