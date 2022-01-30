#ifndef __IFinalizeListener
#define __IFinalizeListener

#include <framework/MTypes.h>
#include "IListener.h"

/**
 * this listener interface is designed to
 * be informed about the termination of an obeject 
 */
class IFinalizeListener :
	public IListener
{
	public:

		/**
		 * called if the object is deleted, after recievieing this message,
		 * the object MUST deregister itself as listener because the observed
		 * object will be deleted, if not there it will result in an endless loop!!!
		 **/
		virtual void objectTerminated( void* obj ) = 0;
};

#endif