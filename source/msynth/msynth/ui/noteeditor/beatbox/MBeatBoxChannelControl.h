/*
	
	MBeatBoxChannelControl (C)2001 MRoc hifiShock

	The control in the left side of the BeatBoxNoteFrame,
	to select the beatBox's channel

*/

#ifndef __MBeatBoxChannelControl
#define __MBeatBoxChannelControl

#include <framework/MUtil.h>
#include <gui/MGuiUtils.h>
#include <gui/MFont.h>

#include "../../dcgrid/MDCControl.h"
#include "../../../model/source/MWavePlayer.h"

class MBeatBoxChannelControl :
	public MDCControl
{
protected:

	MWavePlayer*		ivPtWavePlayer;

public:

	MBeatBoxChannelControl();
	virtual				~MBeatBoxChannelControl();

	virtual void		paintControl( IGraphics* pDC );

	void				setWavePlayer( MWavePlayer* ptWavePlayer );
	MWavePlayer*		getWavePlayer();
};

#endif
