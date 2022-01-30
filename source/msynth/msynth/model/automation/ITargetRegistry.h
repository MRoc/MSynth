/*
	(c)2002 MRoc

	-	A object registering its controls MUST deregister them when being destroyed.

	-	A IControl having events to record knows the right context by
			
		1. quering the ControlRegistry
		2. getting the IAutomationTracker out of the IControlContext

	-	The IControlContext holds the target that will track the events produced by the
		control, the IAutomationTracker. This can perhaps be a barcontainer.

	-	the sequencer must trigger every event found, the target control is saved
		in the event.

	to do:

		- IControlCollection must unregister it controls
*/
#ifndef __ITARGETREGISTRY
#define __ITARGETREGISTRY

#include "IAutomationContext.h"
#include <framework/listener/IListener.h>

/**
 * interface for beeing informed about
 * changes in the target registry
 */
class ITargetRegistryListener :
	public IListener
{
public:

	/** 
	 * invoked when a event target was added to the registry
	 */
	virtual void automationAdded( IEventTarget* ptControl ) = 0;

	/** 
	 * invoked when a event target was removed to the registry
	 */
	virtual void automationRemoved( IEventTarget* ptControl ) = 0;
};

/** 
 * defines the target registry.
 *
 * 1)
 * it defines a map that stores IAutomationContext as values under
 * their IEventTarget ad key.
 * a IAutomationContext can be added and removed.
 * so it is a registry returning a IAutomationTracker for
 * a IEventTarget. used e.g. when a IEventTarget wants to store
 * a event. it queries first the registry for its tracker and then
 * records its event.
 *
 * 2)
 * it also stores all event targets that are in automation.
 * event targets can be added/removed from automation.
 * a listener can be registered for such an event.
 */
class ITargetRegistry
{
public:

	/**
	 * registers a automation context for a target
	 */
	virtual void registerContext( IAutomationContext* ptContext ) = 0;

	/** 
	 * unregisters the context for the given target
	 */
	virtual void unregisterContext( IEventTarget* ptTarget ) = 0;

	/** 
	 * query registry for the automation context of the given target
	 */
	virtual IAutomationContext* getContext( IEventTarget* ptTarget ) = 0;


	/**
	 * adds a control to automation
	 */
	virtual void addToAutomation( IEventTarget* ptControl ) = 0;

	/** 
	 * removes a control from automation
	 */
	virtual void removeFromAutomation( IEventTarget* ptControl ) = 0;

	/** 
	 * returns true if the given target is in automation
	 */
	virtual bool isInAutomation( IEventTarget* ptControl ) = 0;

	/** 
	 * removes all events stored in automation for the given target
	 */
	virtual void removeAllEvents( IEventTarget* ptTarget ) = 0;


	/**
	 * adds a registry listener to be informed about changed
	 * on the automation targets.
	 */
	virtual void addListener( ITargetRegistryListener* ptListener ) = 0;

	/**
	 * removes a registry listener to be informed about changed
	 * on the automation targets.
	 */
	virtual void removeListener( ITargetRegistryListener* ptListener ) = 0;
};

#endif