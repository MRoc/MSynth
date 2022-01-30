#include "MStringControl.h"

MRtti MStringControl::gvRtti = MRtti( "MStringControl", 0 );

MStringControl::MStringControl(
	String name,
	String defaultValue ) :
	MDefaultControl( name )
{
	setDefaultValue( defaultValue );
	setValue( defaultValue );
}

MStringControl::~MStringControl()
{
}

String MStringControl::getValue()
{
	return ivValue;
}

void MStringControl::setValue( String value )
{
	ivValue = value;
	fireValueChanged();
}

String MStringControl::getDefaultValue()
{
	return ivDefaultValue;
}

void MStringControl::setDefaultValue( String defaultValue )
{
	ivDefaultValue = defaultValue;
}

MTreeNode* MStringControl::save()
{
	MTreeNode* ptBack = new MTreeNode( "control" );
	ptBack->setAttribute( "name", getName() );
	ptBack->setAttribute( "value", getValue() );
	return ptBack;
}

void MStringControl::load( MTreeNode* node )
{
	setValue( node->getAttribute( "value" ) );
}

String MStringControl::toString()
{
	return getValue();
}

IRtti* MStringControl::getRtti() const
{
	return &gvRtti;
}