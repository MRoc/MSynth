#ifndef __MEventNoteOn
#define __MEventNoteOn

#include "MEvent.h"

class MEventNoteOn :
	public MEvent
{
protected:

	FP ivFrequency;

public:

	MEventNoteOn( FP frequency );

	virtual ~MEventNoteOn();

	virtual FP getFrequency();
};

#endif