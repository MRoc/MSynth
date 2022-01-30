#include "MDCControlSelectionManager.h"

MDCControlSelectionManager::MDCControlSelectionManager()
{
}

MDCControlSelectionManager::~MDCControlSelectionManager()
{
	ivArPtDCControlSelected.removeAll();
}

bool MDCControlSelectionManager::hasSelection()
{
	return ivArPtDCControlSelected.getLength() > 0;
}

void MDCControlSelectionManager::singleSelection( MDCControl *control )
{
	ASSERT( control );
	resetSelection();
	control->setSelected( true );
	ivArPtDCControlSelected.add( control );
	fireSelectionChanged();
}

void MDCControlSelectionManager::addToSelection( MDCControl *control )
{
	ASSERT( control );
	ASSERT( ! isInSelection( control ) );

	control->setSelected( true );
	ivArPtDCControlSelected.add( control );
	fireSelectionChanged();
}

bool MDCControlSelectionManager::isInSelection( MDCControl *control )
{
	ASSERT( control );
	return this->ivArPtDCControlSelected.getContains( control );
}

int	MDCControlSelectionManager::getIndexOfControl( MDCControl *control )
{
	ASSERT( control );
	return this->ivArPtDCControlSelected.getIndex( control );
}

void MDCControlSelectionManager::removeFromSelection( unsigned int index )
{
	ASSERT( index >= 0 && index < this->ivArPtDCControlSelected.getLength() );
	getSelectionAt( index )->setSelected( false );
	ivArPtDCControlSelected.remove( index );
	fireSelectionChanged();
}

void MDCControlSelectionManager::removeFromSelection( MDCControl *control )
{
	ASSERT( control );
	ASSERT( isInSelection( control ) );
	ivArPtDCControlSelected.remove( control );
	fireSelectionChanged();
}

void MDCControlSelectionManager::resetSelection()
{
	unsigned int count = ivArPtDCControlSelected.getLength(); 
	for( unsigned int i=0;i<count;i++ )
		getSelectionAt( i )->setSelected( false );

	ivArPtDCControlSelected.removeAll();
	fireSelectionChanged();
}

unsigned int MDCControlSelectionManager::getSelectionCount()
{
	return ivArPtDCControlSelected.getLength();
}

MDCControl *MDCControlSelectionManager::getSelectionAt( unsigned int index )
{
	return (MDCControl*) ivArPtDCControlSelected.get( index );
}

void MDCControlSelectionManager::addListener( ISelectionListener* pListener )
{
	ivListeners.addListener( pListener );
}

void MDCControlSelectionManager::removeListener( ISelectionListener* pListener )
{
	ivListeners.removeListener( pListener );
}

void MDCControlSelectionManager::fireSelectionChanged()
{
	unsigned int count = ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
	{
		ISelectionListener* pListener = (ISelectionListener*) ivListeners.getListener( i );
		pListener->selectionChanged( 0, 0 );
	}
}