#include "MObjectList.h"

/**
 * runtime support
 */
MRtti MObjectList::gvRtti = MRtti( "MObjectList", MObjectList::createInstance );

/**
 * defines the invalid index in this list
 */
const int MObjectList::INVALID_INDEX = -1;

/**
 * constructor
 */
MObjectList::MObjectList()
{
}

/**
 * destructor
 */
MObjectList::~MObjectList()
{
	ivListeners.clear();
	removeAll();
}

/**
 * adds a object to this list
 */
void MObjectList::add( MObject* obj )
{
	ASSERT( ! this->getContains( obj ) );
	ivObjects.push_back( obj );
	this->fireObjectAdded( obj );
}

/**
 * adds a object at the specified index
 */
void MObjectList::add( MObject* ptObj, unsigned int index )
{
	ivObjects.insert( ivObjects.begin() + index, ptObj );
	this->fireObjectAdded( ptObj );
}

/**
 * returns a object from this list
 */
MObject* MObjectList::get( unsigned int index )
{
	ASSERT( index < ivObjects.size() );
	return ivObjects.at( index );
}

/**
 * return the amount of items in the list
 */
unsigned int MObjectList::getLength()
{
	return (unsigned int) ivObjects.size();
}

/**
 * removes the specified object from the list
 */
MObject* MObjectList::remove( MObject* obj )
{
	return remove( getIndex( obj ) );
}

/**
 * removes and deletes the object from the list
 */
void MObjectList::deleteObj( MObject* obj )
{
	MObject* o = remove( obj );
	if( o )
		delete o;
}

/**
 * returns the index of the specified index in list or INVALID_INDEX
 */
int MObjectList::getIndex( MObject* obj )
{
	int back = INVALID_INDEX;
	unsigned int count = (unsigned int) ivObjects.size();
	for( unsigned int i=0;i<count;i++ )
		if( obj == ivObjects.at( i ) &&
			obj->toString() == ivObjects.at( i )->toString() )
		{
			back = i;
			break;
		}
	return back;
}

/**
 * removes the object at the specified index from the list
 */
MObject* MObjectList::remove( unsigned int index )
{
	MObject* back = 0;
	ASSERT( index < ivObjects.size() );

	back = ivObjects.at( index );
	ivObjects.erase( ivObjects.begin() + index );
	fireObjectRemoved( back );
	return back;
}

/**
 * removes and deletes the object at the specified index from the list
 */
void MObjectList::deleteObj( unsigned int index )
{
	MObject* o = remove( index );
	if( o )
		delete o;
}

/**
 * returns true if the object in contained in the list
 */
bool MObjectList::getContains( MObject* obj )
{
	return getIndex( obj ) != INVALID_INDEX;
}

/**
 * removes all objects from the list
 */
void MObjectList::removeAll()
{
	if( ivObjects.size() > 0 )
	{
		ivObjects.clear();
		this->fireAllObjectsRemoved();
	}
}

/**
 * deletes all objects from the list
 */
void MObjectList::deleteAll()
{
	unsigned int count = (unsigned int) ivObjects.size();
	for( unsigned int i=0;i<count;i++ )
	{
		MObject* obj = ivObjects.at( i );
		ASSERT( obj );
		delete obj;
	}
	removeAll();
}

/**
 * adds a listener to this list
 */
void MObjectList::addObjectListListener( IObjectListListener* listener )
{
	ivListeners.addListener( listener );
}

/**
 * removes a listener from this list
 */
void MObjectList::removeObjectListListener( IObjectListListener* listener )
{
	ivListeners.removeListener( listener );
}

/**
 * informs all listeners about a adding
 */
void MObjectList::fireObjectAdded( MObject* obj )
{
	unsigned int count = ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IObjectListListener*)ivListeners.getListener(i))->objectAdded( obj );
}

/**
 * informs all listeners about a removing
 */
void MObjectList::fireObjectRemoved( MObject* obj )
{
	unsigned int count = ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IObjectListListener*)ivListeners.getListener(i))->objectRemoved( obj );
}

/**
 * informs all listeners about a removing all objects
 */
void MObjectList::fireAllObjectsRemoved()
{
	unsigned int count = ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IObjectListListener*)ivListeners.getListener(i))->allObjectsRemoved();
}

/**
 * returns the class name for runtime purpose
 */
IRtti* MObjectList::getRtti() const
{
	return &gvRtti;
}

/**
 * returns an instance of this class
 */
MObject* MObjectList::createInstance()
{
	return new MObjectList();
}