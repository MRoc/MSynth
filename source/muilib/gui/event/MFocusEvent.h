#ifndef __MFocusEvent
#define __MFocusEvent

#include "IUiEvent.h"

class MFocusEvent :
	public IUiEvent
{
public:

	enum MFocusEventType
	{
		FOCUS_GAINED,
		FOCUS_LOST
	};

protected:

	MFocusEventType ivType;

public:

	MFocusEvent( MFocusEventType type );
	virtual ~MFocusEvent();

	virtual MFocusEventType getType();
	virtual const IUiEvent::MEventType getEventType() const;

	virtual String toString() const;
};

#endif