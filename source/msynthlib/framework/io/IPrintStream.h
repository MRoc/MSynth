#ifndef __IPrintStream
#define __IPrintStream

#include "../MTypes.h"

/**
 * defines a interface for printing text and number
 */
class IPrintStream
{
public:

	/** 
	 * prints the given string
	 */
	virtual void print( String str ) = 0;

	/** 
	 * prints the given string followed by a \n
	 */
	virtual void println( String str ) = 0;

	/** 
	 * prints the given int
	 */
	virtual void print( int i ) = 0;

	/** 
	 * prints the given int followed by a \n
	 */
	virtual void println( int i ) = 0;

	/** 
	 * prints the given float
	 */
	virtual void print( FP i ) = 0;

	/** 
	 * prints the given float followed by a \n
	 */
	virtual void println( FP i ) = 0;
};

#endif