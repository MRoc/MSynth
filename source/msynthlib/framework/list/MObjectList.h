#ifndef __MObjectList
#define __MObjectList

#include "../listener/MListenerList.h"

/**
 * defines a object list listener.
 * about this interface a object can be informed
 * about changes in a object list
 */
class IObjectListListener :
	public IListener
{
public:

	/** invoked when a object was added to list */
	virtual void objectAdded( MObject* obj ) = 0;

	/** invoked when a object was removed from list */
	virtual void objectRemoved( MObject* obj ) = 0;

	/** invoked when all objects were removed from list */
	virtual void allObjectsRemoved() = 0;
};

/**
 * implementation of a object list
 */
class MObjectList :
	public MObject
{
public:

	/**
	 * runtime support
	 */
	static MRtti gvRtti;

protected:

	/**
	 * defines the invalid index in this list
	 */
	static const int INVALID_INDEX;

	/**
	 * a list of listeners that will be informed
	 * about every changes in this list
	 */
	MListenerList ivListeners;

	/**
	 * the delegation of this list stuff to stl
	 */
	std::vector<MObject*> ivObjects;

public:

	/**
	 * constructor
	 */
	MObjectList();

	/**
	 * destructor
	 */
	virtual ~MObjectList();

	/**
	 * adds a object at the end of this list
	 */
	virtual void add( MObject* obj );

	/**
	 * adds a object at the specified index
	 */
	virtual void add( MObject* ptObj, unsigned int index );

	/**
	 * returns a object from this list
	 */
	virtual MObject* get( unsigned int index );

	/**
	 * return the amount of items in the list
	 */
	virtual unsigned int getLength();

	/**
	 * removes the object at the specified index from the list
	 */
	virtual MObject* remove( unsigned int index );

	/**
	 * removes and deletes the object at the specified index from the list
	 */
	virtual void deleteObj( unsigned int index );

	/**
	 * removes the specified object from the list
	 */
	virtual MObject* remove( MObject* obj );

	/**
	 * removes and deletes the specified object from the list
	 */
	virtual void deleteObj( MObject* obj );

	/**
	 * returns true if the object in contained in the list
	 */
	virtual bool getContains( MObject* obj );

	/**
	 * returns the index of the specified index in list or INVALID_INDEX
	 */
	virtual int getIndex( MObject* obj );

	/**
	 * removes all objects from the list
	 */
	virtual void removeAll();

	/**
	 * deletes all objects from the list
	 */
	virtual void deleteAll();

	/**
	 * adds a listener to this list
	 */
	virtual void addObjectListListener( IObjectListListener* listener );

	/**
	 * removes a listener from this list
	 */
	virtual void removeObjectListListener( IObjectListListener* listener );

	/**
	 * returns the class name for runtime purpose
	 */
	virtual IRtti* getRtti() const;

	/**
	 * returns an instance of this class
	 */
	static MObject* createInstance();

protected:

	/**
	 * informs all listeners about a adding
	 */
	virtual void fireObjectAdded( MObject* obj );

	/**
	 * informs all listeners about a removing
	 */
	virtual void fireObjectRemoved( MObject* obj );

	/**
	 * informs all listeners about a removing all objects
	 */
	virtual void fireAllObjectsRemoved();
};

#endif