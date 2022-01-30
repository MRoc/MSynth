#include "MSelectableObjectList.h"

/**
 * the runtime type info
 */
MRtti MSelectableObjectList::gvRtti = MRtti( "MSelectableObjectList", MSelectableObjectList::createInstance );

/**
 * constructor
 */
MSelectableObjectList::MSelectableObjectList() :
	ivSelection( INVALID_INDEX )
{
}

/**
 * destructor
 */
MSelectableObjectList::~MSelectableObjectList()
{
}

/**
 * sets the selected index
 */
void MSelectableObjectList::setSelection( int index, bool notify )
{
	ASSERT( index == INVALID_INDEX || (index >= 0 && index < (int)getLength()) );
	if( ivSelection != index )
	{
		ivSelection = index;
		if( notify )
			fireSelectionChange( getSelectedObject() );
	}
}

/**
 * sets the selected object
 */
void MSelectableObjectList::setSelection( MObject* ptObject, bool notify )
{
	setSelection( getIndex( ptObject ), notify );
}

/**
 * removes the object at the specified index from the list
 */
MObject* MSelectableObjectList::remove( unsigned int index )
{
	MObject* ptBack = MObjectList::remove( index );
	if( this->ivSelection >= (int)ivObjects.size() )
		this->setSelection( ((int)ivObjects.size()) - 1 );
	return ptBack;
}

/**
 * sets the selection by string (invoking every object's toString method)
 */
void MSelectableObjectList::setSelection( String str, bool notify )
{
	unsigned int count = this->getLength();
	for( unsigned int i=0;i<count;i++ )
		if( get( i )->toString() == str )
		{
			setSelection( i, notify );
			break;
		}
}

/**
 * returns the selected index
 */
int MSelectableObjectList::getSelection()
{
	return ivSelection;
}

/**
 * returns the selected object
 */
MObject* MSelectableObjectList::getSelectedObject()
{
	MObject* back = 0;
	if( ivSelection != INVALID_INDEX && ivSelection >= 0 && ivSelection < (int) getLength() )
		back = get( ivSelection );
	return back;
}

/**
 * adds a selection listener
 */
void MSelectableObjectList::addSelectionListener( ISelectionListener* ptListener )
{
	ivSelectionListeners.addListener( ptListener );
}

/**
 * removes a selection listener
 */
void MSelectableObjectList::removeSelectionListener( ISelectionListener* ptListener )
{
	ivSelectionListeners.removeListener( ptListener );
}

/**
 * fires selection changed event
 */
void MSelectableObjectList::fireSelectionChange( MObject* ptSelectedObject )
{
	unsigned int count = ivSelectionListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((ISelectionListener*)ivSelectionListeners.getListener( i ))->selectionChanged( ptSelectedObject, this );
}

/**
 * returns the runtime type info
 */
IRtti* MSelectableObjectList::getRtti() const
{
	return &gvRtti;
}

/**
 * queries for a specified interface
 */
void* MSelectableObjectList::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return ((MSelectableObjectList*)this);
	else
		return MObject::getInterface( className );
}

/**
 * creates an instance of this class
 */
MObject* MSelectableObjectList::createInstance()
{
	return new MSelectableObjectList();
}