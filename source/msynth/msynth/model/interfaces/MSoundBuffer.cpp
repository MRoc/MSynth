/**
 * MSoundBuffer (C)2000-2003 MRoc
 */

#include "MSoundBuffer.h"
#include <math.h>

#define CLIP( value )\
{\
	if((value)>MW_MAX )\
	{\
		(value) = MW_MAX;\
	}\
	else if((value)< -MW_MAX)\
	{\
		(value)=-MW_MAX;\
	}\
}

#define ROUND( value )\
{\
	if( ((int)(value)) > ((int)(value))+1 )\
		value = ((int)(value));\
	else (value) = ((int)(value))+1;\
}

//#define ROUND( value ){}

/** constructor */
MSoundBuffer::MSoundBuffer( unsigned int channelCount, unsigned int dataLength ) :
	ivData( 0 ),
	ivChannelCount( channelCount ),
	ivDataLength( dataLength )
{
	createBuffer();
	zeroBuffer();
}

/** constructor */
MSoundBuffer::MSoundBuffer( MSoundBuffer* buffer ) :
	ivData( 0 ),
	ivChannelCount( buffer->ivChannelCount ),
	ivDataLength( buffer->ivDataLength )
{
	createBuffer();
	zeroBuffer();
}

/** destructor */
MSoundBuffer::~MSoundBuffer()
{
	deleteBuffer();
}

/** returns the channel data */
FP* MSoundBuffer::getData( unsigned int index )
{
	ASSERT( ivData );
	ASSERT( index < ivChannelCount );
	return ivData[ index ];
}

/** returns the number of channels */
unsigned int MSoundBuffer::getChannelCount()
{
	return ivChannelCount;
}

/** returns the data's length */
unsigned int MSoundBuffer::getDataLength()
{
	return ivDataLength;
}

/** initializes the buffer and installs a checksum */
void MSoundBuffer::createBuffer()
{
	if( ivData )
		delete ivData;
	ivData = new FP*[ ivChannelCount ];
	for( unsigned int i=0;i<ivChannelCount;i++ )
	{
		ivData[i] = new FP[ ivDataLength+1 ];
		ivData[i][ivDataLength] = -1; // <- set last step code to prevent array index out of bounds
	}
}

/** checks the checksum and deletes the buffer. */
void MSoundBuffer::deleteBuffer()
{
	for( unsigned int i=0;i<ivChannelCount;i++ )
	{
		ASSERT( ivData[i][ivDataLength] == -1 ); // <- assert last step is the same
		delete ivData[i];
	}
	delete ivData;
	ivData = 0;
}

/** returns true if the buffer contains any data */
bool MSoundBuffer::isBufferValid()
{
	return ivData != NULL;
}

/** zerosizes the buffer */
void MSoundBuffer::zeroBuffer()
{
	for( unsigned int i=0;i<ivChannelCount;i++ )
		memset( ivData[ i ], 0, ivDataLength * sizeof( FP ) );
}

//----------------------------------------------------------------------
// + addBuffer
//	adds the given buffer to this buffer
//----------------------------------------------------------------------
/*void MSoundBuffer::addBuffer( MSoundBuffer* buffer )
{
	ASSERT( ivDataLength == buffer->ivDataLength );
	if( ( ivChannelCount == buffer->ivChannelCount ) ||
		( ivChannelCount == 1 && buffer->ivChannelCount == 2 ) )
	{
		for( unsigned int i=0;i<ivChannelCount;i++ )
		{
			FP *sourceData = buffer->ivData[ i ];
			FP *bufferEnd = ivData[ i ] + ivDataLength;
			for( FP *pointer=ivData[ i ]; pointer<bufferEnd; pointer++, sourceData++ )
				(*pointer) += (*sourceData);
		}
	}
	else if( ivChannelCount == 2 && buffer->ivChannelCount == 1 )
	{
		FP *sourceData = buffer->ivData[ 0 ];
		FP *bufferEnd = ivData[ 0 ] + ivDataLength;
		FP *pointer1=ivData[ 0 ];
		FP *pointer2=ivData[ 1 ];
		for( ; pointer1<bufferEnd; pointer1++, pointer2++, sourceData++ )
		{
			(*pointer1) += (*sourceData);
			(*pointer2) += (*sourceData);
		}
	}
}*/

