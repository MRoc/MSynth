#ifndef __Win32ResizeRequest
#define __Win32ResizeRequest

#include <framework/MNativeTypes.h>
#include "MResizeRequestEvent.h"

class MWin32ResizeRequest :
	public MResizeRequestEvent
{
protected:

	MINMAXINFO* ivNativeInfo;

public:

	MWin32ResizeRequest( MINMAXINFO* info );
	virtual ~MWin32ResizeRequest();

	virtual void setMax( MSize max );
	virtual void setMin( MSize min );
	virtual MSize getMax();
	virtual MSize getMin();
};

#endif