#ifndef __MTYPES
#define __MTYPES

	#pragma warning(disable:4786) //std::truncate warning
	#pragma warning(disable:4100) //unreferenced formal parameter

	#include "MDebug.h"

	#include <vector>
	#include <map>
	#include <set>

	#include "primitive/MString.h"

	class MObject;

	typedef __int32 MColor;
	typedef MString String;
	typedef	signed short MW_SAMPLETYPE;
	typedef float FP;

	typedef std::vector<String> StrList;
	typedef StrList::iterator StrListIter;

	typedef std::vector<int> IntList;
	typedef IntList::iterator IntListIter;

	typedef std::map<String,String> StrStrMap;
	typedef StrStrMap::iterator StrStrMapIter;

	typedef std::map<String, MObject*> Str2ObjMap;
	typedef Str2ObjMap::iterator Str2ObjMapIter;

	#define SAFE_DELETE(p)\
		if(p){\
			delete (p);\
			(p)=NULL;\
		}

#endif