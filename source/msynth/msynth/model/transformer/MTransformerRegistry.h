/**
 * (C) 2003 MRoc
 */

#ifndef __MSOUNDTRANSFORMERFACTORY
#define __MSOUNDTRANSFORMERFACTORY

#include <framework/MTypes.h>
#include "MTransformer.h"
#include "MTransformerRtti.h"

typedef std::vector<MTransformerRtti*> MTRttiList;

class MTransformerRegistry
{
protected:

	static MTransformerRegistry* gvInstance;

	MTRttiList ivRttis;

	MTransformerRegistry();
	virtual ~MTransformerRegistry();

public:

	static MTransformerRegistry* getInstance();
	static void release();

	virtual void store( MTransformerRtti* rtti );
	virtual unsigned int getTransformerCount();
	virtual MTransformerRtti* getTransformer( unsigned int index );
	virtual MTransformerRtti* getTransformerByName( String name );
	virtual MTransformerRtti* getTransformerByClass( String className );

	virtual MTransformer* createTransformerByClass( String className, unsigned int channelCount );
	virtual MTransformer* createTransformerByName( String name, unsigned int channelCount );
	virtual MTransformer* createTransformer( MTransformerRtti* ptRtti, unsigned int channelCount );
};
#endif