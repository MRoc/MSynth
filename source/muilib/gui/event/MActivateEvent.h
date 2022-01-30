#ifndef __MActivateEvent
#define __MActivateEvent

#include "IUiEvent.h"

class MActivateEvent :
	public IUiEvent
{
public:

	enum MActivateEventType
	{
		ACTIVATION,
		DEACTIVATION
	};

protected:

	MActivateEventType ivType;

public:

	MActivateEvent( MActivateEventType type );
	virtual ~MActivateEvent();

	virtual MActivateEventType getType();
	virtual const IUiEvent::MEventType getEventType() const;

	virtual String toString() const;
};

#endif