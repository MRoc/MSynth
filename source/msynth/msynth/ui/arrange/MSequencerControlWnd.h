/*(C)2000 MRoc hifiShock*/

#ifndef __MSequencerControlWnd
#define __MSequencerControlWnd

#include "MPlayPositionLabel.h"

#include <gui/MWndCollection.h>
#include <gui/counter/MCountControl.h>
#include <gui/button/MToggleButton.h>
#include <gui/scrollbar/MScrollBar.h>
#include <gui/combobox/MComboBox.h>
#include <gui/layout/MLayout.h>

#include "../dialog/MRecordDialog.h"
#include "../../model/Sequencer.h"
#include "../../model/MUpdateThread.h"
#include "../../model/MSong.h"
#include "../../model/MSequencerControl.h"
#include "../../model/automation/ITargetRegistry.h"
#include "../../model/streamingengine/IAudioEngine.h"

#include "../../../resource.h"

class MSequencerControlWnd :
	public MWndCollection,
	public ISpinButtonModelListener
{
protected:

	class MPlayStateListener;
	class MButtonListener;

	MSequencerControl*	ivPtSequencerControl;

	//BUTTONS
	MToggleButton* ivPtButtonPlay;
	MToggleButton* ivPtButtonStreaming;
	MToggleButton* ivPtButtonRecord;
	MButton* ivPtButtonStop;

	// COUNTER
	MCountControl* ivPtCountControl;
	MCountControl* ivPtShuffleControl;

	MPlayPositionLabel* ivPtPosLabel;

	MLayout ivLayout;

	MPlayStateListener* ivPtPlaystateListener;
	MButtonListener* ivPtButtonListener;

public:

	FULL_RTTI_SUPER( MSequencerControlWnd, "MSequencerControlWnd", MWndCollection );

	MSequencerControlWnd();
	virtual ~MSequencerControlWnd();

	virtual void paint( IGraphics* ptGraphics, const MRect &rect );
	virtual void doLayout();

	void Play();
	void Export();
	void SetBPM();
	void updateFromModel();
	void SetSequencerControl( MSequencerControl* ptControl );

	virtual void valueChanged( MSpinButtonModel* pModel );

protected:

	virtual void onCommand( unsigned int id );

	friend class MPlayStateListener;
	/**
	 * implementation of a IPlaystateListener so that the
	 * MSequencerControlWnd gets informed about play state changes.
	 */
	class MPlayStateListener :
		public MObject,
		public IPlayStateListener
	{
	protected:
		/**
		 * reference to the control
		 */
		MSequencerControlWnd* ivPtControl;
		/**
		 * reference to the sequencer control model
		 */
		MSequencerControl* ivPtSControl;
	public:
		FULL_RTTI( MSequencerControlWnd::MPlayStateListener, "MSequencerControlWnd::MPlayStateListener" );
		/**
		 * constructor
		 */
		MPlayStateListener( MSequencerControlWnd* ptControl, MSequencerControl* ptModel );
		/**
		 * destructor
		 */
		virtual ~MPlayStateListener();
		/**
		 * invoked when play state changed
		 */
		virtual void onPlayStateChanged( IPlayState::PlayState newState );
	};

	friend class MButtonListener;
	class MButtonListener :
		public MObject,
		public IActionListener
	{
	protected:
		MSequencerControlWnd* ivPtWnd;
	public:
		FULL_RTTI( MSequencerControlWnd::MButtonListener, "MSequencerControlWnd::MButtonListener" );
		MButtonListener( MSequencerControlWnd* ptwnd );
		virtual ~MButtonListener();
		virtual void onActionPerformed( void* ptSrc );
	};
};

#endif