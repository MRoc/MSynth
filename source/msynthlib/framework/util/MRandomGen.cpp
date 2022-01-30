#include "mrandomgen.h"

/* seed multi and adder taken from audiere.sourceforge.net src/noise.cpp */

/**
 * the multiplicator
 */
int MRandomGen::gvA = 196314165;

/**
 * the adder
 */
int MRandomGen::gvB = 907633515;

/**
 * constructor
 */
MRandomGen::MRandomGen() :
	ivX( 0 )
{
}

/**
 * destructor
 */
MRandomGen::~MRandomGen()
{
}

/**
 * returns the next random value
 */
int MRandomGen::next()
{
	ivX = ivX * gvA + gvB;
	return ivX;
}
