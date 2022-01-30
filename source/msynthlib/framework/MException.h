#ifndef __MException
#define __MException

#include "MTypes.h"

class MException
{
protected:

	String ivExceptionDescription;
	
public:

	MException(String text)
	{
		ivExceptionDescription=text;
	}
	
	virtual ~MException()
	{
	}

	String getExceptionDescripton()
	{
		return ivExceptionDescription;
	}
};

#endif