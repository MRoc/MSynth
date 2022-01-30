#ifndef __MActionListenerList
#define __MActionListenerList

#include "MListenerList.h"
#include "IActionListener.h"

class MActionListenerList :
	public MListenerList
{
public:
	MActionListenerList();
	virtual ~MActionListenerList();

	virtual void fireActionPerformed( void* ptSrc );
};

#endif