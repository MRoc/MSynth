#ifndef __MListenerList
#define __MListenerList

#include "IListener.h"
#include "../MObject.h"

/**
 * implementation of a list containing some IListeners
 */
class MListenerList :
	public MObject
{
public:

	/**
	 * runtime support
	 */
	static MRtti gvRtti;

protected:

	/**
	 * the constant for a invalid index, -1
	 */
	static const int INVALID_INDEX;

	/**
	 * delegated collection storing the listeners
	 */
	std::vector<IListener*> ivListeners;

	/**
	 * collection storing the listeners to be removed
	 */
	std::vector<IListener*> ivListenersToRemove;

	/**
	 * this flag is set while a listenerlist is iterated.
	 * while this flag is true, no listener can be removed
	 * from the listenener list
	 */
	bool ivLocked;

public:

	/**
	 * constructor
	 */
	MListenerList();

	/**
	 * destructor
	 */
	virtual ~MListenerList();

	/**
	 * removes all registered listeners
	 */
	virtual void clear();

	/**
	 * adds a IListener to the listener list
	 */
	virtual void addListener( IListener* ptListener );

	/**
	 * removes a IListener from the listener list
	 */
	virtual void removeListener( IListener* ptListener );

	/**
	 * returns the number of registered listeners
	 */
	virtual unsigned int getListenerCount();

	/**
	 * returns the listener at the specified index
	 */
	virtual IListener* getListener( unsigned int index );

	/**
	 * returns true if the given listener is registered
	 */
	virtual bool hasListener( IListener* ptListener );

	/**
	 * returns the class name for runtime purpose
	 */
	virtual IRtti* getRtti() const;

	/**
	 * creates an instance of a MListenerList
	 */
	static MObject* createInstance();

	/**
	 * locks the listener list. no listeners are
	 * removed during the listenerlist is locked.
	 */
	virtual void lock();

	/**
	 * unlocks the listenerlist. all listeners
	 * contained in the toRemove list are 
	 * removed from the list.
	 */
	virtual void unlock();

protected:

	/**
	 * returns the index of the given index if registered,
	 * if not INVALID_INDEX
	 */
	virtual int getIndex( IListener* ptListener );
};

#endif