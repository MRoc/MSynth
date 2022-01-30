#ifndef __IActionListener
#define __IActionListener

#include "IListener.h"

class IActionListener :
	public IListener
{
public:

	virtual void onActionPerformed( void* ptSrc ) = 0;
};

#endif