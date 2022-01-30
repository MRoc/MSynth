#include "MFileInputStream.h"

#include "../MUtil.h"
#include "MIoException.h"
#include <fcntl.h>

/**
 * constructor
 */
MFileInputStream::MFileInputStream() :
	ivPtStreamHandle( 0 ),
	ivFileLength( 0 )
{
}

/**
 * destructor
 */
MFileInputStream::~MFileInputStream()
{
	ASSERT( ivPtStreamHandle == 0 );
}

/**
 * opens a file for reading
 */
void MFileInputStream::open( String fileName )
{
	ASSERT( ivPtStreamHandle == 0 );

	ivPtStreamHandle =
		fopen(
			fileName,
			"rb" );

	if( ivPtStreamHandle == 0 )
		throw MIoException( "Could not open file \"" + fileName + "\"!" );


	fseek( ivPtStreamHandle, 0, SEEK_END );
	ivFileLength = ftell( ivPtStreamHandle );
	fseek( ivPtStreamHandle, 0, SEEK_SET );
}

/**
 * closes the current opened file
 */
void MFileInputStream::close()
{
	ASSERT( ivPtStreamHandle != 0 );

	if( ivPtStreamHandle != 0 && fclose( ivPtStreamHandle ) != 0 )
		throw MIoException( "Could not close stream handle!" );

	// must not be closed when underlying stream was closed
	// see example ms-help://MS.VSCC/MS.MSDNVS.1031/vclib/html/_crt__fdopen.2c_._wfdopen.htm
	//if( ivFileHandle != -1 && _close( ivFileHandle ) != 0 )
	//	throw MIoException( "Could not close file handle!" );

	ivPtStreamHandle = 0;
	ivFileLength = 0;
}

/**
 * seeks to the given position if possible
 */
void MFileInputStream::seek( unsigned int position )
{
	ASSERT( ivPtStreamHandle );

	if( fseek( ivPtStreamHandle, (long) position, SEEK_SET ) != 0 )
		throw MIoException( "Could not seek in file!" );
}

/**
 * returns the length of file
 */
unsigned int MFileInputStream::getFileLength()
{
	return (unsigned int) this->ivFileLength;
}

/**
 * returns the position of the file pointer
 */
unsigned int MFileInputStream::getPosition()
{
	ASSERT( ivPtStreamHandle );

	return (unsigned int) ftell( this->ivPtStreamHandle );
}

/**
 * reads a signed int with 4 bytes little endian
 */
void MFileInputStream::read( signed int& out )
{
	ASSERT( ivPtStreamHandle );

	fread(
		(void*) &out,
		1,
		sizeof( out ),
		ivPtStreamHandle );
}

/**
 * reads a unsigned int with 4 bytes little endian
 */
void MFileInputStream::read( unsigned int &out )
{
	ASSERT( ivPtStreamHandle );

	fread(
		(void*) &out,
		1,
		sizeof( out ),
		ivPtStreamHandle );
}

/**
 * reads a signed short with 2 bytes little endian
 */
void MFileInputStream::read( signed short& out )
{
	ASSERT( ivPtStreamHandle );

	fread(
		(void*) &out,
		1,
		sizeof( out ),
		ivPtStreamHandle );
}

/**
 * reads a unsigned short with 2 bytes little endian
 */
void MFileInputStream::read( unsigned short& out )
{
	ASSERT( ivPtStreamHandle );

	fread(
		(void*) &out,
		1,
		sizeof( out ),
		ivPtStreamHandle );
}

/**
 * reads a signed char
 */
void MFileInputStream::read( signed char& out )
{
	ASSERT( ivPtStreamHandle );

	fread(
		(void*) &out,
		1,
		sizeof( out ),
		ivPtStreamHandle );
}

/**
 * reads a unsigned char
 */
void MFileInputStream::read( unsigned char& out )
{
	ASSERT( ivPtStreamHandle );
	fread(
		(void*) &out,
		1,
		sizeof( out ),
		ivPtStreamHandle );
}

/**
 * reads a amount of data
 */
void MFileInputStream::read( signed char* ptData, unsigned int len )
{
	ASSERT( ptData );
	ASSERT( ivPtStreamHandle );

	read( (unsigned char*) ptData, len );
}

/**
 * reads a amount of data
 */
void MFileInputStream::read( unsigned char* ptData, unsigned int len )
{
	ASSERT( ptData );
	ASSERT( ivPtStreamHandle );

	if( (ftell( ivPtStreamHandle ) + (long)len) > ivFileLength )
		throw MIoException( "Could not read from file (BoundsException)" );
	if( fread(
		(void*) ptData,
		1,
		len,
		ivPtStreamHandle ) != len )
	{
		throw MIoException( "Could not read from file!" );
	}
}

	