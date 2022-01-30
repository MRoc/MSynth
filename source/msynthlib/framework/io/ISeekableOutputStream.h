#ifndef __ISeekableOutputStream
#define __ISeekableOutputStream

#include "IOutputStream.h"

/**
 * defines a seekable output stream, means,
 * the write position can be set
 */
class ISeekableOutputStream :
	public IOutputStream
{
public:

	/**
	 * seeks in stream
	 */
	virtual void seek( unsigned int position ) = 0;
};

#endif