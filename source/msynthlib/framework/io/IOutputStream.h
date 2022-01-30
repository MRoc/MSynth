#ifndef __IOutputStream
#define __IOutputStream

/**
 * defines a output stream able writing som datatypes out.
 * all types bigger than one byte should be written in
 * intel little endian, means the smallest byte at first.
 */
class IOutputStream
{
public:

	/**
	 * writes the given int with 4 bytes intel little endian
	 */
	virtual void write( int value ) = 0;

	/**
	 * writes the given unsigned int with 4 bytes intel little endian
	 */
	virtual void write( unsigned int value ) = 0;

	/**
	 * writes the given short with 2 bytes intel little endian
	 */
	virtual void write( short value ) = 0;

	/**
	 * writes the given unsigned short with 2 bytes intel little endian
	 */
	virtual void write( unsigned short value ) = 0;

	/**
	 * writes the given char
	 */
	virtual void write( char value ) = 0;

	/**
	 * writes the given unsigned char
	 */
	virtual void write( unsigned char value ) = 0;

	/**
	 * writes the given char data with the given length
	 */
	virtual void write( const char* data, unsigned int length ) = 0;

	/**
	 * writes the given unsigned char data with the given length
	 */
	virtual void write( const unsigned char* data, unsigned int length ) = 0;
};

#endif