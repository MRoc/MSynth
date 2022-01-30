#include "MRtti.h"

/** constructor */
MRtti::MRtti( const char* className, LPCREATEOBJECT ptObjCreator ) :
	ivClassName( className ),
	ivPtObjectCreator( ptObjCreator )
{
}

/** destructor */
MRtti::~MRtti()
{
}

/** returns the classname for the specified class */
String MRtti::getClassName() const
{
	return ivClassName;
}

/** returns true if this rtti is able to create an instance */
bool MRtti::canCreate() const
{
	return ivPtObjectCreator != 0;
}

/** creates an new instance of the class */
MObject* MRtti::createInstance() const
{
	return ivPtObjectCreator();
}