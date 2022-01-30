#ifdef _DEBUG

#include "MDebug.h"
#include "MTypes.h"
#include "MNativeTypes.h"

void __cdecl trace(const char *pchFormat, ...)
{
	va_list args;
	va_start(args, pchFormat);

	int iBuf;
	char pchString[4096];
	pchString[0];

	iBuf = _vsnprintf(pchString, 4095 , pchFormat, args);

	if(iBuf <= 0)
	{
		sprintf(pchString,"Warning! Trace string extends buffer!\n");
	}

	OutputDebugString(pchString);

	va_end(args);
}

#endif