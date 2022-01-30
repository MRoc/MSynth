#ifndef __ISongListener
#define __ISongListener

#include <framework/listener/MListenerList.h>

class ISongListener :
	public IListener
{
public:
	virtual void instrumentAdded( unsigned int index ) = 0;
	virtual void instrumentRemoved( unsigned int index ) = 0;
	virtual void instrumentsSwapped( unsigned int index1, unsigned int index2 ) = 0;

	virtual void songModelChanged() = 0;

	virtual void songDestroy() = 0;
};

#endif
