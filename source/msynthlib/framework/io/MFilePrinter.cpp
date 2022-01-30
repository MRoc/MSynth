#include "MFilePrinter.h"
#include "../MUtil.h"

MFilePrinter::MFilePrinter() :
	ivPtFile( 0 )
{
}

MFilePrinter::~MFilePrinter()
{
}

bool MFilePrinter::open( String filename )
{
	ivPtFile = fopen( filename, "w+" );
	return ivPtFile != 0;
}

bool MFilePrinter::close()
{
	ASSERT( ivPtFile );
	return fclose( ivPtFile ) == 0;
}

void MFilePrinter::print( String str )
{
	ASSERT( ivPtFile );
	fwrite(
		(const void*) str.getData(),
		1,
		(size_t) str.GetLength(),
		ivPtFile );
}

void MFilePrinter::println( String str )
{
	ASSERT( ivPtFile );
	print( str );
	print( "\n" );
}

void MFilePrinter::print( int i )
{
	ASSERT( ivPtFile );
	print( MUtil::toString( i ) );
}

void MFilePrinter::println( int i )
{
	ASSERT( ivPtFile );
	print( i );
	print( "\n" );
}

void MFilePrinter::print( FP i )
{
	ASSERT( ivPtFile );
	print( MUtil::toString( i ) );
}

void MFilePrinter::println( FP i )
{
	ASSERT( ivPtFile );
	print( i );
	print( "\n" );
}