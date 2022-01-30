#include "MSourceRegistry.h"

/**
 * the singleton instance
 */
MSourceRegistry* MSourceRegistry::gvInstance = 0;

MSourceRegistry::MSourceRegistry()
{
}

MSourceRegistry::~MSourceRegistry()
{
	this->ivRegistrations.deleteAll();
}

/**
 * returns the number of registered sources
 */
unsigned int MSourceRegistry::getRegistrationCount()
{
	return ivRegistrations.getLength();
}

/**
 * returns the class name
 */
MObjectDescription* MSourceRegistry::getRegistration( unsigned int index )
{
	ASSERT( index < getRegistrationCount() );
	return (MObjectDescription*) ivRegistrations.get( index );
}

/**
 * registers a source
 */
void MSourceRegistry::store( MObjectDescription* ptDesc )
{
	ivRegistrations.add( ptDesc );
}

/**
 * returns the singleton instance
 */
MSourceRegistry* MSourceRegistry::getInstance()
{
	if( gvInstance == 0 )
		gvInstance = new MSourceRegistry();
	return gvInstance;
}

/**
 * deletes the singleton instance
 */
void MSourceRegistry::release()
{
	if( gvInstance )
		delete gvInstance;
}