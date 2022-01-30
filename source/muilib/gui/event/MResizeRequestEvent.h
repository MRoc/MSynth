#ifndef __MResizeRequestEvent
#define __MResizeRequestEvent

#include "IUiEvent.h"
#include <framework/primitive/MSize.h>

class MResizeRequestEvent :
	public IUiEvent
{
public:

	MResizeRequestEvent();
	virtual ~MResizeRequestEvent();

	virtual void setMax( MSize max ) = 0;
	virtual void setMin( MSize min ) = 0;
	virtual MSize getMax() = 0;
	virtual MSize getMin() = 0;

	virtual String toString() const;

	virtual const IUiEvent::MEventType getEventType() const;
};

#endif