#ifndef __MPositionEvent
#define __MPositionEvent

#include "IUiEvent.h"
#include <framework/primitive/MRect.h>

/**
 * native event sent by the framework to a toplevel window
 */
class MPositionEvent : public IUiEvent
{
protected:

	/**
	 * the windows's new position and size
	 */
	MRect ivNewPosition;

public:

	/** 
	 * constructor
	 */
	MPositionEvent( const MRect& newRect );

	/**
	 * destructor
	 */
	virtual ~MPositionEvent();

	/**
	 * to be overwritten in subclasses, 
	 * returns the type of event.
	 */
	virtual const MEventType getEventType() const;

	/**
	 * returns a string representation of the event
	 */
	virtual String toString() const;

	/**
	 * returns the new window position
	 */
	virtual MRect getNewRect() const;
};

#endif