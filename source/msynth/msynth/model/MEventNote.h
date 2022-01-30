#ifndef __MEventNote
#define __MEventNote

#include "event/MEvent.h"
#include "note/MDefaultNote.h"

/**
 * a note event,
 * triggered by sequencer to synth/beatbox
 */
class MEventNote :
	public MEvent
{
protected:

	/**
	 * the note to be triggered
	 */
	MDefaultNote* ivPtNote;

public:

	/**
	 * constructor
	 */
	MEventNote( Type t, MDefaultNote* ptNote );

	/**
	 * destructor
	 */
	virtual ~MEventNote();

	/**
	 * returns the note to be triggered
	 */
	virtual MDefaultNote* getNote();
};

#endif