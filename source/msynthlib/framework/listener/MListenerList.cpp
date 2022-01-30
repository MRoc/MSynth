#include "MListenerList.h"
#include <framework/io/MLogger.h>

/**
 * runtime support
 */
MRtti MListenerList::gvRtti = MRtti( "MListenerList", MListenerList::createInstance );

/**
 * the constant for a invalid index, -1
 */
const int MListenerList::INVALID_INDEX = -1;

/**
 * constructor
 */
MListenerList::MListenerList() :
	ivLocked( false )
{
}

/**
 * destructor
 */
MListenerList::~MListenerList()
{
	clear();
}

/**
 * removes all registered listeners
 */
void MListenerList::clear()
{
	if( ivListeners.size() > 0 )
		MLogger::logWarning( "WARNING: clearing MListenerList with %i listeners left!\n", ivListeners.size() );

	ivListeners.clear();
	ivListenersToRemove.clear();
}

/**
 * adds a IListener to the listener list
 */
void MListenerList::addListener( IListener* ptListener )
{
#ifdef _DEBUG
	if( hasListener( ptListener ) )
	{
		MLogger::logError(
			"MListenerList::addListener: adding listener %s", ((MObject*)ptListener->getInterface( "MObject" ))->getRtti()->getClassName() );
	}
#endif
	ASSERT( ptListener );
	ASSERT( ! hasListener( ptListener ) );
	ivListeners.push_back( ptListener );
}

/**
 * removes a IListener from the listener list.
 * if the listenerlist is locked, the listener
 * is added into the removelist and really removed
 * the the listenerlist is unlocked.
 */
void MListenerList::removeListener( IListener* ptListener )
{
	ASSERT( ptListener );
	ASSERT( hasListener( ptListener ) );

	if( ivLocked )
		ivListenersToRemove.push_back( ptListener );
	else
		ivListeners.erase( ivListeners.begin() + (unsigned int)getIndex( ptListener ) );
}

/**
 * returns the number of registered listeners
 */
unsigned int MListenerList::getListenerCount()
{
	return (unsigned int) ivListeners.size();
}

/**
 * returns the listener at the specified index
 */
IListener* MListenerList::getListener( unsigned int index )
{
	ASSERT( index < ivListeners.size() );
	return ivListeners[ index ];
}

/**
 * returns true if the given listener is registered
 */
bool MListenerList::hasListener( IListener* ptListener )
{
	return getIndex( ptListener ) != INVALID_INDEX;
}

/**
 * returns the index of the given index if registered,
 * if not INVALID_INDEX
 */
int MListenerList::getIndex( IListener* ptListener )
{
	int back = INVALID_INDEX;
	for( unsigned int i=0;i<ivListeners.size();i++ )
		if( ivListeners[ i ] == ptListener )
		{
			back = i;
			break;
		}
	return back;
}

/**
 * returns the class name for runtime purpose
 */
IRtti* MListenerList::getRtti() const
{
	return &gvRtti;
}

/**
 * creates an instance of a MListenerList
 */
MObject* MListenerList::createInstance()
{
	return new MListenerList();
}

/**
 * locks the listener list. no listeners are
 * removed during the listenerlist is locked.
 */
void MListenerList::lock()
{
	if( ! ivLocked )
		ivLocked = true;
	else
		MLogger::logError( "MListenerList::lock: listenerlist allready locked!" );
}

/**
 * unlocks the listenerlist. all listeners
 * contained in the toRemove list are 
 * removed from the list.
 */
void MListenerList::unlock()
{
	if( ivLocked )
	{
		ivLocked = false;
		while( ivListenersToRemove.size() )
		{
			removeListener( *ivListenersToRemove.begin() );
			ivListenersToRemove.erase( ivListenersToRemove.begin() );
		}
	}
	else
		MLogger::logError( "MListenerList::unlock: listenerlist is not locked!" );
}