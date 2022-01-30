#ifndef __MBiquad
#define __MBiquad

#include <framework/MTypes.h>

/**
 * the biquad stack
 */
struct MBiquadStack
{
	FP x0, x1, x2, y0, y1, y2;

	MBiquadStack()
	{
		x0 = x1 = x2 = y0 = y1 = y2 = 0.0f;
	}
};

/** 
 * the biquad filter coeffs
 */
struct MBiquadCoeffs
{
	FP a0, a1, a2, b1, b2;
};

/**
 * the biquad filter algorythm
 */
class MBiquad
{
public:

	MBiquad();
	virtual ~MBiquad();

	virtual void goNext(
		MBiquadCoeffs* pCoeffs,
		MBiquadStack* pStack,
		FP* pData,
		unsigned int len );
};

#endif