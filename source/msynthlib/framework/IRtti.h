#ifndef __IRtti
#define __IRtti

#include "MTypes.h"
class MObject;
typedef MObject* (*LPCREATEOBJECT)();

/**
 * interface defining a runtime support for c++ classes.
 * can query for classname and perhaps can instantiate.
 */
class IRtti
{
public:

	/** returns the classname for the specified class */
	virtual String getClassName() const = 0;

	/** returns true if this rtti is able to create an instance */
	virtual bool canCreate() const = 0;

	/** creates an new instance of the class */
	virtual MObject* createInstance() const = 0;
};

#endif