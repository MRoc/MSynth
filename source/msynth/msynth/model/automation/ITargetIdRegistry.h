#ifndef __ITargetIdRegistry
#define __ITargetIdRegistry

#pragma warning(disable:4786)

#include "IAutomationContext.h"

class ITargetIdRegistry
{
	public:

		virtual IEventTarget* getTarget( int serId ) = 0;
		virtual void registerTarget( IEventTarget* ptContext, int serId ) = 0;
		virtual void clear() = 0;
};

#endif

