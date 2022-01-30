#include "MStreamPrinter.h"
#include "../MUtil.h"

/** 
 * constructor
 */
MStreamPrinter::MStreamPrinter( IOutputStream* pOut ) :
	ivPtOutputStream( pOut )
{
	ASSERT( ivPtOutputStream );
}

/**
 * destructor
 */
MStreamPrinter::~MStreamPrinter()
{
}

/** 
 * prints the given string
 */
void MStreamPrinter::print( String str )
{
	ASSERT( ivPtOutputStream );
	ivPtOutputStream->write( str.getData(), str.GetLength() );
}

/** 
 * prints the given string followed by a \n
 */
void MStreamPrinter::println( String str )
{
	ASSERT( ivPtOutputStream );
	ivPtOutputStream->write( str.getData(), str.GetLength() );
	ivPtOutputStream->write( "\n", 0 );
}

/** 
 * prints the given int
 */
void MStreamPrinter::print( int i )
{
	ASSERT( ivPtOutputStream );
	print( MUtil::toString( i ) );
}

/** 
 * prints the given int followed by a \n
 */
void MStreamPrinter::println( int i )
{
	ASSERT( ivPtOutputStream );
	print( MUtil::toString( i ) );
	print( "\n" );
}

/** 
 * prints the given float
 */
void MStreamPrinter::print( FP i )
{
	ASSERT( ivPtOutputStream );
	print( MUtil::toString( i ) );
}

/** 
 * prints the given float followed by a \n
 */
void MStreamPrinter::println( FP i )
{
	ASSERT( ivPtOutputStream );
	print( i );
	print( "\n" );
}
