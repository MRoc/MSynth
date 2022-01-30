#ifndef __MRandomGen
#define __MRandomGen

/**
 * random number generation based in linear kongruence
 */
class MRandomGen
{
protected:

	/**
	 * the current random value
	 */
	int ivX;

	/**
	 * the multiplicator
	 */
	static int gvA;

	/**
	 * the adder
	 */
	static int gvB;

public:

	/**
	 * constructor
	 */
	MRandomGen();

	/**
	 * destructor
	 */
	virtual ~MRandomGen();

	/**
	 * returns the next random value
	 */
	int next();
};

#endif