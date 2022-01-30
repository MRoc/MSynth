#ifndef __IAUTOMATIONTRACKER
#define __IAUTOMATIONTRACKER

#include "IAutomationEvent.h"
#include "IEventTarget.h"

/**
 * the automation tracker defines a interface
 * that can record events.
 */
class IAutomationTracker
{
public:

	/**
	 * inserts the automation event in the event list at the given position
	 */
	virtual void addAutomationEvent(
		IAutomationEvent* ptEvent,
		IEventTarget* ptTarget,
		unsigned int pattern,
		unsigned int systemStep ) = 0;

	/**
	 * removes the given automation event
	 */
	virtual void removeAutomationEvent( IAutomationEvent* ptEvent ) = 0;

	/**
	 * removes all automation events for this control
	 */
	virtual void removeAutomationEvents( IEventTarget* ptTarget ) = 0;

	/** 
	 * collects all targets that are handled in this context
	 */
	virtual void collectTargets( std::vector<IEventTarget*> &collector ) = 0;
};

#endif