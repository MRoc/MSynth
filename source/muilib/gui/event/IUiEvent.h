#ifndef __IUiEvent
#define __IUiEvent

#include <framework/MTypes.h>

/**
 * interface of all UI events
 */
class IUiEvent
{
public:

	/**
	 * type of ui event
	 */
	enum MEventType
	{
		EVENT_MOUSE,			// ok
		EVENT_KEY,				// ok
		EVENT_HOVER,			// ok
		EVENT_FOCUS,			// ok
		EVENT_CAPTURE,          // ok
		EVENT_COMMAND,			// ok
		EVENT_PAINT,			// ok
		EVENT_SIZE,				// ok
		EVENT_DESTROY,			// ok
		EVENT_ACTIVATE,			// ok
		EVENT_CLOSE,			// ok
		EVENT_RESIZE_REQUEST,	// ok
		EVENT_POSITION_CHANGED, // ok
		EVENT_NATIVE			// ok
	};

	/**
	 * to be overwritten in subclasses, 
	 * returns the type of event.
	 */
	virtual const MEventType getEventType() const = 0;

	/**
	 * returns a string representation of the event
	 */
	virtual String toString() const = 0;

	/**
	 * returns a string representation of the given eventtype
	 */
	static String toString( MEventType type )
	{
		switch( type )
		{
		case EVENT_MOUSE: return "EVENT_MOUSE";	break;
		case EVENT_KEY: return "EVENT_KEY"; break;
		case EVENT_HOVER: return "EVENT_HOVER"; break;
		case EVENT_FOCUS: return "EVENT_FOCUS"; break;
		case EVENT_CAPTURE: return "EVENT_CAPTURE"; break;
		case EVENT_COMMAND: return "EVENT_COMMAND"; break;
		case EVENT_PAINT: return "EVENT_PAINT"; break;
		case EVENT_SIZE: return "EVENT_SIZE"; break;
		case EVENT_DESTROY: return "EVENT_DESTROY"; break;
		case EVENT_ACTIVATE: return "EVENT_ACTIVATE"; break;
		case EVENT_CLOSE: return "EVENT_CLOSE"; break;
		case EVENT_RESIZE_REQUEST: return "EVENT_RESIZE_REQUEST"; break;
		case EVENT_NATIVE: return "EVENT_NATIVE"; break;
		default: return "UNKNOWN"; break;
		}
	}
};
#endif