#ifndef __MEVENTLISTCOLLECTION
#define __MEVENTLISTCOLLECTION

#pragma warning(disable:4786)

#include <framework/sync/MSynchronize.h>
#include "IEventListCollection.h"

typedef std::vector<IAutomationEventList*> EventListCollection;

class MEventListCollection :
	public MObject,
	public IEventListCollection,
	public IEventTargetListener
{
protected:

	IAutomationTracker* ivPtTracker;

	unsigned int ivLength;
	EventListCollection ivData;
	MListenerList ivListeners;
	
	MCriticalSection ivCriticalSection;

public:

	FULL_RTTI( MEventListCollection, "MEventListCollection" );

	MEventListCollection( IAutomationTracker* ptTracker );
	MEventListCollection( IAutomationTracker* ptTracker, unsigned int length );
	MEventListCollection( MEventListCollection* toClone );

	virtual ~MEventListCollection();

	virtual void triggerEvents( unsigned int index );

	virtual void addAutomationEvent(
		IAutomationEvent* ptEvent,
		IEventTarget* ptTarget,
		unsigned int step );

	virtual void glow( IEventListCollection* ptToAdd );
	virtual IEventListCollection* cut( unsigned int index );
	virtual void scale( unsigned int newLength );

	virtual void setLength( unsigned int length );
	virtual unsigned int getLength();

	virtual IAutomationTracker* getTracker();
	virtual void setTracker( IAutomationTracker* );

	virtual unsigned int getEventListCount();

	virtual IAutomationEventList* getEventList( unsigned int index );
	virtual IAutomationEventList* getEventList( IEventTarget* ptTarget );
	virtual void addAutomationEventList( IAutomationEventList* ptList );

	virtual void removeAutomationEventList( IEventTarget* ptTarget );
	virtual void removeAutomationEventList( unsigned int index );
	virtual void removeAutomationEvent( IAutomationEvent* ptEvent );
	virtual bool hasEventList( IEventTarget* ptTarget );

	virtual IEventListCollection* clone();
	virtual void clear();
	virtual void release();

	virtual MTreeNode* save();
	virtual void load( MTreeNode* node );

	// listeners
	virtual void addListener( IEventListCollectionListener* ptListener );
	virtual void removeListener( IEventListCollectionListener* ptListener );

	// inheritanced from IEventTargetListener
	virtual void onEventTargetChange( IEventTarget* pTarget, Type type );

protected:

	int getIndex( IEventTarget* ptTarget );

	void fireEventAdded( IAutomationEvent* ptEvent );
	void fireEventRemoved( IAutomationEvent* ptEvent );
	void fireAllEventsRemoved();

	void fireEventListAdded( IAutomationEventList* ptList );
	void fireEventListRemoved( IAutomationEventList* ptList );
	void fireReleasing();
};

#endif