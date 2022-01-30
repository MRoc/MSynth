#include "MEventNote.h"

/**
 * constructor
 */
MEventNote::MEventNote( MEvent::Type t, MDefaultNote* ptNote ) :
	MEvent( t ),
	ivPtNote( ptNote )
{
}

/**
 * destructor
 */
MEventNote::~MEventNote()
{
}

/**
 * returns the note to be triggered
 */
MDefaultNote* MEventNote::getNote()
{
	return ivPtNote;
}
