/*

	MToggleButton (C)2001 MRoc hifiShock

*/

#include "MToggleButton.h"

/** runtime type info */
MRtti MToggleButton::gvRtti = MRtti( "MToggleButton", MToggleButton::createInstance );

/** constructor */
MToggleButton::MToggleButton() :
	ivState( false )
{
}

/** constructor */
MToggleButton::MToggleButton( IWndRenderer* pRenderer ) :
	MButton( pRenderer ),
	ivState( false )
{
}

/** constructor */
MToggleButton::MToggleButton( unsigned int bmIdUnpressed, unsigned int bmIdPressed, MColor tColor1,
	MColor tColor2, MColor bkColor ) : 
	MButton( bmIdUnpressed, bmIdPressed, tColor1, tColor2, bkColor ),
	ivState( false )
{
}

/** constructor */
MToggleButton::MToggleButton( String text, MColor textColor, MColor tColorInvalid,	MColor bkColor ) : 
	MButton( text, textColor, tColorInvalid, bkColor ),
	ivState( false )
{
}

/** constructor */
MToggleButton::MToggleButton( MArrowRenderer::Direction direction, MColor bkColor ) :
	MButton( direction, bkColor ),
	ivState( false )
{
}
	
/** destructor */
MToggleButton::~MToggleButton()
{
}

/** returns the toggle value */
bool MToggleButton::getValue()
{
	return ivState;
}

/** sets the toggle value */
void MToggleButton::setValue( bool value)
{
	if( value != ivState )
	{
		ivState = value;
		repaint();
	}
}

/** invoked from MButton when clicked */
void MToggleButton::clicked()
{
	ivState = ! ivState;
}

/** returns true if button should be painted sunken */
bool MToggleButton::isSunken()
{
	//TRACE( "MToggleButton::isSunken: state=%s\n", ivState ? "true":"false" );
	return ivState || ( getCapturing() && getMouseIn() );
}

/** query superclass */
void* MToggleButton::getInterface( const String& className ) const
{
	if( className == "MToggleButton" )
		return (void*) ((MToggleButton*)this);
	else
		return MButton::getInterface( className );
}

/** returns the runtime type info */
IRtti* MToggleButton::getRtti() const
{
	return &gvRtti;
}

/** creates an instance of this class */
MObject* MToggleButton::createInstance()
{
	return new MToggleButton();
}