#ifndef __MArrangeFrame
#define __MArrangeFrame

#include <framework/MTypes.h>
#include <framework/performance/MClock.h>
#include <framework/list/MSelectableObjectList.h>

#include "MBarGrid.h"
#include "MArrangeChannelContainer.h"
#include "MBarLocationBar.h"
#include "../clipboard/MClipBoard.h"
#include <gui/MWndCollection.h>
#include <gui/split/MSplitBar.h>
#include <gui/scrollbar/MScrollPane.h>
#include <gui/combobox/MComboBox.h>
#include <gui/button/MToggleButton.h>
#include <gui/button/MMultiToggleButtonControl.h>

#include "../../../resource.h"
#include "../../model/MSong.h"
#include "../../model/MTimeController.h"
#include "../../model/Sequencer.h"

class MArrangeFrame :
	public MWndCollection,
	public IClipBoardContext,
	public ISelectionListener
{
protected:

	class MComboBoxListener;
	class MSongListener;
	class MToolBarListener;
	class MSizerBarListener;

	MComboBoxListener*			ivPtSelectionListener;

	MSize*						ivPartSize;
	int							ivDividerPosX,
								ivDividerPosY,
								ivSplitBarWidth;

	bool						ivSplitDragged;
	MSplitBar*					ivPtSplitBar;
	MScrollPane*				ivPtScrollPane;
	MBarGrid*					ivPtGrid;
	MArrangeChannelContainer*	ivPtChannelContainer;
	MBarLocationBar*			ivPtLocationBar;

	MToggleButton				*ivPtButtonSelect,
								*ivPtButtonDraw,
								*ivPtButtonGlow,
								*ivPtButtonCut;

	MMultiToggleButtonControl	ivToolBarControl;

	MSong*						ivPtSong;
	MTimeController*			ivPtTimeController;
	MUpdateThread*				ivPtUpdateThread;
	IBpmControl*				ivPtBpmControl;

	MComboBox*					ivPtComboBox;

	MSongListener*				ivPtListener;

	MToolBarListener*			ivPtToolBarListener;

	MSizerBarListener*			ivPtSizerBarListener;

public:

	FULL_RTTI_SUPER( MArrangeFrame, "MArrangeFrame", MWndCollection );

	MArrangeFrame( int id, MSong *ptSong, MTimeController* ptTimeController, MUpdateThread* ptUpdateThread, IBpmControl* ptBpmControl );
	virtual ~MArrangeFrame();

	void				closeChildFrames();
	void				storeChildPositions();

	void				updateFromModel();
	void				doLayout();
	void				updateFromPartSize();

	virtual void		onCopy();
	virtual void		onCut();
	virtual void		onPaste();
	virtual void		onDelete();

	MSize*				getPartSize();

	void				setSong( MSong* ptSong );
	MSong*				getSong();

	void				setTimeController( MTimeController* ptController );
	MTimeController*	getTimeController();

	/**
	 * invoked when bar selection changed
	 */
	virtual void selectionChanged( MObject* newSelection, MObject* ptSource );

	virtual void	onMouseUp( MMouseEvent* anEvent );
	virtual void	onMouseMove( MMouseEvent* anEvent );

	friend MComboBoxListener;
	
protected:

	friend class MSongListener;
	class MSongListener :
		public MObject,
		public ISongListener
	{
	protected:

		MSong* ivPtSong;
		MArrangeFrame* ivPtFrame;

	public:
		FULL_RTTI( MArrangeFrame::MSongListener, "MArrangeFrame::MSongListener" );
		MSongListener( MSong* ptSong, MArrangeFrame* ptFrame );
		virtual ~MSongListener();

		virtual void instrumentAdded( unsigned int index );
		virtual void instrumentRemoved( unsigned int index );
		virtual void instrumentsSwapped( unsigned int index1, unsigned int index2 );
		virtual void songModelChanged();
		virtual void songDestroy();

		virtual String getClassName();
	};

	friend class MComboBoxListener;
	class MComboBoxListener :
		public MObject,
		public ISelectionListener
	{
	protected:

		MArrangeFrame* ivPtFrame;
		MComboBox* ivPtComboBox;

	public:
		FULL_RTTI( MArrangeFrame::MComboBoxListener, "MArrangeFrame::MComboBoxListener" );
		MComboBoxListener( MArrangeFrame* ptParent, MComboBox* ptComboBox );
		virtual ~MComboBoxListener();

		virtual void selectionChanged( MObject* newSelection, MObject* ptSource );
		virtual String getClassName();
	};

	friend class MToolBarListener;
	class MToolBarListener :
		public MObject,
		public IActionListener
	{
	protected:
		MArrangeFrame* ivPtFrame;
	public:
		FULL_RTTI( MArrangeFrame::MToolBarListener, "MArrangeFrame::MToolBarListener" );
		MToolBarListener( MArrangeFrame* ptFrame );
		virtual ~MToolBarListener();
		virtual void onActionPerformed( void* ptSrc );
	};

	friend class MSizerBarListener;
	class MSizerBarListener :
		public MObject,
		public IActionListener
	{
	protected:
		MArrangeFrame* ivPtFrame;
	public:
		FULL_RTTI( MArrangeFrame::MSizerBarListener, "MArrangeFrame::MSizerBarListener" );
		MSizerBarListener( MArrangeFrame* ptFrame );
		virtual ~MSizerBarListener();
		virtual void onActionPerformed( void* ptSrc );
	};
};

#endif