#ifndef __MCloseEvent
#define __MCloseEvent

#include "IUiEvent.h"

class MCloseEvent :
	public IUiEvent
{
public:
	MCloseEvent();
	virtual ~MCloseEvent();

	virtual const IUiEvent::MEventType getEventType() const;
	virtual String toString() const;
};

#endif