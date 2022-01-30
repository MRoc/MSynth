#ifndef __IEventTarget
#define __IEventTarget

#include <framework/listener/IFinalizeListener.h>
#include "IAutomationEvent.h"

class IEventTargetListener;

/**
 * defines a automation target
 *
 * a target can process a automation event.
 * it also can record events.
 *
 * it can be queried for a name. this name must not be unique.
 *
 * the automation state can be changed and queried.
 * the automation state defines, what the target should do
 * with generated events. if added to automation state,
 * it routes the events into the ... else it discards all
 * events.
 *
 * a finalize listener can be added.
 *
 */
class IEventTarget :
	public IMUnknown
{
public:

	/**
	 * the automation state defines, what the target should do
	 * with generated events. if added to automation state,
	 * it routes the events into the ... else it discards all
	 * events.
	 */
	enum AutomationState
	{
		ADDED_TO_AUTOMATION,
		REMOVED_FROM_AUTOMATION
	};

	/**
	 * returns the name of the target
	 */
	virtual String getName() = 0;

	/**
	 * returns a string representation of the target
	 */
	virtual String toString() = 0;

	/**
	 * the input side, processes an automation event
	 */
	virtual void postEvent( IAutomationEvent* ptEvent ) = 0;

	/** 
	 * sets the automation state
	 */
	virtual void setAutomationState( AutomationState value ) = 0;

	/** 
	 * returns the automation state
	 */
	virtual AutomationState getAutomationState() = 0;

	/** 
	 * adds a finalize listener
	 */
	virtual void addListener( IEventTargetListener* ptListener ) = 0;

	/** 
	 * removes a finalize listener
	 */
	virtual void removeListener( IEventTargetListener* ptListener ) = 0;
};

/**
 * a listener to install at a eventtarget
 */
class IEventTargetListener :
	public IListener
{
public:

	/**
	 * the types of event target events :-)
	 */
	enum Type
	{
		ET_ADDED_TO_AUTOMATION,
		ET_REMOVED_FROM_AUTOMATION,
		ET_TERMINATING
	};

	/**
	 * invoked when event target state changed
	 */
	virtual void onEventTargetChange( IEventTarget* pTarget, Type type ) = 0;
};

#endif