#include "msynchronize.h"

/**
 * constructor
 */
MSynchronize::MSynchronize( MCriticalSection* ptCriticalSection ) :
	ivPtCriticalSection( ptCriticalSection )
{
	ivPtCriticalSection->lock();
}

/**
 * destructor
 */
MSynchronize::~MSynchronize()
{
	ivPtCriticalSection->unlock();
}
