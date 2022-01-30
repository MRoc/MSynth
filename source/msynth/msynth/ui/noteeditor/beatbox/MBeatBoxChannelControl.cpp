/**
 * MBeatBoxChannelControl (C)2001-2003 MRoc hifiShock
 */

#include "MBeatBoxChannelControl.h"

MBeatBoxChannelControl::MBeatBoxChannelControl() :
	MDCControl()
{
}

MBeatBoxChannelControl::~MBeatBoxChannelControl()
{
}

void MBeatBoxChannelControl::paintControl( IGraphics* pDC )
{
	if( getSelected() )
		MGuiUtils::paintSunkenBorder(
			pDC,
			MRect( ivRect.getX(), ivRect.getY(), ivRect.getWidth() - 1, ivRect.getHeight() - 1 ),
			ivBkColor );
	else
		MGuiUtils::paintRaisedBorder(
			pDC,
			MRect( ivRect.getX(), ivRect.getY(), ivRect.getWidth() - 1, ivRect.getHeight() - 1 ),
			ivBkColor );

	// Draw WavePlayer name
	pDC->drawText(
		ivPtWavePlayer->getSampleName(),
		MRect(
			ivRect.getX() + 3,
			ivRect.getY() + 3,
			ivRect.getWidth() - 6,
			ivRect.getHeight() - 6 ),
		this->ivBkColorSelected );
}

void MBeatBoxChannelControl::setWavePlayer( MWavePlayer* ptWavePlayer )
{
	ivPtWavePlayer = ptWavePlayer;
}

MWavePlayer* MBeatBoxChannelControl::getWavePlayer()
{
	return ivPtWavePlayer;
}
