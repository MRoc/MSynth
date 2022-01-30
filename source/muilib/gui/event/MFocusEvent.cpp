#include "MFocusEvent.h"

MFocusEvent::MFocusEvent( MFocusEventType type ) :
	ivType( type )
{
}

MFocusEvent::~MFocusEvent()
{
}

MFocusEvent::MFocusEventType MFocusEvent::getType()
{
	return ivType;
}

const IUiEvent::MEventType MFocusEvent::getEventType() const
{
	return IUiEvent::MEventType::EVENT_FOCUS;
}

String MFocusEvent::toString() const
{
	String back = "<iuievent eventtype=\"" + IUiEvent::toString( getEventType() ) + "\" type=\"";
	switch( ivType )
	{
	case FOCUS_GAINED: back += "FOCUS_GAINED"; break;
	case FOCUS_LOST: back += "FOCUS_LOST"; break;
	}
	back += "\"/>";

	return back;
}