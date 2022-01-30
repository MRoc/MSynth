#include "MEventListCollection.h"

INIT_RTTI( MEventListCollection, "MEventListCollection" );

MEventListCollection::MEventListCollection( IAutomationTracker* ptTracker ) :
	ivLength( 16 ),
	ivPtTracker( ptTracker )
{
}

MEventListCollection::MEventListCollection(
	IAutomationTracker* ptTracker, unsigned int length ) :
	ivLength( length ),
	ivPtTracker( ptTracker )
{
}

MEventListCollection::MEventListCollection( MEventListCollection* toClone ) :
	ivLength( toClone->getLength() ),
	ivPtTracker( toClone->getTracker() )
{
	unsigned int count = toClone->getEventListCount();
	for( unsigned int i=0;i<count;i++ )
		addAutomationEventList( toClone->getEventList( i )->clone() );
}

MEventListCollection::~MEventListCollection()
{
	fireReleasing();
	clear();
}

void MEventListCollection::addAutomationEvent(
	IAutomationEvent* ptEvent,
	IEventTarget* ptTarget,
	unsigned int step )
{
	ASSERT( getEventList( ptTarget ) );

	MSynchronize cls( &ivCriticalSection );

	getEventList( ptTarget )->setItem( ptEvent, step );
	fireEventAdded( ptEvent );
	TRACE( "succeeded!]\n" );
}

unsigned int MEventListCollection::getEventListCount()
{
	return ivData.size();
}

IAutomationEventList* MEventListCollection::getEventList( unsigned int index )
{
	return ivData[ index ];
}

IAutomationEventList* MEventListCollection::getEventList( IEventTarget* ptTarget ) 
{
	IAutomationEventList* back = 0;
	int index = getIndex( ptTarget );
	if( index == -1 )
	{
		back = new MAutomationEventList( getLength(), ptTarget );
		addAutomationEventList( back );
	}
	else
		back = ivData[ index ];
	return back;
}

void MEventListCollection::addAutomationEventList( IAutomationEventList* ptList )
{
	TRACE( "[MEventListCollection::addAutomationEventList... " );
	MSynchronize cls( &ivCriticalSection );

	IEventTarget* ptTarget = ptList->getTarget();
	if( ! MTargetRegistry::getInstance()->isInAutomation( ptTarget ) )
		MTargetRegistry::getInstance()->addToAutomation( ptTarget );
	ivData.push_back( ptList );
	ptTarget->addListener( this );
	fireEventListAdded( ptList );
	TRACE( "succeeded!]\n" );
}

bool MEventListCollection::hasEventList( IEventTarget* ptTarget )
{
	bool back = false;
	unsigned int count = ivData.size();
	for( unsigned int i=0;i<count;i++ )
		if( ivData[ i ]->getTarget() == ptTarget )
		{
			back = true;
			break;
		}
	return back;
}

void MEventListCollection::removeAutomationEventList( IEventTarget* ptTarget )
{
	MSynchronize cls( &ivCriticalSection );

	int index = getIndex( ptTarget );
	if( index != -1 )
		removeAutomationEventList( index );
}

void MEventListCollection::removeAutomationEventList( unsigned int index )
{
	ASSERT( index < ivData.size() );
	IAutomationEventList* ptList = ivData[ index ];
	ptList->getTarget()->removeListener( this );
	ivData.erase( ivData.begin() + index );
	fireEventListRemoved( ptList );
	ptList->release();
}

void MEventListCollection::removeAutomationEvent( IAutomationEvent* ptEvent )
{
	MSynchronize cls( &ivCriticalSection );

	unsigned int count = getEventListCount();
	for( unsigned int i=0;i<count;i++ )
		getEventList( i )->removeItem( ptEvent );
}

unsigned int MEventListCollection::getLength()
{
	return ivLength;
}

void MEventListCollection::setLength( unsigned int length )
{
	MSynchronize cls( &ivCriticalSection );

	ivLength = length;
	unsigned int count = ivData.size();
	for( unsigned int i=0;i<count;i++ )
		ivData[ i ]->setLength( ivLength );
}

void MEventListCollection::glow( IEventListCollection* ptToAdd )
{
	MSynchronize cls( &ivCriticalSection );

	unsigned int count = getEventListCount();
	for( unsigned int i=0;i<count;i++ )
	{
		IAutomationEventList* thisPtList = getEventList( i );
		IAutomationEventList* ptList = ptToAdd->getEventList(
			thisPtList->getTarget() );
		ASSERT( ptList );
		thisPtList->glow( ptList );
	}
	ivLength += ptToAdd->getLength();
}

