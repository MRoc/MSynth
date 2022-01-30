#ifndef __MIoException
#define __MIoException

#include "../MException.h"

class MIoException :
	public MException
{
public:

	MIoException( String msg );

	virtual ~MIoException();
};

#endif