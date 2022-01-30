#include "MFileOutputStream.h"

#include "../MUtil.h"
#include "MIoException.h"

/**
 * constructor
 */
MFileOutputStream::MFileOutputStream() :
	ivPtFile( 0 )
{
}

/**
 * destructor
 */
MFileOutputStream::~MFileOutputStream()
{
	ASSERT( ivPtFile == 0 ); // assert file is closed
}

/**
 * opens the specified file for writing
 */
void MFileOutputStream::open( String filename )
{
	ASSERT( ivPtFile == 0 );
	ivPtFile = fopen( filename, "w+b" );
	if( ivPtFile == 0 )
		throw MIoException( "Could not open file \"" + filename + "\"!" );
}

/**
 * closes the currently opened file
 */
void MFileOutputStream::close()
{
	ASSERT( ivPtFile );
	if( fclose( ivPtFile ) != 0 )
		throw MIoException( "Could not close file handle!" );
	ivPtFile = 0;
}

/**
 * seeks to the given position if possible
 */
void MFileOutputStream::seek( unsigned int position )
{
	ASSERT( ivPtFile );
	if( fseek( ivPtFile, (long) position, SEEK_SET ) != 0 )
		throw new MIoException( "Could not seek!" );
}

/**
 * writes the given int with 4 bytes intel little endian
 */
void MFileOutputStream::write( int value )
{
	write( (const unsigned char*) &value, sizeof( value ) );
}

/**
 * writes the given unsigned int with 4 bytes intel little endian
 */
void MFileOutputStream::write( unsigned int value )
{
	write( (const unsigned char*) &value, sizeof( value ) );
}

/**
 * writes the given short with 2 bytes intel little endian
 */
void MFileOutputStream::write( short value )
{
	write( (const unsigned char*) &value, sizeof( value ) );
}

/**
 * writes the given unsigned short with 2 bytes intel little endian
 */
void MFileOutputStream::write( unsigned short value )
{
	write( (const unsigned char*) &value, sizeof( value ) );
}

/**
 * writes the given char
 */
void MFileOutputStream::write( char value )
{
	write( (const unsigned char*) &value, sizeof( value ) );
}

/**
 * writes the given unsigned char
 */
void MFileOutputStream::write( unsigned char value )
{
	write( (const unsigned char*) &value, sizeof( value ) );
}

/**
 * writes the given char data with the given length
 */
void MFileOutputStream::write( const char* data, unsigned int length )
{
	write( (const unsigned char*) data, length );
}

/**
 * writes the given unsigned char data with the given length
 */
void MFileOutputStream::write( const unsigned char* data, unsigned int length )
{
	ASSERT( ivPtFile );
	if(
		fwrite(
			(const void*) data,
			1,
			(size_t) length,
			ivPtFile ) != length )
	{
		throw MIoException( "Could not write data!" );
	}
}
