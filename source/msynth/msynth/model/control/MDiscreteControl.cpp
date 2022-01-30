#include "MDiscreteControl.h"
#include <framework/primitive/MInt.h>

/**
 * constructor 1
 */
MDiscreteControl::MDiscreteControl( String name, IntList values, int defaultValue )
	: MDefaultControl( name ),
	ivValues( values ),
	ivDefaultValue( defaultValue )
{
	setValue( ivDefaultValue );
}

/**
 * constructor 2
 */
MDiscreteControl::MDiscreteControl( String name, const int values[], unsigned int valueLength, int defaultValue )
: MDefaultControl( name )
{
	for( unsigned int i=0;i<valueLength;i++ )
		ivValues.push_back( values[ i ] );

	ivDefaultValue = defaultValue;
	setValue( ivDefaultValue );
}

/**
 * destructor
 */
MDiscreteControl::~MDiscreteControl()
{
	ivValues.clear();
}


/**
 * resets the control to default value
 */
void MDiscreteControl::resetToDefault()
{
	setValue( ivDefaultValue );
}

/**
 * sets the value
 */
void MDiscreteControl::setValue( int value )
{
	ASSERT( isPossibleValue( value ) );
	setSelectedIndex( getIndex( value ) );
}

/**
 * returns the selected value
 */
int MDiscreteControl::getValue()
{
	ASSERT( unsigned int(ivSelectedIndex) < getValueCount() );
	return ivValues[ ivSelectedIndex ];
}

/**
 * returns the number of possible values
 */
unsigned int MDiscreteControl::getValueCount()
{
	return ivValues.size();
}

/**
 * returns the possible value at index
 */
int MDiscreteControl::getValueAt( unsigned int index )
{
	ASSERT( index < getValueCount() );
	return ivValues[ index ];
}

/**
 * sets the selected index
 */
void MDiscreteControl::setSelectedIndex( unsigned int index )
{
	ASSERT( index < getValueCount() );
	ivSelectedIndex = index;
	fireValueChanged();
}

/**
 * returns the defaulr value
 */
int MDiscreteControl::getDefaultValue()
{
	return ivDefaultValue;
}

/**
 * returns the selected index
 */
unsigned int MDiscreteControl::getSelectedIndex()
{
	return ivSelectedIndex;
}

/**
 * saves the control
 */
MTreeNode* MDiscreteControl::save()
{
	MTreeNode* ptBack = new MTreeNode( "control" );
	ptBack->setAttribute( "name", getName() );
	ptBack->setAttribute( "value", toString() );
	return ptBack;
}

/**
 * loads the control
 */
void MDiscreteControl::load( MTreeNode* node )
{
	int v =
		MInt::parse(
			node->getAttribute( "value" ),
			MInt::MIN_INT,
			MInt::MAX_INT,
			getDefaultValue() );

	if( isPossibleValue( v ) )
		setValue( v );
	else
		setValue( getDefaultValue() );
}

/**
 * returns a string representation
 */
String MDiscreteControl::toString()
{
	return MInt::toString( getValue() );
}

/**
 * returns the index of the given value in intlist
 */
int MDiscreteControl::getIndex( int value )
{
	int index = -1;
	unsigned int count = getValueCount();
	for( unsigned int i=0;i<count;i++ )
		if( ivValues[ i ] == value )
		{
			index = int( i );
			break;
		}
	return index;
}

/**
 * returns true if a value is possible
 */
bool MDiscreteControl::isPossibleValue( int value )
{
	return getIndex( value ) != -1;
}