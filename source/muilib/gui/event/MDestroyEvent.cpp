#include "MDestroyEvent.h"

MDestroyEvent::MDestroyEvent()
{
}

MDestroyEvent::~MDestroyEvent()
{
}

const IUiEvent::MEventType MDestroyEvent::getEventType() const
{
	return IUiEvent::MEventType::EVENT_DESTROY;
}

String MDestroyEvent::toString() const
{
	return
		"<iuievent eventtype=\"" +
		IUiEvent::toString( getEventType() ) +
		"\" native=\"true\"/>";
}