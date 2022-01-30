#include "MTransformerRegistry.h"

MTransformerRegistry* MTransformerRegistry::gvInstance = 0;

MTransformerRegistry::MTransformerRegistry()
{
}

MTransformerRegistry::~MTransformerRegistry()
{
	unsigned int count = ivRttis.size();
	for( unsigned int i=0;i<count;i++ )
	{
		MTransformerRtti* ptRtti = ivRttis[ i ];
		delete ptRtti;
	}
	ivRttis.clear();
}

MTransformerRegistry* MTransformerRegistry::getInstance()
{
	if( gvInstance == NULL )
		gvInstance = new MTransformerRegistry();
	return gvInstance;
}

void MTransformerRegistry::release()
{
	if( gvInstance )
	{
		delete gvInstance;
		gvInstance = 0;
	}
}

void MTransformerRegistry::store( MTransformerRtti* rtti )
{
	this->ivRttis.push_back( rtti );
}

unsigned int MTransformerRegistry::getTransformerCount()
{
	return (unsigned int) ivRttis.size();
}

MTransformerRtti* MTransformerRegistry::getTransformer( unsigned int index )
{
	ASSERT( index < getTransformerCount() );
	return ivRttis[ index ];
}

MTransformerRtti* MTransformerRegistry::getTransformerByName( String name )
{
	MTransformerRtti* ptBack = 0;
	unsigned int count = (unsigned int) ivRttis.size();
	for( unsigned int i=0;i<count;i++ )
		if( ivRttis[i]->getDescription() == name )
		{
			ptBack = ivRttis[i];
			break;
		}
	return ptBack;
}

MTransformerRtti* MTransformerRegistry::getTransformerByClass( String className )
{
	MTransformerRtti* ptBack = 0;
	unsigned int count = (unsigned int) ivRttis.size();
	for( unsigned int i=0;i<count;i++ )
		if( ivRttis[i]->getDescriptedRtti()->getClassName() == className )
		{
			ptBack = ivRttis[i];
			break;
		}
	return ptBack;
}

MTransformer* MTransformerRegistry::createTransformerByClass( String className, unsigned int channelCount )
{
	MTransformer* ptBack = 0;
	MTransformerRtti* ptRtti = getTransformerByClass( className );
	if( ptRtti )
		ptBack = createTransformer( ptRtti, channelCount );
	return ptBack;
}

MTransformer* MTransformerRegistry::createTransformerByName( String name, unsigned int channelCount )
{
	MTransformer* ptBack = 0;
	MTransformerRtti* ptRtti = getTransformerByName( name );
	if( ptRtti )
		ptBack = createTransformer( ptRtti, channelCount );
	return ptBack;
}

MTransformer* MTransformerRegistry::createTransformer( MTransformerRtti* ptRtti, unsigned int channelCount )
{
	MObject* ptObj = ptRtti->getDescriptedRtti()->createInstance();
	MTransformer* ptTrans = (MTransformer*) ptObj->getInterface( "MTransformer" );
	if( ! ptTrans )
	{
		delete ptObj;
	}
	else
	{
		if( ! ( ptTrans->init( ptRtti->getInitParams() ) &&	ptTrans->create( channelCount ) ) )
		{
			delete ptObj;
			ptTrans = 0;
		}
	}
	return ptTrans;
}