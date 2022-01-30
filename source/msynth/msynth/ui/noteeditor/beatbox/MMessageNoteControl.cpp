/*

	MMessageNoteControl (C)2001 MRoc hifiShock

	Represents a Control to paint on the DC, but this class is not a window,
	the paint-method must be called explicit with a given device-context

*/

#include "MMessageNoteControl.h"

//------------------------------------------------------------------------------------
// + CONSTRUCTION
//------------------------------------------------------------------------------------
MMessageNoteControl::MMessageNoteControl( MDefaultMessageNote *ptNote, MMessage *ptMessage ) : MDCControl()
{
	SetNote( ptNote );
	setMsg( ptMessage );
};

//------------------------------------------------------------------------------------
// + DESTRUCTION
//------------------------------------------------------------------------------------
MMessageNoteControl::~MMessageNoteControl()
{
};

//------------------------------------------------------------------------------------
// + PAINT CONTROL
// Paints this control at the ivRect position in the given device-context
//------------------------------------------------------------------------------------
void MMessageNoteControl::paintControl( IGraphics* pDC )
{
	ASSERT( ivPtMessage != 0 );

	MColor bkColor, fgColor;

	int col = 255 - ((int)(((MBeatBoxMessage*)ivPtMessage)->getVolume() * 255.0));
	MColor volumeColor = MColorMap::create( col, col, 255 );

	if( getSelected() )
	{
		fgColor = volumeColor;
		bkColor = getBkColorSelected();
	}
	else
	{
		fgColor = getBkColorSelected();
		bkColor = volumeColor;
	}

	pDC->fillRect( ivRect, bkColor );
	pDC->drawRect( ivRect, fgColor );
};

//------------------------------------------------------------------------------------
// + SET NOTE
// The note this control references
//------------------------------------------------------------------------------------
void MMessageNoteControl::SetNote( MDefaultMessageNote *ptMessageNote )
{
	ivPtNote = ptMessageNote;
};

//------------------------------------------------------------------------------------
// + GET NOTE
// The note this control references
//------------------------------------------------------------------------------------
MDefaultNote *MMessageNoteControl::GetNote()
{
	return ivPtNote;
};

//------------------------------------------------------------------------------------
// + SET MESSAGE
// The message this control references
//------------------------------------------------------------------------------------
void MMessageNoteControl::setMsg( MMessage* ptMessage )
{
	ivPtMessage = ptMessage;
};

//------------------------------------------------------------------------------------
// + GET MESSAGE
// The message this control references
//------------------------------------------------------------------------------------
MMessage* MMessageNoteControl::getMsg()
{
	return ivPtMessage;
};
