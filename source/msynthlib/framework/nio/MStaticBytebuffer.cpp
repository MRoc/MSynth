#include "MStaticByteBuffer.h"
#include "../io/MLogger.h"

const unsigned char MStaticByteBuffer::CHECKSUM_END = 1;

MStaticByteBuffer::MStaticByteBuffer( unsigned int bLen ) :
	ivBufferLength( bLen ),
	ivBufferFillLength( 0 ),
	ivPtBuffer( 0 )
{
	ivPtBuffer = new unsigned char[ ivBufferLength + 1 ];
	ivPtBuffer[ ivBufferLength ] = CHECKSUM_END;
}

MStaticByteBuffer::~MStaticByteBuffer()
{
	if( ! checksum() )
	{
		MLogger::logError(
			"!!!FATAL CHECKSUM ERROR at MStaticByteBuffer::~MStaticByteBuffer()!!!" );
	}
	delete ivPtBuffer;
}

unsigned int MStaticByteBuffer::getLength()
{
	return ivBufferLength;
}

unsigned int MStaticByteBuffer::getFill()
{
	return ivBufferFillLength;
}

unsigned char* MStaticByteBuffer::getData()
{
	return ivPtBuffer;
}

unsigned char MStaticByteBuffer::get( unsigned int index )
{
	ASSERT( index < ivBufferLength );
	ASSERT( index < ivBufferFillLength );
	return ivPtBuffer[ index ];
}

void MStaticByteBuffer::setFill( unsigned int fill )
{
	ivBufferFillLength = fill;
}

bool MStaticByteBuffer::checksum()
{
	return ivPtBuffer[ ivBufferLength ] == CHECKSUM_END;
}

unsigned int MStaticByteBuffer::getLeftInBuffer()
{
	return ivBufferLength-ivBufferFillLength;
}
