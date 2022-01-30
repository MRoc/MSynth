#ifndef __ISeekableInputStream
#define __ISeekableInputStream

#include "IInputStream.h"

/**
 * defines a seekable input stream, means,
 * the read position can be set
 */
class ISeekableInputStream :
	public IInputStream
{
public:

	/**
	 * returns the length of the stream
	 */
	virtual unsigned int getFileLength() = 0;

	/**
	 * returns the position of the file pointer
	 */
	virtual unsigned int getPosition() = 0;

	/**
	 * seeks in stream
	 */
	virtual void seek( unsigned int position ) = 0;
};

#endif