void MSoundBuffer::fadeOut( unsigned int start, unsigned int stop )
{
	ASSERT( start < this->ivDataLength );
	ASSERT( stop <= this->ivDataLength );
	ASSERT( start < stop );

	unsigned int len = stop - start;
	FP diff = 1.0f / len;

	for( unsigned int i=0;i<ivChannelCount;i++ )
	{
		FP* ptStart = ivData[ i ] + start;
		FP* ptStop = ptStart + len;
		FP value = 1.0f;
		unsigned int count = 0;
		for( FP* pt=ptStart;pt<ptStop;pt++ )
			(*pt) *= (1.0f-(diff*(count++)));
	}
}

/** the output data is 2 channels pcm coded */
void MSoundBuffer::export2pcm16bit2channel( MW_SAMPLETYPE *ptData )
{
	switch( ivChannelCount )
	{
	case 1:
		{
			//TRACE(  "exporting one channel %u\n", ivDataLength*this->ivChannelCount*sizeof(MW_SAMPLETYPE));

			FP* ptSrc = ivData[0];
			FP* ptStop = ptSrc + ivDataLength;
			MW_SAMPLETYPE* ptDest = ptData;
			FP tmp;
			for( ;ptSrc<ptStop; ptSrc++, ptDest++ )
			{
				tmp = (*ptSrc) * MW_MAX;
				CLIP( tmp );
				ROUND( tmp );
				(*ptDest) = (MW_SAMPLETYPE)tmp;
				(*(++ptDest)) = (MW_SAMPLETYPE)tmp;
			}
		}
		break;
	case 2:
		{
			//TRACE(  "exporting two channels %u\n", ivDataLength*this->ivChannelCount*sizeof(MW_SAMPLETYPE));

			FP* ptSrc1 = ivData[0];
			FP* ptSrc2 = ivData[1];
			FP* ptStop1 = ptSrc1 + ivDataLength;

			MW_SAMPLETYPE* ptDest = ptData;
			FP tmp1, tmp2;
			for(;
				ptSrc1<ptStop1;
				ptSrc1++, ptSrc2++, ptDest++ )
			{
				tmp1 = (*ptSrc1) * MW_MAX;
				tmp2 = (*ptSrc2) * MW_MAX;

				CLIP( tmp1 );
				CLIP( tmp2 );
				ROUND( tmp1 );
				ROUND( tmp2 );

				(*ptDest) = (MW_SAMPLETYPE)tmp1;
				(*(++ptDest)) = (MW_SAMPLETYPE)tmp2;
			}
		}
		break;
	default:
		ASSERT( false );
	}
}

void MSoundBuffer::export2pcm16bit2channel( MW_SAMPLETYPE *ptData, FP pan, FP vol )
{
	FP volLeft = pan > 0.5f ? 1.0f - (2.0f*(pan-0.5f)) : 1.0f;
	FP volRight = pan < 0.5f ? (2.0f*pan) : 1.0f;
	volLeft *= MW_MAX * vol;
	volRight *= MW_MAX * vol;
	FP tmp1, tmp2;

	switch( ivChannelCount )
	{
	case 1:
		{
			FP* ptSrc = ivData[0];
			FP* ptStop = ptSrc + ivDataLength;

			MW_SAMPLETYPE* ptDest = ptData;

			for(;ptSrc<ptStop;
				ptSrc++, ptDest++ )
			{
				tmp1 = (*ptSrc) * volLeft;
				tmp2 = tmp1 * volRight;

				CLIP( tmp1 );
				CLIP( tmp2 );
				ROUND( tmp1 );
				ROUND( tmp2 );

				(*ptDest) = (MW_SAMPLETYPE) tmp1;
				(*(++ptDest)) = (MW_SAMPLETYPE) tmp2;
			}
		}
		break;
	case 2:
		{
			FP* ptSrc1 = ivData[0];
			FP* ptSrc2 = ivData[1];
			FP* ptStop1 = ptSrc1 + ivDataLength;

			MW_SAMPLETYPE* ptDest = ptData;

			for(;ptSrc1<ptStop1;
				ptSrc1++, ptSrc2++, ptDest++ )
			{
				tmp1 = (*ptSrc1) * volLeft;
				tmp2 = (*ptSrc2) * volRight;

				CLIP( tmp1 );
				CLIP( tmp2 );
				ROUND( tmp1 );
				ROUND( tmp2 );

				(*ptDest) = (MW_SAMPLETYPE)tmp1;
				(*(++ptDest)) = (MW_SAMPLETYPE)tmp2;
			}
		}
		break;
	default:
		ASSERT( false );
	}
}

