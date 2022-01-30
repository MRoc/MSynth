#ifndef __IScrollBarListener
#define __IScrollBarListener

#include <framework/listener/IListener.h>

class IScrollBarListener :
	public IListener
{
public:

	virtual void valueChanged( void* source ) = 0;
};

#endif