#include "MEventNoteOn.h"

MEventNoteOn::MEventNoteOn( FP frequency ) :
	MEvent( MEvent::NOTE_ON ),
	ivFrequency( frequency )
{
}

MEventNoteOn::~MEventNoteOn()
{
}

FP MEventNoteOn::getFrequency()
{
	return ivFrequency;
}