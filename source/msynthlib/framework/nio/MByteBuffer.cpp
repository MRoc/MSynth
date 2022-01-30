#include "MByteBuffer.h"
#include "../io/MLogger.h"

MByteBuffer::MByteBuffer( unsigned int length, unsigned int increment ) :
	MStaticByteBuffer( length ),
    ivIncrement( increment )
{
}

MByteBuffer::~MByteBuffer()
{
}

void MByteBuffer::append( unsigned char byte )
{
	if( ivBufferFillLength+1 >= ivBufferLength )
		increment( 1 );
	ivPtBuffer[ ivBufferFillLength++ ] = byte;
}

void MByteBuffer::append( unsigned char* ptBytes, unsigned int length )
{
	if( ivBufferFillLength + length >= ivBufferLength )
	{
		unsigned int multi = ((length - getLeftInBuffer()) / ivIncrement) + 1;
		if( ! multi )
			MLogger::logError( "MByteBuffer::append: multi == 0" );
		increment( multi );
		if( ! (ivBufferFillLength + length < ivBufferLength) )
			MLogger::logError( "MByteBuffer::append: ivBufferFillLength + length < ivBufferLength == 0" );
	}
	memcpy( ivPtBuffer + ivBufferFillLength, ptBytes, length );
	ivBufferFillLength += length;
}

void MByteBuffer::trim()
{
	if( ivBufferFillLength < ivBufferLength )
		setLength( ivBufferFillLength );
}

void MByteBuffer::setLength( unsigned int newLength )
{
	unsigned char* ptBuffer = new unsigned char[ newLength + 1];
	ptBuffer[ newLength ] = CHECKSUM_END;
	memcpy( ptBuffer, ivPtBuffer, ivBufferFillLength );

	delete ivPtBuffer;
	ivPtBuffer = ptBuffer;
	ivBufferLength = newLength;
}

void MByteBuffer::increment( unsigned int multi )
{
	setLength( ivBufferLength + multi * ivIncrement );
}
