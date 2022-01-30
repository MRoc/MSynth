#ifndef __MObject
#define __MObject

#include "IMUnknown.h"
#include "runtime/MRtti.h"

/** superclass of all used objects */
class MObject :
	public IMUnknown
{
public:

	/** the element name of a serialized MObject */
	static const String ELEM_OBJECT;

	/** the attribute name for the classname of a serialized MObject */
	static const String ATTR_CLASS;

public:

	/** constructor */ 
	MObject();

	/** destructor */
	virtual ~MObject();

	/** returns a string representation */
	virtual String toString() const;

	/** returns the runtime type information */
	virtual IRtti* getRtti() const = 0;

	/** query for superclass */
	virtual void* getInterface( const String &className ) const;
};

#define GET_INTERFACE( ClassName, StrClassName ) virtual void* getInterface( const String &cn ) const{ if(cn==StrClassName) return (void*)((ClassName*)this); else return MObject::getInterface( cn ); };
#define GET_INTERFACE_SUPER( ClassName, StrClassName, SuperClass ) virtual void* getInterface( const String &cn ) const{ if(cn==StrClassName) return (void*)((ClassName*)this); else return SuperClass::getInterface( cn ); };
#define FULL_RTTI( ClassName, StrClassName ) DECL_RTTI(); GET_RTTI( ClassName ); GET_INTERFACE( ClassName, StrClassName );
#define FULL_RTTI_SUPER( ClassName, StrClassName, SuperClass ) DECL_RTTI(); GET_RTTI( ClassName ); GET_INTERFACE_SUPER( ClassName, StrClassName, SuperClass );

#endif