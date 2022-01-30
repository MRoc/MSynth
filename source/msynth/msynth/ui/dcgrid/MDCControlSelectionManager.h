#ifndef __MDCControlSelectionManager
#define __MDCControlSelectionManager

#include <framework/list/MObjectList.h>
#include <framework/list/MSelectableObjectList.h>

#include "MDCControl.h"

class MDCControlSelectionManager
{
protected:

	MObjectList ivArPtDCControlSelected;
	MListenerList ivListeners;

public:

	MDCControlSelectionManager();
	virtual ~MDCControlSelectionManager();

	void			singleSelection( MDCControl *control );
	void			addToSelection( MDCControl *control );
	bool			isInSelection( MDCControl *control );
	void			removeFromSelection( MDCControl *control );
	void			removeFromSelection( unsigned int index );
	int				getIndexOfControl( MDCControl *control );

	bool			hasSelection();

	void			resetSelection();

	unsigned int	getSelectionCount();
	MDCControl*		getSelectionAt( unsigned int index );

	void			addListener( ISelectionListener* pListener );
	void			removeListener( ISelectionListener* pListener );

protected:

	void			fireSelectionChanged();
};

class MDCList :
	private MObjectList
{
protected:

	MDCControlComparator* ivComparator;
	
public:

	MDCList() :
		ivComparator( 0 )
	{
	};

	MDCList( MDCControlComparator* ptComparator ) :
		ivComparator( ptComparator )
	{
	};

	MDCList( MDCControlComparator* ptComparator, MDCControlSelectionManager* ptSelectionManager ) :
		ivComparator( ptComparator )
	{
		unsigned int count = ptSelectionManager->getSelectionCount();
		for( unsigned int i=0;i<count;i++ )
			addControl( ptSelectionManager->getSelectionAt( i ) );
	};

	virtual ~MDCList()
	{
		MObjectList::removeAll();
	};

	void addControl( MDCControl* ptControl )
	{
		ASSERT( ivComparator );
		ASSERT( ptControl );
		ASSERT( ! MObjectList::getContains( ptControl ) );
		insert( ptControl );	
	};

	MDCControl* getControl( unsigned int index )
	{
		ASSERT( index < MObjectList::getLength() );
		return (MDCControl*) MObjectList::get( index );
	};

	unsigned int getControlCount()
	{
		return MObjectList::getLength();
	};

protected:

	void insert( MDCControl* ptControl )
	{
		unsigned int count = MObjectList::getLength();
		for( unsigned int i=0;i<count;i++ )
			if( ivComparator->compare( ptControl, getControl( i ) ) < 0 )
			{
				MObjectList::add( ptControl, i );
				break;
			}
		if( i == count )
			MObjectList::add( ptControl );
	};

};

#endif