#include "MObjectDescription.h"

/**
 * the runtime type info
 */
MRtti MObjectDescription::gvRtti = MRtti( "MObjectDescription", 0 );

/** 
 * constructor
 */
MObjectDescription::MObjectDescription( String description, IRtti* ptRtti ) :
	ivDescription( description ),
	ivPtRtti( ptRtti )
{
}

/**
 * destructor
 */
MObjectDescription::~MObjectDescription()
{
}

/**
 * returns the runtime type info
 */
IRtti* MObjectDescription::getRtti() const
{
	return &gvRtti;
}

/**
 * queries for an interface
 */
void* MObjectDescription::getInterface( const String &className ) const
{
	if( className == "MObjectDescription" )
		return (void*) ((MObjectDescription*)this);
	else
		return MObject::getInterface( className );
}

/**
 * returns the runtime type info of the descripted class
 */
IRtti* MObjectDescription::getDescriptedRtti() const
{
	return ivPtRtti;
}

/**
 * returns the description of the class
 */
String MObjectDescription::getDescription() const
{
	return ivDescription;
}

/**
 * returns the desctiption
 */
String MObjectDescription::toString() const
{
	return getDescription();
}