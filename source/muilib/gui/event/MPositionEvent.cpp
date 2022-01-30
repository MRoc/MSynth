#include "MPositionEvent.h"

/** 
 * constructor
 */
MPositionEvent::MPositionEvent( const MRect& newRect ) :
	ivNewPosition( newRect )
{
}

/**
 * destructor
 */
MPositionEvent::~MPositionEvent()
{
}

/**
 * to be overwritten in subclasses, 
 * returns the type of event.
 */
const IUiEvent::MEventType MPositionEvent::getEventType() const
{
	return IUiEvent::MEventType::EVENT_POSITION_CHANGED;
}

/**
 * returns a string representation of the event
 */
String MPositionEvent::toString() const
{
	return
		"<iuievent eventtype=\"" +
		IUiEvent::toString( getEventType() ) +
		"\" rect=\"" +
		ivNewPosition.toString().getData() + 
		"\" native=\"only\"/>";
}

/**
 * returns the new window position
 */
MRect MPositionEvent::getNewRect() const
{
	return ivNewPosition;
}