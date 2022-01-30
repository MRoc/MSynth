#ifndef __MTargetIdRegistry
#define __MTargetIdRegistry

#pragma warning(disable:4786)

#include "ITargetIdRegistry.h"

// this class is to resolve all referenced ids after serialisation from
// an file
typedef std::map<IEventTarget*, IEventTarget*> TargetMap;
class MTargetIdRegistry : public ITargetIdRegistry
{
	protected:

		TargetMap ivTargetMap;

	public:

		MTargetIdRegistry();
		virtual ~MTargetIdRegistry();

		virtual IEventTarget* getTarget( int serId );
		virtual void registerTarget( IEventTarget* ptContext, int serId );
		virtual void clear();
};

#endif