#ifndef __MABSTRACTNOTEFRAME
#define __MABSTRACTNOTEFRAME

#include <gui/framewnd/MFrameWnd.h>
#include <gui/scrollbar/MScrollBarController.h>
#include <gui/button/MToggleButton.h>
#include <gui/button/MMultiToggleButtonControl.h>

#include "MNoteLocationBar.h"
#include "MNoteInspector.h"
#include "MInspectorCombo.h"
#include "../dcgrid/MDCGrid.h"
#include "../../model/MBar.h"
#include "../../model/interfaces/MInstrument.h"
#include "../clipboard/MClipBoard.h"

/**
 * MAbstractNoteFrame (C) 2001-2003 MRoc
 * A abstract editor frame for a MBar
 * with some base-implementations.
 */
class MAbstractNoteFrame :
	public MFrameWnd,
	public IDCGridListener,
	public INoteInspectorListener,
	public IClipBoardContext
{
protected:

	class MButtonListener;
	class MBarListener;
	class MInstrumentListener;
	class MControlListener;
	
	/** the bpm control */
	IBpmControl* ivPtBpmControl;

	/** the bar that should be edited */
	MBar* ivPtBar;

	/** the depending instrument */
	MInstrument* ivPtInstrument;

	/** the bar listener to handle incoming events from the bar */
	MBarListener* ivPtBarListener;

	/** the instrument listener to handle incoming events from the instrument */
	MInstrumentListener* ivPtInstrumentListener;

	/** to handle instrument name changes */
	MControlListener* ivPtControlListener;

	/** the partsize of a item in the noteframe (subclass dependent) */
	MSize* ivPtPartSize;

	/** the grid where to edit the notes */
	MDCGrid* ivPtGrid;

	/** the location bar upper */
	MNoteLocationBar* ivPtLocationBar;

	/** the inspector down under */
	MNoteInspector* ivPtInspector;

	/** the inspector combo for the inspector */
	MInspectorCombo* ivPtInspectorCombo;

	/** the horizontal scrollbar every noteframe has */
	MScrollBar* ivPtScrollbarHorizontal;

	/** the vertical scrollbar every noteframe has */
	MScrollBar* ivPtScrollbarVertical;

	/** the scrollbar controller controlling the scrollbars */
	MScrollBarController* ivPtScrollbarController;

	/** the select button */
	MToggleButton* ivPtButtonSelect;

	/** the draw button */
	MToggleButton* ivPtButtonDraw;

	/** the toolbar button control */
	MMultiToggleButtonControl ivButtonControl;

	/** listener to handle button events */
	MButtonListener* ivPtButtonListener;

	String ivTitle;

public:

	FULL_RTTI_SUPER( MAbstractNoteFrame, "MAbstractNoteFrame", MFrameWnd );

	/** constructor */
	MAbstractNoteFrame();

	/** destructor */
	virtual ~MAbstractNoteFrame();

	/** create the window */
	virtual bool create( MRect rect, MTopWnd* parentWnd );

	/** makes the window visible */
	virtual void setVisible( bool value );

	/** updates the controls from model */
	virtual void updateFromModel() = 0;

	virtual void setInstrument( MInstrument* ptInstrument );
	virtual MInstrument* getInstrument();

	virtual void setBpmControl( IBpmControl* ptControl );
	virtual IBpmControl* getBpmControl();

	virtual void setBar( MBar* bar );
	virtual MBar* getBar();

	virtual void setPartSize( MSize* ptSize );
	virtual MSize* getPartSize();

	virtual void storeWindowPosition();
	virtual bool restoreWindowPosition();

	virtual void onCopy();
	virtual void onCut();
	virtual void onPaste();
	virtual void onDelete();

protected:

	virtual void onActivate( bool active );

	virtual void updateWindowText();

	/** invoked when grid content changed */
	virtual void onGridContentChanged();

	/** invoked when note inspector made any changes on notes */
	virtual void onInspectorChanged();

protected:

	friend class MInstrumentListener;

	/** this class hanled incoming events from the instrument */
	class MInstrumentListener :
		public MObject,
		public IFinalizeListener
	{
	protected:
		MInstrument* ivPtInstrument;
		MAbstractNoteFrame* ivPtFrame;
	public:
		FULL_RTTI( MAbstractNoteFrame::MInstrumentListener, "MAbstractNoteFrame::MInstrumentListener" );
		MInstrumentListener( MInstrument* ptInstrument, MAbstractNoteFrame* ptFrame );
		virtual ~MInstrumentListener();
		virtual void objectTerminated( void* obj );
	};

	friend class MBarListener;
	/** this class handles incoming events from the bar */
	class MBarListener :
		public MObject,
		public IBarListener
	{
	protected:
		MAbstractNoteFrame* ivPtBarEditor;
	public:
		FULL_RTTI( MAbstractNoteFrame::MBarListener, "MAbstractNoteFrame::MBarListener" );
		MBarListener( MAbstractNoteFrame* ptEditor );
		virtual ~MBarListener();
		virtual void barColChanged();
		virtual void barRowChanged();
		virtual void barLengthChanged();
		virtual void barResolutionChanged();
		virtual void barReleasing();
	};

	friend class MControlListener;
	class MControlListener :
		public MObject,
		public IControlListener
	{
	protected:
		MAbstractNoteFrame* ivPtFrame;
	public:
		FULL_RTTI( MAbstractNoteFrame::MControlListener, "MAbstractNoteFrame::MControlListener" );
		MControlListener( MAbstractNoteFrame* ptFrame );
		virtual ~MControlListener();
		virtual void valueChanged( MControlListenerEvent *anEvent );
	};

	friend class MButtonListener;
	class MButtonListener :
		public MObject,
		public IActionListener
	{
	protected:
		MAbstractNoteFrame* ivPtFrame;
	public:
		FULL_RTTI( MAbstractNoteFrame::MButtonListener, "MAbstractNoteFrame::MButtonListener" );
		MButtonListener( MAbstractNoteFrame* ptFrame );
		virtual ~MButtonListener();
		virtual void onActionPerformed( void* ptSrc );
	};
};


#endif