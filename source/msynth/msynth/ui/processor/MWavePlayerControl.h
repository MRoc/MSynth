/**
 * (C)2000-2003 MRoc
 */

#ifndef __MWAVEPLAYERCONTROL
#define __MWAVEPLAYERCONTROL

#include <gui/button/MToggleButton.h>
#include <gui/MWndCollection.h>
#include "../../../resource.h"	
#include "../../model/source/MWavePlayer.h"
#include "../control/MFloatGui.h"
#include "../control/MBoolGui.h"
#include "../control/MStringGui.h"

#define BUTTON_SAMPLE_SELECTION_ID		5000
#define LOAD_BUTTON_ID					5100
#define SAVE_BUTTON_ID					5200
#define REV_BUTTON_ID					5300
#define LOOP_BUTTON_ID					5400
#define MUTE_BUTTON_ID					5500
#define SAMPLENAME_PANEL_ID				5900

class MWavePlayerControl :
	public MWndCollection
{
public:

	static const int WPC_DEFAULT_WIDTH;
	static const int WPC_DEFAULT_HEIGHT;

protected:

	class MControlListener;
	class MActionListener;

	MControlListener* ivPtControlListener;
	MActionListener* ivPtActionListener;
	MListenerList ivActionListener;

	MToggleButton* ivPtButtonChannelSelect;
	MButton* ivPtButtonLoad;
	MButton* ivPtButtonSave;

	MStringGui* ivPtNameGui;
	MBoolGui* ivPtReverseControl;
	MBoolGui* ivPtLoopControl;
	MBoolGui* ivPtMuteControl;
	MBoolGui* ivPtExclusiveControl;

	MFloatGui* ivPtPitchControl;
	MFloatGui* ivPtVolControl;
	MFloatGui* ivPtPanControl;

	MWavePlayer* ivPtWavePlayer;

	unsigned int ivIndex;

public:

	MWavePlayerControl( MWavePlayer *ptWavePlayer, int id );
	~MWavePlayerControl();

	virtual void			paint( IGraphics* ptGraphics, const MRect &rect );
	virtual void			doLayout();

	virtual MWavePlayer*	getWavePlayer();
	virtual void			setWavePlayer(MWavePlayer *wavePlayer);

	virtual void			setActiveChannel(bool value);
	virtual bool			getActiveChannel();

	virtual void			setId( int id );

	void					addActionListener( IActionListener* ptListener );
	void					removeActionListener( IActionListener* ptListener );

protected:

	void					fireActionPerformed( void* ptSrc );
	void					enableControls( bool enable );

	friend class MControlListener;
	class MControlListener :
		public MObject,
		public IControlListener
	{
	protected:
		MWavePlayerControl* ivPtGui;
		MWavePlayer* ivPtModel;
	public:
		MControlListener( MWavePlayerControl* ptGui, MWavePlayer* ptModel );
		virtual ~MControlListener();
		virtual void valueChanged( MControlListenerEvent *anEvent );
		FULL_RTTI( MWavePlayerControl::MControlListener, "MWavePlayerControl::MControlListener" );
	};

	friend class MActionListener;
	class MActionListener :
		public MObject,
		public IActionListener
	{
	protected:
		MWavePlayerControl* ivPtControl;
	public:
		MActionListener( MWavePlayerControl* ptControl );
		virtual ~MActionListener();
		virtual void onActionPerformed( void* ptSrc );
		FULL_RTTI( MWavePlayerControl::MActionListener, "MWavePlayerControl::MActionListener" );
	};
};

#endif
