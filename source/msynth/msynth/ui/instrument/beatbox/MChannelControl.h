/**
 * (C)2000-2003 MRoc
 */

#ifndef __MCHANNELCONTROL
#define __MCHANNELCONTROL

#include <framework/MTypes.h>
#include <framework/MNativeTypes.h>
#include <mmsystem.h>
#include <dsound.h>

#include "../../processor/MWavePlayerControl.h"
#include <gui/MWndCollection.h>
#include <gui/scrollbar/MScrollPane.h>

#include "../../../../resource.h"
#include "../../../model/MBeatBox.h"

typedef std::vector<MWavePlayerControl*> WavePlayerControlList;

class MChannelControl :
	public MWndCollection
{
protected:

	class MBeatBoxListener;
	class MActionListener;

	MBeatBox* ivPtBeatBox;
	WavePlayerControlList ivArControls;
	MBeatBoxListener* ivPtListener;
	MActionListener* ivPtActionListener;

public:

	MChannelControl();
	virtual ~MChannelControl();

	virtual void		doLayout();

	void				setBeatBox( MBeatBox *ptDrumBox );
	MBeatBox*			getBeatBox();

	void				updateGui();
	void				updateFromModel();

	void				setActiveChannel( MObject* activeChannel );

	MSize				getPreferredSize();

protected:

	void				deleteControls();
	void				createControl( unsigned int index );
	void				layoutControl( MWavePlayerControl* ptControl, unsigned int index );

	void				onUpdateFromModel();
	void				onWaveplayerAdded( unsigned int index );
	void				onWaveplayerRemoved( unsigned int index );
	void				onSelectedChannelChanged( unsigned int index );

	friend class MBeatBoxListener;
	class MBeatBoxListener :
		public MObject,
		public IInstrumentListener
	{
	protected:
		MBeatBox* ivPtBeatBox;
		MChannelControl* ivPtControl;
	public:
		FULL_RTTI( MChannelControl::MBeatBoxListener, "MChannelControl::MBeatBoxListener" );
		MBeatBoxListener( MBeatBox* ptBeatBox, MChannelControl* ptChannelControl );
		virtual ~MBeatBoxListener();
		virtual void stateChanged( MInstrumentEvent anEvent );
		virtual String getInstrumentListenerName();
		virtual String getClassName();
	};

	friend class MActionListener;
	/**
	 * handles incoming events from waveplayer gui selection
	 */
	class MActionListener :
		public MObject,
		public IActionListener
	{
	protected:
		MChannelControl* ivPtControl;
	public:
		FULL_RTTI( MChannelControl::MActionListener, "MChannelControl::MActionListener" );
		MActionListener( MChannelControl* ptControl );
		virtual ~MActionListener();
		virtual void onActionPerformed( void* ptSrc );
	};
};

#endif
