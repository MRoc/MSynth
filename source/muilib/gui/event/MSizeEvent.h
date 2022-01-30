#ifndef __MSizeEvent
#define __MSizeEvent

#include "IUiEvent.h"
#include <framework/primitive/MSize.h>

class MSizeEvent :
	public IUiEvent
{
protected:

	MSize ivNewSize;

public:

	MSizeEvent( MSize newSize );
	virtual ~MSizeEvent();

	virtual MSize getNewSize();
	virtual String toString() const;
	virtual const IUiEvent::MEventType getEventType() const;
};

#endif