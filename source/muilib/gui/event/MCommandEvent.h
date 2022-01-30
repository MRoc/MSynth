#ifndef __MCommandEvent
#define __MCommandEvent

#include "IUiEvent.h"

class MCommandEvent :
	public IUiEvent
{
protected:

	unsigned int ivCommandId;

public:

	MCommandEvent( unsigned int commandId );
	virtual ~MCommandEvent();

	virtual unsigned int getCommandId();
	virtual const IUiEvent::MEventType getEventType() const;

	virtual String toString() const;
};

#endif