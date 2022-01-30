#include "MTargetIdRegistry.h"

MTargetIdRegistry::MTargetIdRegistry()
{
}

MTargetIdRegistry::~MTargetIdRegistry()
{
	ivTargetMap.clear();
}

IEventTarget* MTargetIdRegistry::getTarget( int serId )
{
	ASSERT( ivTargetMap.find( (IEventTarget*) serId ) != ivTargetMap.end() );
	return ivTargetMap.find( (IEventTarget*) serId )->second;
}

void MTargetIdRegistry::registerTarget( IEventTarget* ptTarget, int serId )
{
	ivTargetMap[ (IEventTarget*) serId ] = ptTarget;
}

void MTargetIdRegistry::clear()
{
	ivTargetMap.clear();
};
