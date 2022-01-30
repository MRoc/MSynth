#include "MIIROrder1.h"

/**
 * constructor
 */
MIIROrder1::MIIROrder1() :
	ivA( 1.0 ),
	ivB( 0.0 ),
	ivY( 0.0 )
{
}

/**
 * destructor
 */
MIIROrder1::~MIIROrder1()
{
}

/**
 * resets the filter buffer
 */
void MIIROrder1::reset()
{
	ivY = 0.0;
}

/**
 * sets the filter coeffs
 */
void MIIROrder1::setCoeffs( double a, double b )
{
	this->ivA = a;
	this->ivB = b;
}

/**
 * filters a amount of data
 */
void MIIROrder1::filter( float* ptData, unsigned int dataLength )
{
	float* pt = ptData;
	float* ptStop = pt + dataLength;
	for( ; pt < ptStop; pt++ )
	{
		/**
		*
		* first order IIR filter
		*
		* y[n] = a*x[n] - b*y[n-1]
		*
		*/
		double out = ( ivA * (*pt) - ivB * ivY );
		(*pt) = (float) out;
		ivY = out;
	}
}