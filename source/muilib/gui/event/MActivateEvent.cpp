#include "MActivateEvent.h"

MActivateEvent::MActivateEvent( MActivateEventType type ) :
	ivType( type )
{
}

MActivateEvent::~MActivateEvent()
{
}

MActivateEvent::MActivateEventType MActivateEvent::getType()
{
	return ivType;
}

const IUiEvent::MEventType MActivateEvent::getEventType() const
{
	return IUiEvent::MEventType::EVENT_ACTIVATE;
}

String MActivateEvent::toString() const
{
	return "<iuievent eventtype=\"" + IUiEvent::toString( getEventType() ) + "\" native=\"true\"/>";
}