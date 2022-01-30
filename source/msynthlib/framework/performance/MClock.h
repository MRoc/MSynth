#ifndef __MClock
#define __MClock

#include "../MTypes.h"

class MClock
{
protected:

	__int64
		ivInit,
		ivStart,
		ivStop,
		ivFreq,
		ivCycles;
	float
		ivTotalCpuTime,
		ivCpuSum;

public:

	MClock();
	virtual ~MClock();

	virtual void start();
	virtual void stop();
	virtual void init();

	virtual float getLastTimeSpan();
	virtual int getLastCpuConsumption();
	virtual int getAverageCpuConsumption();

	virtual __int64 getCycles();

	static __int64 getTimeStamp();
	static __int64 getFrequency();
};

#endif