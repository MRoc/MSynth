/**
 * (C)2001-2003 MRoc
 *
 * This class contains the MBeatBoxChannelControls on the
 * left side of the BeatBoxNoteFrame
 *
 */

#ifndef __MBeatBoxChannelControlContainer
#define __MBeatBoxChannelControlContainer

#include <gui/MyInteractiveWnd.h>
#include "MBeatBoxChannelControl.h"
#include "../../../model/MBeatBox.h"
#include "../../../model/source/MWavePlayer.h"


class MBeatBoxChannelControlContainer :
	public MyInteractiveWnd
{
protected:

	class MBeatBoxListener;

	MSize*				ivPtSize;
	MBeatBox*			ivPtBeatBox;

	int					ivSelectedChannel;
	std::vector<MBeatBoxChannelControl*> ivArControls;

	MBeatBoxChannelControlContainer::MBeatBoxListener* ivPtListener;

public:

	MBeatBoxChannelControlContainer( MBeatBox* ptBeatBox, MSize* ptPartSize );
	virtual				~MBeatBoxChannelControlContainer();

	virtual void		paint( IGraphics* ptGraphics, const MRect &rect );

	void				layoutControls();
	void				updateFromModel();

	void				SetSelectedChannel( int channelNo );
	int					GetSelectedChannel();

	//LRESULT				OnBeatBoxDestroy( WPARAM wParam, LPARAM lParam );

protected:

	void				setBeatBox( MBeatBox* ptBeatBox );
	MBeatBox*			getBeatBox();

	void				setPartSize( MSize* ptSize );
	MSize*				getPartSize();

	void				deleteControls();

	void				RegisterListeners();
	void				UnregisterListeners();

	virtual void		onMouseDown( MMouseEvent* anEvent );

	class MBeatBoxListener : 
		public MObject,
		public IInstrumentListener,
		public IControlListener
	{
	protected:

		MBeatBox* ivPtBeatBox;
		MBeatBoxChannelControlContainer* ivPtContainer;

	public:

		MBeatBoxListener( MBeatBox* ptBeatBox, MBeatBoxChannelControlContainer* ptContainer );
		virtual ~MBeatBoxListener();

		virtual void stateChanged( MInstrumentEvent anEvent );
		virtual String getInstrumentListenerName();

		virtual String getClassName();

		virtual void valueChanged( MControlListenerEvent *anEvent );

		FULL_RTTI( MBeatBoxChannelControlContainer::MBeatBoxListener, "MBeatBoxChannelControlContainer::MBeatBoxListener" );
	};
};

#endif
