#ifndef __MDefaultControl
#define __MDefaultControl

#include <framework/MObject.h>
#include <framework/listener/MListenerList.h>
#include <framework/sync/MSynchronize.h>
#include "IControl.h"

/**
 * MDefaultControl (C)2001-2003 MRoc hifiShock
 *
 * base class of all controls, has base event mechanism
 */
class MDefaultControl :
	public MObject,
	public IControl
{
protected:

	/**
	 * the control's name like displayed to the used
	 */
	String ivName;

	/**
	 * a list of listeners to be informed about changes
	 */
	MListenerList ivControlListeners;

	/**
	 * a sync object
	 */
	MCriticalSection ivSync;

public:

	/**
	 * constructor
	 */
	MDefaultControl( String name );

	/**
	 * destructor
	 */
	virtual ~MDefaultControl();

	/**
	 * resets the control to default value
	 */
	virtual void resetToDefault();

	/**
	 * adds a control listener to be informed
	 * about control changes and termination
	 */
	virtual void addControlListener( IControlListener* ptListener );

	/**
	 * removes a presious installed listener
	 */
	virtual void removeControlListener( IControlListener* ptListener );

	/**
	 * returns the id of a view that is able to display this control
	 */
	virtual unsigned int getViewID();

	/**
	 * returns the control's name
	 */
	virtual String getName();

	/**
	 * sets the control's name
	 */
	virtual void setName( String name );

	/**
	 * query for interface
	 */
	virtual void* getInterface( const String &className ) const;

protected:
  
	/**
	 * fires a value changed event
	 */
	virtual void fireValueChanged();

	/**
	 * fires a termination event
	 */
	virtual void fireFinalisation();

	/**
	 * fires the given event
	 */
	virtual void fireEvent( MControlListenerEvent* pEvent );
};

#endif

