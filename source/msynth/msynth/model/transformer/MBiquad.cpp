#include "MBiquad.h"

MBiquad::MBiquad()
{
}

MBiquad::~MBiquad()
{
}

void MBiquad::goNext(
	MBiquadCoeffs* coeffs,
	MBiquadStack* stack,
	FP* pData,
	unsigned int len )
{
	ASSERT( coeffs );
	ASSERT( stack );
	ASSERT( pData );

	FP* pt = pData;
	FP* ptStop = pData + len;
	for( ;pt<ptStop;pt++ )
	{
		stack->x2 = stack->x1;
		stack->x1 = stack->x0;
		stack->x0 = (*pt);
		stack->y2 = stack->y1;	
		stack->y1 = stack->y0;
		(*pt) = stack->y0 =
			coeffs->a0 * stack->x0 +
			coeffs->a1 * stack->x1 +
			coeffs->a2 * stack->x2 -
			coeffs->b1 * stack->y1 -
			coeffs->b2 * stack->y2;
	}
}