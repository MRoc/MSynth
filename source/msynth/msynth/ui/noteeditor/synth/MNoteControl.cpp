/*

	MNoteControl (C)2001 MRoc hifiShock

	Represents a Control to paint on the DC, but this class is not a window,
	the paint-method must be called explicit with a given device-context

*/

#include "MNoteControl.h"

/** construction */
MNoteControl::MNoteControl( MDefaultNote *ptNote ) : MDCControl()
{
	setNote( ptNote );
}

/** destruction */
MNoteControl::~MNoteControl()
{
}

/** Paints this control at the ivRect position in the given device-context */
void MNoteControl::paintControl( IGraphics* pDC )
{
	ASSERT( ivPtNote != 0 );

	MColor bkColor, fgColor;

	int col = 255 - ((int)(((MDefaultSynthNote*)ivPtNote)->getVolume() * 255.0));
	MColor volumeColor = MColorMap::create( col, col, 255 );

	if( getSelected() )
	{
		bkColor = ivBkColorSelected;
		fgColor = volumeColor;
	}
	else
	{
		bkColor = volumeColor;
		fgColor = ivBkColorSelected;
	}

	pDC->fillRect( ivRect, bkColor );
	pDC->drawRect( ivRect, fgColor );
}

/** The note this control references */
void MNoteControl::setNote( MDefaultNote *ptNote )
{
	ivPtNote = ptNote;
}

/** The note this control references */
MDefaultNote *MNoteControl::getNote()
{
	return ivPtNote;
}
