#ifndef __AUTOMATIONCONTEXT
#define __AUTOMATIONCONTEXT

#include "IEventTarget.h"
#include "IAutomationTracker.h"

/** 
 * defines a object which has
 * a IEventTarget and a IAutomationTracker 
 *
 * the target is used to fire events to.
 * the tracker is used for recording events.
 */
class IAutomationContext
{
public:

	/** 
	 * returns the event target used to fire events
	 */
	virtual IEventTarget* getTarget() = 0;

	/**
	 * returns the automation tracker used to record events
	 */
	virtual IAutomationTracker* getTracker() = 0;

	/**
	 * frees the object
	 */
	virtual void release() = 0;
};

/**
 * implementation of a IAutomationContext
 */
class MAutomationContext : public IAutomationContext
{
protected:

	/**
	 * the event target used for firing events
	 */
	IEventTarget* ivPtEventTarget;

	/**
	 * the tracker used to record events
	 */
	IAutomationTracker* ivPtTracker;

public:

	/**
	 * constructor
	 */
	MAutomationContext( IEventTarget* ptControl, IAutomationTracker* ptTracker ) :
		ivPtEventTarget( ptControl ),
		ivPtTracker( ptTracker )
	{
	}

	/**
	 * constructor 2
	 */
	MAutomationContext( IAutomationContext* ptAutomationContext ) :
		ivPtEventTarget( ptAutomationContext->getTarget() ),
		ivPtTracker( ptAutomationContext->getTracker() )
	{
	}

	/** 
	 * destructor
	 */
	virtual ~MAutomationContext()
	{
	}

	/** 
	 * returns the event target used to fire events
	 * inheritanced from IAutomationContext
	 */
	virtual IEventTarget* getTarget()
	{
		return ivPtEventTarget;
	}

	/**
	 * returns the automation tracker used to record events
	 * inheritanced from IAutomationContext
	 */
	virtual IAutomationTracker* getTracker()
	{
		return ivPtTracker;
	}

	/**
	 * frees the object
	 * inheritanced from IAutomationContext
	 */
	virtual void release()
	{
		delete this;
	}
};

#endif