#include "MCloseEvent.h"

MCloseEvent::MCloseEvent()
{
}

MCloseEvent::~MCloseEvent()
{
}

const IUiEvent::MEventType MCloseEvent::getEventType() const
{
	return IUiEvent::MEventType::EVENT_CLOSE;
}

String MCloseEvent::toString() const
{
	return "<iuievent eventtype=\"" + IUiEvent::toString( getEventType() ) + "\"/>";
}