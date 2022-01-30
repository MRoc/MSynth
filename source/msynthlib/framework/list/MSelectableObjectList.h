#ifndef __MSelectableObjectList
#define __MSelectableObjectList

#include "MObjectList.h"

/**
 * defines a selection listener
 */
class ISelectionListener :
	public IListener
{
public:

	/**
	 * invoked when selection changed
	 * newSelection = the new selected object
	 * ptSource = the object list
	 */
	virtual void selectionChanged( MObject* newSelection, MObject* ptSource ) = 0;
};

/**
 * defines a object list with a selection mechanism
 */
class MSelectableObjectList :
	public MObjectList
{
public:

	/**
	 * runtime type info
	 */
	static MRtti gvRtti;

protected:

	/**
	 * stores the current selection, can be INVALID_INDEX
	 */
	int ivSelection;

	/**
	 * stores the selection listeners
	 */
	MListenerList ivSelectionListeners;

public:

	/**
	 * constructor
	 */
	MSelectableObjectList();

	/**
	 * destructor
	 */
	virtual ~MSelectableObjectList();

	/**
	 * sets the selected index
	 */
	virtual void setSelection( int index, bool notify = true );

	/**
	 * sets the selected index by invoking the toString method
	 */
	virtual void setSelection( String str, bool notify = true );

	/**
	 * sets the given object as selected
	 */
	virtual void setSelection( MObject* ptObject, bool notify = true );

	/**
	 * returns the selected index
	 */
	virtual int getSelection();

	/**
	 * returns the selected object
	 */
	virtual MObject* getSelectedObject();

	/**
	 * removes the object at the specified index from the list
	 */
	virtual MObject* remove( unsigned int index );

	/**
	 * adds a selection listener
	 */
	virtual void addSelectionListener( ISelectionListener* ptListener );

	/**
	 * removes the selection listener
	 */
	virtual void removeSelectionListener( ISelectionListener* ptListener );

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * queries for a specified interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * creates a instance of this class
	 */
	static MObject* createInstance();

protected:

	/**
	 * fires a selection changed event
	 */
	virtual void fireSelectionChange( MObject* ptSelectedObject );
};

#endif