#include "midmap.h"

MInt2ObjMap* MIdMap::gvInstance = 0;

MInt2ObjMap* MIdMap::getInstance()
{
	if( ! gvInstance )
		gvInstance = new MInt2ObjMap();
	return gvInstance;
}

void MIdMap::release()
{
	if( gvInstance )
	{
		gvInstance->clear();
		delete gvInstance;
		gvInstance = 0;
	}
}
