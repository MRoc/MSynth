#ifndef __IEVENTLISTCOLLECTION
#define __IEVENTLISTCOLLECTION

#include "MAutomationEventList.h"

class IEventListCollectionListener :
	public IListener
{
public:

	virtual void eventAdded( IAutomationEvent* ptEvent ) = 0;
	virtual void eventRemoved( IAutomationEvent* ptEvent ) = 0;
	virtual void allEventsRemoved() = 0;

	virtual void eventListAdded( IAutomationEventList* ptList ) = 0;
	virtual void eventListRemoved( IAutomationEventList* ptList ) = 0;

	virtual void eventListCollectionReleasing() = 0;
};

class IEventListCollection :
	public ISerializeable
{
public:

	virtual void triggerEvents( unsigned int index ) = 0;

	virtual void addAutomationEvent(
		IAutomationEvent* ptEvent,
		IEventTarget* ptTarget,
		unsigned int step ) = 0;

	virtual void glow( IEventListCollection* ptToAdd ) = 0;
	virtual IEventListCollection* cut( unsigned int index ) = 0;
	virtual void scale( unsigned int newLength ) = 0;

	virtual void setLength( unsigned int length ) = 0;
	virtual unsigned int getLength() = 0;

	virtual void addAutomationEventList( IAutomationEventList* ptList ) = 0;
	virtual IAutomationEventList* getEventList( IEventTarget* ptTarget ) = 0;
	virtual bool hasEventList( IEventTarget* ptTarget ) = 0;
	virtual void removeAutomationEventList( IEventTarget* ptTarget ) = 0;
	virtual void removeAutomationEvent( IAutomationEvent* ptEvent ) = 0;

	virtual IAutomationTracker* getTracker() = 0;
	virtual void setTracker( IAutomationTracker* ptTracker ) = 0;

	virtual void clear() = 0;
	virtual void release() = 0;

	virtual IEventListCollection* clone() = 0;

	virtual void addListener( IEventListCollectionListener* ptListener ) = 0;
	virtual void removeListener( IEventListCollectionListener* ptListener ) = 0;
};

#endif