IEventListCollection* MEventListCollection::cut( unsigned int index )
{
	ASSERT( index < ivLength );

	MSynchronize cls( &ivCriticalSection );

	IEventListCollection* back = 0;
	back = new MEventListCollection( ivPtTracker, ivLength - index );

	unsigned int count = getEventListCount();
	for( unsigned int i=0;i<count;i++ )
		back->addAutomationEventList(
			getEventList( i )->cut( index ) );
	ivLength = index;

	return back;
}

void MEventListCollection::scale( unsigned int newLength )
{
	TRACE( "[MEventListCollection::scale... " );
	MSynchronize cls( &ivCriticalSection );

	unsigned int count = ivData.size();
	for( unsigned int i=0;i<count;i++ )
		ivData[ i ]->scale( newLength );
	ivLength = newLength;
	TRACE( "succeeded!]\n" );
}

void MEventListCollection::triggerEvents( unsigned int index )
{
	MSynchronize cls( &ivCriticalSection );

	unsigned int count = getEventListCount();
	for( unsigned int i=0;i<count;i++ )
	{
		IAutomationEventList* ptList = getEventList( i );
		//ASSERT( ptList );
		IAutomationEvent* ptEvent = ptList->getItem( index );
		if( ptEvent )
			ptList->getTarget()->postEvent( ptEvent );
	}
}

IEventListCollection* MEventListCollection::clone()
{
	return new MEventListCollection( this );
}

void MEventListCollection::clear()
{
	unsigned int count = getEventListCount();
	for( unsigned int i=0;i<count;i++ )
	{
		ivData[ i ]->getTarget()->removeListener( this );
		ivData[ i ]->release();
	}
	ivData.clear();
}

void MEventListCollection::release()
{
	delete this;
}

MTreeNode* MEventListCollection::save()
{
	MTreeNode* ptBack = new MTreeNode( "elc" );
	unsigned int count = ivData.size();
	for( unsigned int i=0;i<count;i++ )
		ptBack->addChild( ivData[ i ]->save() );
	return ptBack;
}

void MEventListCollection::load( MTreeNode* node )
{
	MTreeNodeIterator iter = node->getIterator();
	while( iter.hasNext() )
	{
		MTreeNode* child = iter.nextTreeNode();
		String nodeName = child->getName();
		if( strcmp( nodeName, "automationeventlist" ) == 0 )
		{
			MAutomationEventList* ptNewList = new MAutomationEventList( getLength() );
			ptNewList->load( child );
			addAutomationEventList( ptNewList );
		}
	}
}

// listeners
void MEventListCollection::addListener( IEventListCollectionListener* ptListener )
{
	ivListeners.addListener( ptListener );
}

void MEventListCollection::removeListener( IEventListCollectionListener* ptListener )
{
	ivListeners.removeListener( ptListener );
}

// inheritanced from IFinalizeListener
void MEventListCollection::onEventTargetChange( IEventTarget* pTarget, Type type )
{
	if( type == ET_TERMINATING )
		removeAutomationEventList( pTarget );
}

int MEventListCollection::getIndex( IEventTarget* ptTarget )
{
	int back = -1;
	unsigned int count = getEventListCount();
		for( unsigned int i=0;i<count;i++ )
			if( ivData[ i ]->getTarget() == ptTarget )
			{
				back = i;
				break;
			}
		return back;
}

void MEventListCollection::fireEventAdded( IAutomationEvent* ptEvent )
{
	unsigned int count = ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IEventListCollectionListener*)ivListeners.getListener(i))->eventAdded( ptEvent );
}

void MEventListCollection::fireEventRemoved( IAutomationEvent* ptEvent )
{
	unsigned int count = ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IEventListCollectionListener*)ivListeners.getListener(i))->eventRemoved( ptEvent );
}

void MEventListCollection::fireAllEventsRemoved()
{
	unsigned int count = ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IEventListCollectionListener*)ivListeners.getListener(i))->allEventsRemoved();
}

void MEventListCollection::fireEventListAdded( IAutomationEventList* ptList )
{
	unsigned int count = ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IEventListCollectionListener*)ivListeners.getListener(i))->eventListAdded( ptList );
}

void MEventListCollection::fireEventListRemoved( IAutomationEventList* ptList )
{
	unsigned int count = ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IEventListCollectionListener*)ivListeners.getListener(i))->eventListRemoved( ptList );
}

void MEventListCollection::fireReleasing()
{
// this assertion should prevent endless loop if an listener does not remove itself
#ifdef _DEBUG
	unsigned int max = 100;
	unsigned int iterations = 0;
#endif
	while( ivListeners.getListenerCount() > 0 )
	{
		((IEventListCollectionListener*)ivListeners.getListener(0))->eventListCollectionReleasing();
#ifdef _DEBUG
		ASSERT( (iterations++) < max );
#endif
	}
}

IAutomationTracker* MEventListCollection::getTracker()
{
	return ivPtTracker;
}

void MEventListCollection::setTracker( IAutomationTracker* ptTracker )
{
	ivPtTracker = ptTracker;
}