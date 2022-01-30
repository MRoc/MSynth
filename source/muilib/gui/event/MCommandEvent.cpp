#include "MCommandEvent.h"
#include <framework/primitive/MInt.h>

MCommandEvent::MCommandEvent( unsigned int commandId ) :
	ivCommandId( commandId )
{
}

MCommandEvent::~MCommandEvent()
{
}

unsigned int MCommandEvent::getCommandId()
{
	return ivCommandId;
}

const IUiEvent::MEventType MCommandEvent::getEventType() const
{
	return IUiEvent::MEventType::EVENT_COMMAND;
}

String MCommandEvent::toString() const
{
	return
		"<iuievent eventtype=\"" +
		IUiEvent::toString( getEventType() ) +
		"\" code=\"" + MInt::toString( ivCommandId ) +
		"\"/>";
}