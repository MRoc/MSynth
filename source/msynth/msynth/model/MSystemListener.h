#ifndef __MSystemListener
#define __MSystemListener

class ISystemListener
{
public:

	virtual void systemStepChanged( int currentStep ) = 0;
	virtual void systemPatternChanged( int currentPattern ) = 0;
};

#endif