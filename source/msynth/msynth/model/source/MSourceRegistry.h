#ifndef __MSourceRegistry
#define __MSourceRegistry

#include <framework/MTypes.h>
#include <framework/MException.h>
#include <framework/MObjectDescription.h>
#include <framework/list/MObjectList.h>

/**
 * a registry where all soundsources (MOsc) are registered
 */
class MSourceRegistry
{
protected:

	/**
	 * objectlist storing all MObjectDescriptions
	 */
	MObjectList ivRegistrations;

	/**
	 * the singleton instance
	 */
	static MSourceRegistry* gvInstance;

	/**
	 * constructor
	 */
	MSourceRegistry();

	/**
	 * destructor
	 */
	virtual ~MSourceRegistry();


public:

	/**
	 * returns the number of registered sources
	 */
	virtual unsigned int getRegistrationCount();

	/**
	 * returns the name of the source like the user known it
	 */
	virtual MObjectDescription* getRegistration( unsigned int index );

	/**
	 * registers a source
	 */
	virtual void store( MObjectDescription* ptDesc );

	/**
	 * returns the singleton instance
	 */
	static MSourceRegistry* getInstance();

	/**
	 * deletes the singleton instance
	 */
	static void release();
};

#endif