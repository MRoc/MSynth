#ifndef __MTransformerRtti
#define __MTransformerRtti

#include <framework/MObjectDescription.h>
#include "MTransformer.h"

/**
 * defines a runtime type info for a transformer.
 * it contains the rtti information for the MObject
 * and the init params to initialize the transformer.
 */
class MTransformerRtti :
	public MObjectDescription
{
public:

	/**
	 * the runtime type info
	 */
	static MRtti gvRtti;

protected:

	/**
	 * the initialisation parameters used to init the transformer
	 */
	MTransformerInitParam* ivPtParams;

public:

	/**
	 * constructor
	 */
	MTransformerRtti( String name, IRtti* ptRtti, MTransformerInitParam* ptParams );

	/**
	 * destructor
	 */
	virtual ~MTransformerRtti();

	/**
	 * !!!returns the runtime type info of the transformer!!!
	 */
	virtual IRtti* getRtti() const;

	/**
	 * query interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * returns the initialisation parameters
	 */
	virtual MTransformerInitParam* getInitParams();

	/**
	 * returns the name of the transformer
	 */
	virtual String toString();

};

#endif