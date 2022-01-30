#ifndef __MMultiToggleButtonControl
#define __MMultiToggleButtonControl

#include <framework/MObject.h>

#include "MToggleButton.h"

class MMultiToggleButtonControl :
	public MObject,
	public IActionListener
{
public:

	/** runtime type info */
	static MRtti gvRtti;

protected:

	/** the action listeners this control stores */
	MListenerList ivActionListener;

	/** list of all togglebuttons */
	std::vector<MToggleButton*> ivToggleButtons;

	/** the current button */
	MToggleButton* ivPtCurrentButton;

public:

	/** constructor */
	MMultiToggleButtonControl();

	/** destructor */
	virtual ~MMultiToggleButtonControl();

	/** sets the currently pressed button */
	virtual void setCurrentButton( MToggleButton* ptButton );

	/** adds a toggle button to this control */
	virtual void addToggleButton( MToggleButton* ptButton );

	/** removes a toggle button from this control */
	virtual void removeToggleButton( MToggleButton* ptButton );

	/** incoming events from the toggle buttons */
	virtual void onActionPerformed( void* ptSrc );

	/** registers a actionlistener */
	void addActionListener( IActionListener* ptListener );

	/** unregisters a actionlistener */
	void removeActionListener( IActionListener* ptListener );

	/** returns the index of the toggle button in list */
	virtual int getIndex( MToggleButton* ptButton );

	/** returns the current selected button */
	virtual MToggleButton* getCurrentButton();

	/** returns the runtime type info */
	virtual IRtti* getRtti() const;

	/** query superclass */
	virtual void* getInterface( const String &className ) const;

protected:

	/** fires a action event */
	void fireActionPerformed();

	/** returns true if the toggle button is registered */
	virtual bool hasToggleButton( MToggleButton* ptButton );
};

#endif