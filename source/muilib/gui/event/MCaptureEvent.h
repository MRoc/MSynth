#ifndef __MCaptureEvent
#define __MCaptureEvent

#include "IUiEvent.h"

/**
 * set to a window when it lost the mouse capture
 */
class MCaptureEvent : public IUiEvent
{
public:

	enum MCaptureEventType
	{
		CAPTURE_GAINED,
		CAPTURE_LOST
	};

protected:

	MCaptureEventType ivType;

public:

	MCaptureEvent( MCaptureEventType type );
	virtual ~MCaptureEvent();

	virtual MCaptureEventType getType();
	virtual const IUiEvent::MEventType getEventType() const;

	virtual String toString() const;

};

#endif