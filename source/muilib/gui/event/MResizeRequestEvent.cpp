#include "MResizeRequestEvent.h"


MResizeRequestEvent::MResizeRequestEvent()
{
}

MResizeRequestEvent::~MResizeRequestEvent()
{
}

const IUiEvent::MEventType MResizeRequestEvent::getEventType() const
{
	return IUiEvent::MEventType::EVENT_RESIZE_REQUEST;
}

String MResizeRequestEvent::toString() const
{
	return
		"<iuievent eventtype=\"" +
		IUiEvent::toString( getEventType() ) +
		"\" native=\"true\"/>";
}