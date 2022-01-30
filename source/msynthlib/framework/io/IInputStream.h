#ifndef __IInputStream
#define __IInputStream

/**
 * defines an input stream from reading from a binary source.
 * the source should be in intel little endian to be compatible
 * with the IOutputStream
 */
class IInputStream
{
public:

	/**
	 * reads a signed int with 4 bytes little endian
	 */
	virtual void read( signed int& out ) = 0;

	/**
	 * reads a unsigned int with 4 bytes little endian
	 */
	virtual void read( unsigned int &out ) = 0;

	/**
	 * reads a signed short with 2 bytes little endian
	 */
	virtual void read( signed short& out ) = 0;

	/**
	 * reads a unsigned short with 2 bytes little endian
	 */
	virtual void read( unsigned short& out ) = 0;

	/**
	 * reads a signed char
	 */
	virtual void read( signed char& out ) = 0;

	/**
	 * reads a unsigned char
	 */
	virtual void read( unsigned char& out ) = 0;

	/**
	 * reads a amount of data
	 */
	virtual void read( signed char* ptData, unsigned int len ) = 0;

	/**
	 * reads a amount of data
	 */
	virtual void read( unsigned char* ptData, unsigned int len ) = 0;
};

#endif