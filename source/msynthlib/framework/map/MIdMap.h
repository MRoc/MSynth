#ifndef __MIdMap
#define __MIdMap

#include "MInt2ObjMap.h"

class MIdMap
{
	protected:

		static MInt2ObjMap* gvInstance;

	public:

		static MInt2ObjMap* getInstance();
		static void release();
};

#endif