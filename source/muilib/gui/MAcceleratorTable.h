#ifndef __MAcceleratorTable
#define __MAcceleratorTable

#include <framework/MTypes.h>
#include <framework/MNativeTypes.h>

class MAcceleratorTable
{
protected:

	HACCEL ivAccl;

public:

	MAcceleratorTable();
	virtual ~MAcceleratorTable();

	virtual bool load( unsigned int resourceId );
	virtual void unload();
	virtual bool isLoaded();

	virtual HACCEL getHandle();
};

#endif