#ifndef __MObjectDescription
#define __MObjectDescription

#include "MObject.h"

/**
 * encapsulates the IRtti of a class and
 * a name or text describint the object
 */
class MObjectDescription :
	public MObject
{
public:

	/**
	 * the runtime type info
	 */
	static MRtti gvRtti;

protected:

	/**
	 * the runtime type info of the descripted class
	 */
	IRtti* ivPtRtti;

	/**
	 * string description
	 */
	String ivDescription;

public:

	/**
	 * constructor
	 */
	MObjectDescription( String description, IRtti* ptRtti );

	/**
	 * destructor
	 */
	virtual ~MObjectDescription();

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * queries for an interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * returns the runtime type info of the descripted class
	 */
	virtual IRtti* getDescriptedRtti() const;

	/**
	 * returns the description of the class
	 */
	virtual String getDescription() const;

	/**
	 * returns the desctiption
	 */
	virtual String toString() const;
};

#endif