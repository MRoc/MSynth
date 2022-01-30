#ifndef __MDebug
#define __MDebug

#ifdef _DEBUG

	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
	#include <assert.h>

	void __cdecl trace(const char *pchFormat, ...);

	#define TRACE trace
	#define ASSERT assert

#endif

#endif