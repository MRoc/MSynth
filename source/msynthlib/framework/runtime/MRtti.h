#ifndef __MRtti
#define __MRtti

#include "../MTypes.h"
#include "../IRtti.h"

/** implementation of the IRtti interface */
class MRtti :
	public IRtti
{
protected:

	/** the class name */
	const char* ivClassName;

	/**
	 * function pointer to a static method
	 * creating an instance of the referenced class
	 */
	LPCREATEOBJECT ivPtObjectCreator;

public:

	/** constructor */
	MRtti( const char* className, LPCREATEOBJECT ptObjCreator );

	/** destructor */
	virtual ~MRtti();

	/** returns the classname for the specified class */
	virtual String getClassName() const;

	/** returns true if this rtti is able to create an instance */
	virtual bool canCreate() const;

	/** creates an new instance of the class */
	virtual MObject* createInstance() const;
};

#define DECL_RTTI() static MRtti gvRtti;
#define INIT_RTTI( ClassName, StrClassName ) MRtti ClassName::gvRtti = MRtti( StrClassName, 0 );
#define INIT_RTTI_CREATE( ClassName, StrClassName ) MRtti ClassName::gvRtti = MRtti( StrClassName, ClassName::createInstance );
#define GET_RTTI( ClassName ) virtual IRtti* getRtti() const{ return &ClassName::gvRtti; }

#endif