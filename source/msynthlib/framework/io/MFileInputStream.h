#ifndef __MFileInputStream
#define __MFileInputStream

#include <stdio.h>
#include "../MTypes.h"
#include "ISeekableInputStream.h"

/**
 * implementation of a input stream from reading from a binary source.
 * the source should be in intel little endian to be compatible
 * with the IOutputStream
 */
class MFileInputStream :
	public ISeekableInputStream
{
protected:

	/**
	 * ansi c file handle
	 *
	 *	invalid == -1
	 *
	 */
	int ivFileHandle;

	/**
	 * ansi c stream handle
	 *
	 * invalid == 0
	 *
	 */
	FILE* ivPtStreamHandle;

	/**
	 * the length of the file for
	 * checking read operations
	 */
	long ivFileLength;

public:

	/**
	 * constructor
	 */
	MFileInputStream();

	/**
	 * destructor
	 */
	virtual ~MFileInputStream();

	/**
	 * opens a file for reading
	 */
	virtual void open( String fileName );

	/**
	 * closes the current opened file
	 */
	virtual void close();

	/**
	 * returns the length of file
	 */
	virtual unsigned int getFileLength();

	/**
	 * returns the position of the file pointer
	 */
	virtual unsigned int getPosition();

	/**
	 * seeks to the given position if possible
	 */
	virtual void seek( unsigned int position );

	/**
	 * reads a signed int with 4 bytes little endian
	 */
	virtual void read( signed int& out );

	/**
	 * reads a unsigned int with 4 bytes little endian
	 */
	virtual void read( unsigned int &out );

	/**
	 * reads a signed short with 2 bytes little endian
	 */
	virtual void read( signed short& out );

	/**
	 * reads a unsigned short with 2 bytes little endian
	 */
	virtual void read( unsigned short& out );

	/**
	 * reads a signed char
	 */
	virtual void read( signed char& out );

	/**
	 * reads a unsigned char
	 */
	virtual void read( unsigned char& out );

	/**
	 * reads a amount of data
	 */
	virtual void read( signed char* ptData, unsigned int len );

	/**
	 * reads a amount of data
	 */
	virtual void read( unsigned char* ptData, unsigned int len );
};

#endif