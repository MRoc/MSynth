#ifndef __MDestroyEvent
#define __MDestroyEvent

#include "IUiEvent.h"

class MDestroyEvent :
	public IUiEvent
{
public:

	MDestroyEvent();
	virtual ~MDestroyEvent();

	virtual const IUiEvent::MEventType getEventType() const;
	virtual String toString() const;
};

#endif