#ifndef __MDefaultFactory
#define __MDefaultFactory

#include "MObjectFactory.h"

class MDefaultFactory :
	public MObjectFactory
{
private:

	static MDefaultFactory* gvInstance;

	MDefaultFactory();
	virtual ~MDefaultFactory();

public:

	static MDefaultFactory* getInstance();
	static void release();
};

#endif