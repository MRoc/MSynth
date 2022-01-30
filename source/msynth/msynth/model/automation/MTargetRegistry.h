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
#ifndef __MTargetRegistry
#define __MTargetRegistry

#include <framework/MTypes.h>
#include <framework/listener/MListenerList.h>

#include "ITargetRegistry.h"
#include "MTargetIdRegistry.h"

typedef std::map<IEventTarget*, IAutomationContext*> ContextMap;
typedef std::map<IEventTarget*, IEventTarget*> AutomationMap;
typedef ContextMap::iterator ContextMapIter;

/** 
 * implements the target registry as singleton instance.
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
 *
 * 3)
 * stores the current record mode (RecordMode:DISCARD_EVENTS/RECORD_EVENTS)
 */
class MTargetRegistry : public ITargetRegistry
{
public:

	/** 
	 * the app global mode for automation
	 */
	enum RecordMode
	{
		DISCARD_EVENTS,
		RECORD_EVENTS
	};

private:

	/** 
	 * the singleton instance
	 */
	static MTargetRegistry* gvPtInstance;

	/**
	 * the gloabl record mode
	 */
	RecordMode ivRecordMode;

	/**
	 * this context map stores all IEventTargets and their context
	 */
	ContextMap ivContextMap;

	/**
	 * this map stores all controls added to automation
	 */
	AutomationMap ivAutomationMap;

	/**
	 * stores all listeners
	 */
	MListenerList ivListeners;

	/**
	 * this registry is to resolve ids when serializing
	 */
	MTargetIdRegistry ivIdRegistry;

private:

	/**
	 * constructor
	 */
	MTargetRegistry();

	/**
	 * destructor
	 */
	virtual ~MTargetRegistry();

public:

	/**
	 * returns the singleton instance
	 */
	static MTargetRegistry* getInstance();

	/**
	 * releases the singleton instance
	 */
	static void release();


	/**
	 * sets the global play mode (what should be done with recorded events)
	 */
	virtual void setRecordMode( RecordMode mode );

	/**
	 * returns the global play mode (what should be done with recorded events)
	 */
	virtual RecordMode getRecordMode();

	/** 
	 * returns the id map used for serialisation
	 */
	virtual ITargetIdRegistry* getIdRegistry();


	/**
	 * registers a automation context for a target
	 * inheritanced from ITargetRegistry
	 */
	virtual void registerContext( IAutomationContext* ptContext );

	/**
	 * unregisters a automation context for a target
	 * inheritanced from ITargetRegistry
	 */
	virtual void unregisterContext( IEventTarget* ptTarget );

	/** 
	 * query registry for the automation context of the given target
	 * inheritanced from ITargetRegistry
	 */
	virtual IAutomationContext* getContext( IEventTarget* ptControl );


	/**
	 * adds a control to automation
	 * inheritanced from ITargetRegistry
	 */
	virtual void addToAutomation( IEventTarget* ptControl );

	/** 
	 * removes a control from automation
	 * inheritanced from ITargetRegistry
	 */
	virtual void removeFromAutomation( IEventTarget* ptControl );

	/** 
	 * returns true if the given target is in automation
	 * inheritanced from ITargetRegistry
	 */
	virtual bool isInAutomation( IEventTarget* ptControl );

	/**
	 * adds a registry listener to be informed about changed
	 * on the automation targets.
	 * inheritanced from ITargetRegistry
	 */
	virtual void addListener( ITargetRegistryListener* ptListener );

	/**
	 * removes a registry listener to be informed about changed
	 * on the automation targets.
	 * inheritanced from ITargetRegistry
	 */
	virtual void removeListener( ITargetRegistryListener* ptListener );


	/** 
	 * removes all events stored in automation for the given target
	 * inheritanced from ITargetRegistry
	 */
	virtual void removeAllEvents( IEventTarget* ptTarget );

#ifdef _DEBUG
	/**
	 * dumps the registry content
	 */
	virtual String getContent();
#endif

private:
	
	/**
	 * fires a automation added event
	 */
	void fireAutomationAdded( IEventTarget* ptControl );

	/**
	 * fires a automation removed event
	 */
	void fireAutomationRemoved( IEventTarget* ptControl );
};

#endif