MBYTEBuffer::MBYTEBuffer() : m_size( 0 ), m_data( 0 )
{
}

MBYTEBuffer::~MBYTEBuffer()
{
	if( m_data )
		delete m_data;
}

unsigned char* MBYTEBuffer::getData()
{
	return m_data;
}

unsigned long MBYTEBuffer::getSize()
{
	return m_size;
}

void MBYTEBuffer::importSoundBuffer( MSoundBuffer* ptBuffer, unsigned int start, unsigned int stop )
{
	if( m_data )
		delete m_data;
	unsigned int length = stop - start;

	m_size = 2 * ptBuffer->getChannelCount() * length;
	m_data = new unsigned char[ m_size ];

	if( ptBuffer->getChannelCount() == 1 )
	{
		float multi = 0.8 * MW_MAX;
		FP temp;
		unsigned char* ptEnd = m_data + m_size;
		FP* ptSource = ptBuffer->getData( 0 ) + start;
		signed short* pt = ((signed short*)(m_data));
		signed short* ptStop = ((signed short*)(m_data + m_size));
		for( ;pt<ptStop;pt++,ptSource++ )
		{
			temp = (*ptSource) * multi;
			if( temp > MW_MAX )
				temp = MW_MAX;
			else if( temp < -MW_MAX )
				temp = -MW_MAX;
			(*pt) = (signed short) temp;
		}
	}
	else if( ptBuffer->getChannelCount() == 2 )
	{
		FP* ptSource1 = ptBuffer->getData( 0 ) + start;
		FP* ptSource2 = ptBuffer->getData( 1 ) + start;
		signed short* pt = ((signed short*)(m_data));
		signed short* ptStop = ((signed short*)(m_data + m_size));
		for( ;pt<ptStop;pt+=2, ptSource1++, ptSource2++ )
		{
			(*(pt)) = (signed short)( (*ptSource1) * MW_MAX );
			(*(pt+1)) = (signed short)( (*ptSource2) * MW_MAX );
		}
	}
}

void MBYTEBuffer::exportSoundBuffer( MSoundBuffer* ptBuffer, unsigned int start, unsigned int stop )
{
	if( ptBuffer->getChannelCount() == 1 )
	{
		float multi = 0.8 * MW_MAX;
		FP* pt = ptBuffer->getData( 0 ) + start;
		FP* ptStop = ptBuffer->getData( 0 ) + stop;
		signed short* ptSource = ((signed short*)(m_data));
		for( ;pt<ptStop;pt++, ptSource++ )
		{
			(*pt) = (FP) (*ptSource) / multi;
		}
	}
	else if( ptBuffer->getChannelCount() == 2 )
	{
		FP* pt1 = ptBuffer->getData( 0 ) + start;
		FP* pt2 = ptBuffer->getData( 1 ) + start;
		FP* ptStop = ptBuffer->getData( 0 ) + stop;
		signed short* ptSource = ((signed short*)(m_data));
		for( ;pt1<ptStop;pt1++, pt2++, ptSource+=2 )
		{
			(*pt1) = FP( *(ptSource) ) / MW_MAX;
			(*pt2) = FP( *(ptSource+1) ) / MW_MAX;
		}
	}
}
