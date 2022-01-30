#ifndef __MStreamPrinter
#define __MStreamPrinter

#include "IPrintStream.h"
#include "IOutputStream.h"

/**
 * a printer printing to a output stream
 */
class MStreamPrinter :
	public IPrintStream
{
protected:

	/**
	 * the output stream
	 */
	IOutputStream* ivPtOutputStream;

public:

	/** 
	 * constructor
	 */
	MStreamPrinter( IOutputStream* pOut );

	/**
	 * destructor
	 */
	virtual ~MStreamPrinter();

	/** 
	 * prints the given string
	 */
	virtual void print( String str );

	/** 
	 * prints the given string followed by a \n
	 */
	virtual void println( String str );

	/** 
	 * prints the given int
	 */
	virtual void print( int i );

	/** 
	 * prints the given int followed by a \n
	 */
	virtual void println( int i );

	/** 
	 * prints the given float
	 */
	virtual void print( FP i );

	/** 
	 * prints the given float followed by a \n
	 */
	virtual void println( FP i );
};

#endif