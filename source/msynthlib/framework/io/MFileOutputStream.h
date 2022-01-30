#ifndef __MFileOutputStream
#define __MFileOutputStream

#include "ISeekableOutputStream.h"
#include "../MTypes.h"

/**
 * implements a output stream able writing som datatypes out.
 * all types bigger than one byte should be written in
 * intel little endian, means the smallest byte at first.
 */
class MFileOutputStream :
	public ISeekableOutputStream
{
protected:

	/**
	 * the anci c file handle
	 */
	FILE* ivPtFile;

public:

	/**
	 * constructor
	 */
	MFileOutputStream();

	/**
	 * destructor
	 */
	virtual ~MFileOutputStream();

	/**
	 * opens the specified file for writing
	 */
	virtual void open( String filename );

	/**
	 * closes the currently opened file
	 */
	virtual void close();

	/**
	 * seeks in file
	 */
	virtual void seek( unsigned int position );

	/**
	 * writes the given int with 4 bytes intel little endian
	 */
	virtual void write( int value );

	/**
	 * writes the given unsigned int with 4 bytes intel little endian
	 */
	virtual void write( unsigned int value );

	/**
	 * writes the given short with 2 bytes intel little endian
	 */
	virtual void write( short value );

	/**
	 * writes the given unsigned short with 2 bytes intel little endian
	 */
	virtual void write( unsigned short value );

	/**
	 * writes the given char
	 */
	virtual void write( char value );

	/**
	 * writes the given unsigned char
	 */
	virtual void write( unsigned char value );

	/**
	 * writes the given char data with the given length
	 */
	virtual void write( const char* data, unsigned int length );

	/**
	 * writes the given unsigned char data with the given length
	 */
	virtual void write( const unsigned char* data, unsigned int length );
};

#endif