#include "MCaptureEvent.h"

MCaptureEvent::MCaptureEvent( MCaptureEventType type ) :
	ivType( type )
{
}

MCaptureEvent::~MCaptureEvent()
{
}

MCaptureEvent::MCaptureEventType MCaptureEvent::getType()
{
	return ivType;
}

const IUiEvent::MEventType MCaptureEvent::getEventType() const
{
	return IUiEvent::MEventType::EVENT_CAPTURE;
}

String MCaptureEvent::toString() const
{
	return "<iuievent eventtype=\"" + IUiEvent::toString( getEventType() ) + "\" native=\"both\"/>";
}