#include "MCriticalSection.h"

/**
 * constructor
 */
MCriticalSection::MCriticalSection()
{
	InitializeCriticalSection( &ivCriticalSection );
}

/**
 * destructor
 */
MCriticalSection::~MCriticalSection()
{
	DeleteCriticalSection( &ivCriticalSection );
}

/**
 * locks the critical section
 */
void MCriticalSection::lock()
{
	EnterCriticalSection( &ivCriticalSection );
}

/**
 * unlocks the critical section
 */
void MCriticalSection::unlock()
{
	LeaveCriticalSection( &ivCriticalSection );
}