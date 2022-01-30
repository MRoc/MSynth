#include "MClock.h"
#include "../MNativeTypes.h"

MClock::MClock() :
	ivStart( 0 ),
	ivStop( 0 ),
	ivFreq( 0 ),
	ivCycles( 0 ),
	ivTotalCpuTime( 0 ),
	ivCpuSum( 0 )
{
	if( ! QueryPerformanceFrequency( ((LARGE_INTEGER*)&ivFreq ) ) )
	{
		TRACE( "MClock could not querry performance counter!" );
		exit( -69 );
	}
	QueryPerformanceCounter( ((LARGE_INTEGER*)&ivStart) );
}

MClock::~MClock()
{
}

void MClock::init()
{
	ivCycles = 0;
	ivTotalCpuTime = 0;
	ivCpuSum = 0;
	QueryPerformanceCounter( ((LARGE_INTEGER*)&ivInit) );
	ivStart = ivInit;
}

// returns the time eclapsed since last start
void MClock::start()
{
	ivCycles++;
	QueryPerformanceCounter( ((LARGE_INTEGER*)&ivStart) );
}

void MClock::stop()
{
	QueryPerformanceCounter( ((LARGE_INTEGER*)&ivStop) );
	ivTotalCpuTime += getLastTimeSpan();
	ivCpuSum += getLastCpuConsumption();
}

int MClock::getLastCpuConsumption()
{
	float eclapsedSinceInit = float(ivStop-ivInit)/float(ivFreq)*1000.0f;
	return (int) ((ivTotalCpuTime/eclapsedSinceInit)*100.0f);
}

float MClock::getLastTimeSpan()
{
	return (float(ivStop-ivStart)/float(ivFreq)*1000.0f);
}

int MClock::getAverageCpuConsumption()
{
	return (int) (ivCpuSum / ivCycles);
}

__int64 MClock::getCycles()
{
	return ivCycles;
}

__int64 MClock::getTimeStamp()
{
	__int64 tmp = 0;
	QueryPerformanceCounter( ((LARGE_INTEGER*)&tmp) );
	return tmp;
}

__int64 MClock::getFrequency()
{
	__int64 tmp = 0;
	QueryPerformanceFrequency( ((LARGE_INTEGER*)&tmp ) );
	return tmp;
}