/*(C)2000 MRoc hifiShock*/

#ifndef __MNAME
#define __MNAME

#include <framework/MTypes.h>

class IName
{
	virtual String getType() = 0;
	virtual String getName() = 0;
};

#endif