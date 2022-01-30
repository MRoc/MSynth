#include "MDiscreteComboControl.h"

MRtti MDiscreteComboControl::gvRtti = MRtti( "MDiscreteComboControl", 0 );

MDiscreteComboControl::MDiscreteComboControl( String name, IntList values, StrList strings, int defaultValue )
	: MDiscreteControl( name, values, defaultValue )
{
	ivStrings = strings;
	ASSERT( ivValues.size() == ivStrings.size() );
}

MDiscreteComboControl::MDiscreteComboControl( String name, const int values[], const char* strings[], unsigned int valueLength, int defaultValue )
	: MDiscreteControl( name, values, valueLength, defaultValue )
{
	for( unsigned int i=0;i<valueLength;i++ )
		ivStrings.push_back( String( strings[ i ] ) );
	ASSERT( ivValues.size() == ivStrings.size() );
}

MDiscreteComboControl::~MDiscreteComboControl()
{
	ivValues.clear();
	ivStrings.clear();
}

String MDiscreteComboControl::getStringAt( unsigned int index )
{
	ASSERT( index < getValueCount() );
	return ivStrings[ index ];
}

unsigned int MDiscreteComboControl::getViewID()
{
	return DESC_TYPE_COMBO;
}

IRtti* MDiscreteComboControl::getRtti() const
{
	return &gvRtti;
}