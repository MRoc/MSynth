/*
	(C)2001-2003 MRoc hifisShock

	Default note is a note only for monophonic synths
*/

#include "MDefaultNote.h"

MDefaultNote::MDefaultNote() :
	MListItem(),
	ivDuration( 0 )
{
}

MDefaultNote::MDefaultNote( MDefaultNote* ptNote ) :
	MListItem( ptNote ),
	ivDuration( ptNote->getDuration() )
{
}

MDefaultNote::~MDefaultNote()
{
}

void MDefaultNote::setStartsAt( int startsAt )
{
	setIndex( startsAt );
}

int MDefaultNote::getStartsAt()
{
	return this->ivIndex;
}

// The duration, the length before release or another note
void MDefaultNote::setDuration( unsigned int duration )
{
	ivDuration = duration;
}

unsigned int MDefaultNote::getDuration()
{
	return ivDuration;
}

// Stops at, the location, this note stops (calculated from starts ad and duration)
int MDefaultNote::getStopsAt()
{
	return (int)(ivIndex + ivDuration) - 1;
}

void MDefaultNote::setStopsAt( int stopsAt )
{
	ASSERT( stopsAt >= ivIndex );
	ivDuration = stopsAt - ivIndex + 1;
}