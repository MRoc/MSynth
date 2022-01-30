#include "MPaintEvent.h"

MPaintEvent::MPaintEvent()
{
}

MPaintEvent::~MPaintEvent()
{
}

const IUiEvent::MEventType MPaintEvent::getEventType() const
{
	return IUiEvent::MEventType::EVENT_PAINT;
}

String MPaintEvent::toString() const
{
	return
		"<iuievent eventtype=\"" +
		IUiEvent::toString( getEventType() ) +
		"\" native=\"true\"/>";
}