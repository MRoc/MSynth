#include "MDefaultControl.h"

/**
 * constructor
 */
MDefaultControl::MDefaultControl( String name ) :
	ivName( name )
{
}

/**
 * destructor
 */
MDefaultControl::~MDefaultControl()
{
	fireFinalisation();
	ivControlListeners.clear();
}

/**
 * returns the control's name
 */
String MDefaultControl::getName()
{
	return ivName;
}

/**
 * adds a control listener to be informed
 * about control changes and termination
 */
void MDefaultControl::addControlListener( IControlListener* ptListener )
{
	MSynchronize m(&ivSync);
	ivControlListeners.addListener( ptListener );
}

/**
 * removes a presious installed listener
 */
void MDefaultControl::removeControlListener( IControlListener* ptListener )
{
	MSynchronize m(&ivSync);
	ivControlListeners.removeListener( ptListener );
}

/**
 * returns the id of a view that is able to display this control
 */
unsigned int MDefaultControl::getViewID()
{
	return DESC_TYPE_UNDEF;
}

/**
 * sets the control's name
 */
void MDefaultControl::setName( String name )
{
	ivName = name;
}

/**
 * resets the control to default value
 */
void MDefaultControl::resetToDefault()
{
}

/**
 * fires a value changed event
 */
void MDefaultControl::fireValueChanged()
{
	fireEvent( &MControlListenerEvent( this, MControlListenerEvent::CONTROL_EVENT_VALUE_CHANGED ) );
}

/**
 * fires a termination event
 */
void MDefaultControl::fireFinalisation()
{
	fireEvent( &MControlListenerEvent( this, MControlListenerEvent::CONTROL_TERMINATING ) );
}

/**
 * fires the given event
 */
void MDefaultControl::fireEvent( MControlListenerEvent* pEvent )
{
	MSynchronize m(&ivSync);
	ivControlListeners.lock();
	unsigned int count = ivControlListeners.getListenerCount();
	for( unsigned int j=0;j<count;j++ )
		((IControlListener*)ivControlListeners.getListener(j))->valueChanged( pEvent );
	ivControlListeners.unlock();
}

/**
 * query for interface
 */
void* MDefaultControl::getInterface( const String &className ) const
{
	if( className == "MDefaultControl" )
		return (void*) ((MDefaultControl*)this);
	else if( className == "IControl" )
		return (void*) ((IControl*)this);
	else
		return MObject::getInterface( className );
}