#include "MActionListenerList.h"

MActionListenerList::MActionListenerList(void)
{
}

MActionListenerList::~MActionListenerList(void)
{
}

void MActionListenerList::fireActionPerformed( void* ptSrc )
{
	unsigned int count = this->getListenerCount();
	for( unsigned int i=0;i<count;i++ )
	{
		((IActionListener*)this->ivListeners[ i ])->onActionPerformed( ptSrc );
	}
}