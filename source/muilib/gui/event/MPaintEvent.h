#ifndef __MPaintEvent
#define __MPaintEvent

#include "IUiEvent.h"

class MPaintEvent :
	public IUiEvent
{
public:

	MPaintEvent();
	virtual ~MPaintEvent();

	virtual const IUiEvent::MEventType getEventType() const;
	virtual String toString() const;
};

#endif