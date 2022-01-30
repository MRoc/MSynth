#include "MTransformerRtti.h"

/**
 * the runtime type info
 */
MRtti MTransformerRtti::gvRtti = MRtti( "MTransformerRtti", 0 );

/**
 * constructor
 */
MTransformerRtti::MTransformerRtti( String name, IRtti* ptRtti, MTransformerInitParam* ptParams ) :
	MObjectDescription( name, ptRtti ),
	ivPtParams( ptParams )
{
}

/**
 * destructor
 */
MTransformerRtti::~MTransformerRtti()
{
	if( ivPtParams )
		delete ivPtParams;
}

/**
 * returns the name of the transformer
 */
String MTransformerRtti::toString()
{
	return this->ivDescription;
}

/**
 * returns the runtime type info
 */
IRtti* MTransformerRtti::getRtti() const
{
	return &gvRtti;
}

/**
 * returns the initialisation parameters
 */
MTransformerInitParam* MTransformerRtti::getInitParams()
{
	return ivPtParams;
}

/**
 * query interface
 */
void* MTransformerRtti::getInterface( const String &className ) const
{
	if( className == "MTransformerRtti" )
		return (void*) ((MTransformerRtti*)this);
	else
		return MObjectDescription::getInterface( className );
}