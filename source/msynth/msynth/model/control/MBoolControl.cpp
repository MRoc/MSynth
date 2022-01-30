#include "MBoolControl.h"
#include <framework/primitive/MBool.h>

/**
 * runtime type info
 */
MRtti MBoolControl::gvRtti = MRtti( "MBoolControl", 0 );

/**
 * constructor
 */
MBoolControl::MBoolControl(
	String name,
	bool defaultValue ) :
	MDefaultControl( name ),
	ivDefaultValue( defaultValue ),
	ivValue( defaultValue )
{
}

/**
 * destructor
 */
MBoolControl::~MBoolControl()
{
}

/**
 * returns the current value
 */
bool MBoolControl::getValue()
{
	return ivValue;
}

/**
 * sets the current value
 */
void MBoolControl::setValue( bool value )
{
	ivValue = value;
	fireValueChanged();
}

/**
 * returns the default value
 */
bool MBoolControl::getDefaultValue()
{
	return ivDefaultValue;
}

/**
 * resets the control to default value
 */
void MBoolControl::resetToDefault()
{
	setValue( ivDefaultValue );
}

/**
 * saves the control
 */
MTreeNode* MBoolControl::save()
{
	MTreeNode* ptBack = new MTreeNode( "control" );
	ptBack->setAttribute( "name", getName() );
	ptBack->setAttribute( "value", MInt::toString( getValue() ? 1 : 0 ) );
	return ptBack;
}

/**
 * loads the control
 */
void MBoolControl::load( MTreeNode* node )
{
	setValue(
		MBool::parse(
			node->getAttribute( "value" ),
			getDefaultValue() ) );
}

/**
 * returns a string representation
 */
String MBoolControl::toString()
{
	return MBool::toString( this->ivValue );
}

/**
 * returns the view's id
 */
unsigned int MBoolControl::getViewID()
{
	return DESC_TYPE_BUTTON;
}

/**
 * query for implemented interfaces and superclasses
 */
void* MBoolControl::getInterface( const String &className ) const
{
	if( className == "MBoolControl" )
		return (void*) ((MBoolControl*)this);
	else
		return MDefaultControl::getInterface( className );
}

/**
 * returns the runtime type info
 */
IRtti* MBoolControl::getRtti() const
{
	return &gvRtti;
}