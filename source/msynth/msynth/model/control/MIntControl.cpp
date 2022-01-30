#include "MIntControl.h"
#include <framework/primitive/MInt.h>

MRtti MIntControl::gvRtti = MRtti( "MIntControl", 0 );

MIntControl::MIntControl( String name, int minValue, int maxValue, int defaultValue )
	: MDefaultControl( name )
{
	setMinValue( minValue );
	setMaxValue( maxValue );
	setDefaultValue( defaultValue );
	setValue( defaultValue );
	setEinheit( 0 );
}

MIntControl::~MIntControl()
{
}

int	MIntControl::getMinValue()
{
	return ivMinValue;
}

int	MIntControl::getMaxValue()
{
	return ivMaxValue;
}

int	MIntControl::getRange()
{
	return ivMaxValue - ivMinValue;
}

int	MIntControl::getValue()
{
	return ivValue;
}

void MIntControl::setValue( int value )
{
	ASSERT( value >= ivMinValue && value <= ivMaxValue );
	ivValue = value;
	fireValueChanged();
}

int MIntControl::getDefaultValue()
{
	return ivDefaultValue;
}

const char* MIntControl::getEinheit()
{
	return ivEinheit;
}

void MIntControl::setMinValue( int minValue )
{
	ivMinValue = minValue;
}

void MIntControl::setMaxValue( int maxValue )
{
	ivMaxValue = maxValue;
}

void MIntControl::setDefaultValue( int defaultValue )
{
	ivDefaultValue = defaultValue;
}

void MIntControl::setEinheit( char* einheit )
{
	ivEinheit = einheit;
}

MTreeNode* MIntControl::save()
{
	MTreeNode* ptBack = new MTreeNode( "control" );
	ptBack->setAttribute( "name", getName() );
	ptBack->setAttribute( "value", toString() );
	return ptBack;
}

void MIntControl::load( MTreeNode* node )
{
	setValue(
		MInt::parse(
			node->getAttribute( "value" ),
			ivMinValue,
			ivMaxValue,
			getDefaultValue() ) );
}

String MIntControl::toString()
{
	return MInt::toString( ivValue );
}

IRtti* MIntControl::getRtti() const
{
	return &gvRtti;
}
