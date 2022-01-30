#include "MSizeEvent.h"

MSizeEvent::MSizeEvent( MSize newSize ) :
	ivNewSize( newSize )
{
}

MSizeEvent::~MSizeEvent()
{
}

MSize MSizeEvent::getNewSize()
{
	return ivNewSize;
}

const IUiEvent::MEventType MSizeEvent::getEventType() const
{
	return IUiEvent::MEventType::EVENT_SIZE;
}

String MSizeEvent::toString() const
{
	return
		"<iuievent eventtype=\"" +
		IUiEvent::toString( getEventType() ) +
		"\" native=\"true\"/>";
}