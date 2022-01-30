#ifndef __MHoverEvent
#define __MHoverEvent

#include "IUiEvent.h"

class MHoverEvent :
	public IUiEvent
{
public:

	/**
	 * the type of event
	 */
	enum MHoverEventType
	{
		HOVER_GAINED,
		HOVER_LOST
	};

protected:

	/**
	 * the hover event type
	 */
	MHoverEventType ivType;

public:

	/**
	 * constructor
	 */
	MHoverEvent( MHoverEventType type );

	/** 
	 * destructor
	 */
	virtual ~MHoverEvent();

	/**
	 * returns the hover event type (gained/lost)
	 */
	virtual MHoverEventType getType();

	/**
	 * returns the ui event type
	 */
	virtual const IUiEvent::MEventType getEventType() const;

	/**
	 * returns a string representation
	 */
	virtual String toString() const;
};

#endif