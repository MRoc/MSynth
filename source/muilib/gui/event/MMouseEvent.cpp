#include "MMouseEvent.h"

MMouseEvent::MMouseEvent( MMouseButton b, MPoint point, MMouseEventType type, bool shift, bool control ) :
	ivButton( b ),
	ivPoint( point ),
	ivType( type ),
	ivShiftPressed( shift ),
	ivControlPressed( control )
{
}

MMouseEvent::~MMouseEvent()
{
}

const MMouseEvent::MMouseButton MMouseEvent::getButton()
{
	return ivButton;
}

const MMouseEvent::MMouseEventType MMouseEvent::getType()
{
	return ivType;
}

const int MMouseEvent::getX()
{
	return ivPoint.getX();
}

const int MMouseEvent::getY()
{
	return ivPoint.getY();
}

MPoint MMouseEvent::getPoint()
{
	return ivPoint;
}

const IUiEvent::MEventType MMouseEvent::getEventType() const
{
	return IUiEvent::MEventType::EVENT_MOUSE;
}

bool MMouseEvent::getShiftPressed()
{
	return ivShiftPressed;
}

bool MMouseEvent::getControlPressed()
{
	return ivControlPressed;
}

String MMouseEvent::toString() const
{
	String back = "<iuievent eventtype=\"" + IUiEvent::toString( getEventType() ) + "\" type=\"";
	switch( ivType )
	{
	case MOUSE_DOWN: back += "MOUSE_DOWN"; break;
	case MOUSE_UP: back += "MOUSE_UP"; break;
	case MOUSE_DOUBLECLICK: back += "MOUSE_DOUBLECLICK"; break;
	case MOUSE_MOVE: back += "MOUSE_MOVE"; break;
	case MOUSE_WHEEL_UP: back += "MOUSE_WHEEL_UP"; break;
	case MOUSE_WHEEL_DOWN: back += "MOUSE_WHEEL_DOWN"; break;
	}
	back +=
		"\" button=\"";
	switch( ivButton )
	{
	case NEUTRAL: back += "NEUTRAL"; break;
	case BUTTON1: back += "BUTTON1"; break;
	case BUTTON2: back += "BUTTON2"; break;
	}
	back += "\"/>";		

	return back;
}