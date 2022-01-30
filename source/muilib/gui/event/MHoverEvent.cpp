#include "MHoverEvent.h"

/**
 * constructor
 */
MHoverEvent::MHoverEvent( MHoverEventType type ) :
	ivType( type )
{
}

/** 
 * destructor
 */
MHoverEvent::~MHoverEvent()
{
}

/**
 * returns the hover event type (gained/lost)
 */
MHoverEvent::MHoverEventType MHoverEvent::getType()
{
	return ivType;
}

/**
 * returns the ui event type
 */
const IUiEvent::MEventType MHoverEvent::getEventType() const
{
	return IUiEvent::MEventType::EVENT_HOVER;
}

String MHoverEvent::toString() const
{
	String back = "<iuievent eventtype=\"" + IUiEvent::toString( getEventType() ) + "\" type=\"";
	switch( ivType )
	{
	case HOVER_GAINED: back += "HOVER_GAINED"; break;
	case HOVER_LOST: back += "HOVER_LOST"; break;
	}
	back += "\"/>";
	return back;
}