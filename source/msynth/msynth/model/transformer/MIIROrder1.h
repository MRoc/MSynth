#ifndef __MIIROrder1
#define __MIIROrder1

/**
 * first order IIR filter
 *
 * y[n] = a*x[n] - b*y[n-1]
 *
 */
class MIIROrder1
{
protected:

	double ivA, ivB, ivY;

public:

	/**
	 * constructor
	 */
	MIIROrder1();

	/**
	 * destructor
	 */
	virtual ~MIIROrder1();

	/**
	 * resets the filter buffer
	 */
	virtual void reset();

	/**
	 * sets the filter coeffs
	 */
	virtual void setCoeffs( double a, double b );

	/**
	 * filters a amount of data
	 */
	virtual void filter( float* ptData, unsigned int dataLength );
};

#endif