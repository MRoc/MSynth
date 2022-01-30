#include "MObject.h"

/** the element name of a serialized MObject */
const String MObject::ELEM_OBJECT = "mobject";

/** the attribute name for the classname of a serialized MObject */
const String MObject::ATTR_CLASS = "class"; 

/** constructor */
MObject::MObject()
{
}

/** destructor */
MObject::~MObject()
{
}

/** returns a string representation */
String MObject::toString() const
{
	return getRtti()->getClassName();
}

/** querries for a specified interface */
void* MObject::getInterface( const String &className ) const
{
	if( className == "MObject" )
		return (void*) ((MObject*)this);
	else if( className == "IMUnknown" )
		return (void*) ((IMUnknown*)this);
	else
		return 0;
}