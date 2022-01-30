#include "MMultiToggleButtonControl.h"


/** runtime type info */
MRtti MMultiToggleButtonControl::gvRtti = MRtti( "MMultiToggleButtonControl", 0 );

/** constructor */
MMultiToggleButtonControl::MMultiToggleButtonControl() :
	MObject(),
	ivPtCurrentButton( 0 )
{
}

/** destructor */
MMultiToggleButtonControl::~MMultiToggleButtonControl()
{
	ivActionListener.clear();
	ivToggleButtons.clear();
}

/** sets the currently pressed button */
void MMultiToggleButtonControl::setCurrentButton( MToggleButton* ptButton )
{
	ASSERT( hasToggleButton( ptButton ) );
	ptButton->setValue( true );
	this->onActionPerformed( (void*) ptButton );
}

/** adds a toggle button to this control */
void MMultiToggleButtonControl::addToggleButton( MToggleButton* ptButton )
{
	ASSERT( ptButton );
	ASSERT( ! hasToggleButton( ptButton ) );
	this->ivToggleButtons.push_back( ptButton );
	ptButton->addActionListener( this );
}

/** removes a toggle button from this control */
void MMultiToggleButtonControl::removeToggleButton( MToggleButton* ptButton )
{
	ASSERT( ptButton );
	ASSERT( hasToggleButton( ptButton ) );
	int index = this->getIndex( ptButton );
	ivToggleButtons.erase( ivToggleButtons.begin() + index );
	ptButton->removeActionListener( this );
}

/** incoming events from the toggle buttons */
void MMultiToggleButtonControl::onActionPerformed( void* ptSrc )
{
	ASSERT( ptSrc );
	MToggleButton* ptButton = (MToggleButton*) ptSrc;
	ASSERT( hasToggleButton( ptButton ) );

	if( ptButton->getValue() == false )
	{
		ptButton->setValue( true );
		return;
	}

	if( ptButton != this->ivPtCurrentButton )
	{
		if( this->ivPtCurrentButton )
			ivPtCurrentButton->setValue( false );
		ivPtCurrentButton = ptButton;
		if( this->ivPtCurrentButton )
			ivPtCurrentButton->setValue( true );
		this->fireActionPerformed();
	}
}

/** registers a actionlistener */
void MMultiToggleButtonControl::addActionListener( IActionListener* ptListener )
{
	this->ivActionListener.addListener( ptListener );
}

/** unregisters a actionlistener */
void MMultiToggleButtonControl::removeActionListener( IActionListener* ptListener )
{
	this->ivActionListener.removeListener( ptListener );
}

/** returns the index of the toggle button in list */
int MMultiToggleButtonControl::getIndex( MToggleButton* ptButton )
{
	int back = -1;
	unsigned int count = (unsigned int) ivToggleButtons.size();
	for( unsigned int i=0;i<count;i++ )
		if( ivToggleButtons[ i ] == ptButton )
		{
			back = (int) i;
			break;
		}
	return back;
}

/** returns true if the toggle button is registered */
bool MMultiToggleButtonControl::hasToggleButton( MToggleButton* ptButton )
{
	return getIndex( ptButton ) != -1;
}

/** fires a action event */
void MMultiToggleButtonControl::fireActionPerformed()
{
	unsigned int count = this->ivActionListener.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
	{
		((IActionListener*) ivActionListener.getListener( i ) )->onActionPerformed( (void*) this );
	}
}

/** returns the current selected button */
MToggleButton* MMultiToggleButtonControl::getCurrentButton()
{
	return this->ivPtCurrentButton;
}

/** returns the runtime type info */
IRtti* MMultiToggleButtonControl::getRtti() const
{
	return &gvRtti;
}

/** query superclass */
void* MMultiToggleButtonControl::getInterface( const String &className ) const
{
	if( className == "MMultiToggleButtonControl" )
		return (void*)((MMultiToggleButtonControl*)this);
	else if( className == "IActionListener" )
		return (void*)((IActionListener*)this);
	else
		return MObject::getInterface( className );
}