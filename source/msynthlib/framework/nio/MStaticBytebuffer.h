#ifndef __MStaticByteBuffer
#define __MStaticByteBuffer

class MStaticByteBuffer
{
protected:

	static const unsigned char CHECKSUM_END;

	unsigned int ivBufferLength;
	unsigned int ivBufferFillLength;
	unsigned char * ivPtBuffer;

public:

	MStaticByteBuffer( unsigned int bLen );
	virtual ~MStaticByteBuffer();

	virtual unsigned int getLength();
	virtual unsigned int getFill();
	virtual unsigned char* getData();
	virtual unsigned char get( unsigned int index );
	virtual void setFill( unsigned int fill );
	virtual bool checksum();

protected:

	virtual unsigned int getLeftInBuffer();
};

#endif