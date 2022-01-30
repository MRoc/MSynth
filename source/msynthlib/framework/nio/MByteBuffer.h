#ifndef __MByteBuffer
#define __MByteBuffer

#include "MStaticBytebuffer.h"

class MByteBuffer : public MStaticByteBuffer
{
protected:

	unsigned int ivIncrement;

public:

	MByteBuffer( unsigned int  length, unsigned int  increment );
	virtual ~MByteBuffer();

	virtual void append( unsigned char byte );
	virtual void append( unsigned char* ptBytes, unsigned int  length );

	virtual void trim();

protected:

	virtual void setLength( unsigned int  newLength );
	virtual void increment( unsigned int  multi );
};

#endif