#include "MSoundBufferUtils.h"

/**
 * does a stereo multiplication
 *
 * left - the left multiplicator
 * right - the right multiplicator
 */
void MSoundBufferUtils::stereoMultiply( MSoundBuffer* pBuffer, float left, float right )
{
	ASSERT( pBuffer->getChannelCount() == 2 );

	MSoundBufferUtils::stereoMultiply(
		pBuffer,
		0,
		pBuffer->getDataLength(),
		left,
		right );
}

/**
 * pans and volumes the given soundbuffer,
 * uses MSoundBufferUtils::stereoMultiply.
 *
 * volume - the multiplicator
 * pan - the panning between 0.0f and 1.0f
 */
void MSoundBufferUtils::steroVolume( MSoundBuffer* pBuffer, float volume, float pan )
{
	FP left = pan > 0.5f ? 1.0f - (2.0f*(pan-0.5f)) : 1.0f;
	FP right = pan < 0.5f ? (2.0f*pan) : 1.0f;
	left *= volume;
	right *= volume;

	stereoMultiply( pBuffer, left, right );
}

/**
 * pans and volumes the given soundbuffer,
 * uses MSoundBufferUtils::stereoMultiply.
 *
 * volume - the multiplicator
 * pan - the panning between 0.0f and 1.0f
 */
void MSoundBufferUtils::steroVolume(
	MSoundBuffer* pBuffer,
	unsigned int startAt,
	unsigned int stopAt,
	float volume,
	float pan )
{
	FP left = pan > 0.5f ? 1.0f - (2.0f*(pan-0.5f)) : 1.0f;
	FP right = pan < 0.5f ? (2.0f*pan) : 1.0f;
	left *= volume;
	right *= volume;

	MSoundBufferUtils::stereoMultiply(
		pBuffer,
		startAt,
		stopAt,
		left,
		right );
}

/**
 * does a stereo multiplication
 *
 * left - the left multiplicator
 * right - the right multiplicator
 */
void MSoundBufferUtils::stereoMultiply(
	MSoundBuffer* pBuffer,
	unsigned int startAt,
	unsigned int stopAt,
	float left,
	float right )
{
	ASSERT( pBuffer->getChannelCount() == 2 );
	ASSERT( startAt <= stopAt );

	FP* pSrc1 = pBuffer->getData( 0 ) + startAt;
	FP* pSrc2 = pBuffer->getData( 1 ) + startAt;
	FP* pStop1 = pSrc1 + (stopAt - startAt);
	for( ;
		pSrc1<pStop1;
		pSrc1++, pSrc2++ )
	{
		(*pSrc1) *= left;
		(*pSrc2) *= right;
	}
}

/**
 * adds pBuffer2 to pBuffer1 at the given
 * index and the given amount of samples
 */
void MSoundBufferUtils::add(
	MSoundBuffer* pBuffer1,
	MSoundBuffer* pBuffer2,
	unsigned int insertPos,
	unsigned int length )
{
	ASSERT( pBuffer1 );
	ASSERT( pBuffer2 );
	ASSERT( insertPos + length <= pBuffer1->getDataLength() );
	ASSERT( length <= pBuffer2->getDataLength() );

	unsigned int channelCount1 = pBuffer1->getChannelCount();
	unsigned int channelCount2 = pBuffer2->getChannelCount();

	if( channelCount1 == channelCount2 )
	{
		for( unsigned int i=0;i<channelCount1;i++ )
		{
			FP* pSrc = pBuffer2->getData( i );
			FP* pTar = pBuffer1->getData( i ) + insertPos;
			FP* pStop = pTar + length;
			for( ;pTar<pStop;pTar++,pSrc++ )
				(*pTar) += (*pSrc);
		}
	}
	else
		ASSERT( false );
}

/**
 * adds buffer 2 to buffer 1
 */
void MSoundBufferUtils::add(
	MSoundBuffer* pBuffer1,
	MSoundBuffer* pBuffer2,
	FP mixValue )
{
	ASSERT( pBuffer1 );
	ASSERT( pBuffer2 );
	ASSERT( pBuffer1->getDataLength() == pBuffer2->getDataLength() );

	if( ( pBuffer1->getChannelCount() == pBuffer2->getChannelCount() ) ||
		( pBuffer1->getChannelCount() == 1 && pBuffer2->getChannelCount() == 2 ) )
	{
		for( unsigned int i=0;i<pBuffer1->getChannelCount();i++ )
		{
			FP* ptSrc = pBuffer2->getData( i );
			FP* ptTar = pBuffer1->getData( i );
			FP* ptEnd = ptTar + pBuffer1->getDataLength();
			for( ; ptTar<ptEnd; ptTar++, ptSrc++ )
				(*ptTar) += (*ptSrc) * mixValue;
		}
	}
	else if( pBuffer1->getChannelCount() == 2 && pBuffer2->getChannelCount() == 1 )
	{
		FP* sourceData = pBuffer2->getData( 0 );
		FP* pointer1 = pBuffer1->getData( 0 );
		FP* pointer2 = pBuffer1->getData( 1 );
		FP* bufferEnd = pointer1 + pBuffer1->getDataLength();
		FP temp;
		for( ; pointer1<bufferEnd; pointer1++, pointer2++, sourceData++ )
		{
			temp = (*sourceData) * mixValue;
			(*pointer1) += temp;
			(*pointer2) += temp;
		}
	}
}

/**
 * adds buffer 2 to buffer 1 with the mix and pan value
 */
void MSoundBufferUtils::addStereo(
	MSoundBuffer* pBuffer1,
	MSoundBuffer* pBuffer2,
	FP vol,
	FP pan )
{
	ASSERT( pBuffer1->getChannelCount() == 2 );
	ASSERT( pBuffer1->getDataLength() == pBuffer2->getDataLength() );
	FP volume[ 2 ];
	volume[ 0 ] = pan > 0.5f ? 1.0f - (2.0f*(pan-0.5f)) : 1.0f;
	volume[ 1 ] = pan < 0.5f ? (2.0f*pan) : 1.0f;
	volume[ 0 ] *= vol;
	volume[ 1 ] *= vol;

	if( pBuffer1->getChannelCount() == 2 && pBuffer2->getChannelCount() == 2 )
	{
		for( unsigned int i=0;i<pBuffer1->getChannelCount();i++ )
		{
			FP amp = volume[ i ];
			FP *sourceData = pBuffer2->getData( i );
			FP *bufferEnd = pBuffer1->getData( i ) + pBuffer1->getDataLength();
			for( FP *pointer=pBuffer1->getData( i );
				pointer<bufferEnd;
				pointer++, sourceData++ )
			{
				(*pointer) += (*sourceData) * amp;
			}
		}
	}
	else if( pBuffer1->getChannelCount() == 2 && pBuffer2->getChannelCount() == 1 )
	{
		for( unsigned int i=0;i<pBuffer1->getChannelCount();i++ )
		{
			FP amp = volume[ i ];
			FP *sourceData = pBuffer2->getData( 0 );
			FP *bufferEnd = pBuffer1->getData( i ) + pBuffer1->getDataLength();
			for( FP *pointer=pBuffer1->getData( i );
				pointer<bufferEnd;
				pointer++, sourceData++ )
			{
				(*pointer) += (*sourceData) * amp;
			}
		}
	}
}


/**
 * multiplies the given buffer with the given
 * multiplicator from start to stop.
 */
void MSoundBufferUtils::multiply(
	MSoundBuffer* pBuffer,
	unsigned int start,
	unsigned int stop,
	float multiplicator )
{
	for( unsigned int i=0;i<pBuffer->getChannelCount();i++ )
	{
		FP* pt = pBuffer->getData( i ) + start;
		FP* ptStop = pt + (stop-start);
		for(;pt<ptStop;pt++ )
			(*pt) *= multiplicator;
	}
}

/**
 * multiplies the given buffer with the given multiplicator
 */
void MSoundBufferUtils::multiply(
	MSoundBuffer* pBuffer,
	float multiplicator )
{
	MSoundBufferUtils::multiply( pBuffer, 0, pBuffer->getDataLength(), multiplicator );
}

/**
 * pastes buffer 2 into buffer 1
 */
void MSoundBufferUtils::pasteBuffer(
	MSoundBuffer* pBuffer1,
	MSoundBuffer* pBuffer2,
	unsigned int pos,
	unsigned int len )
{
	ASSERT( pBuffer1 );
	ASSERT( pBuffer2 );
	ASSERT( len <= pBuffer2->getDataLength() );
	ASSERT( pos + len <= pBuffer1->getDataLength() );

	if( pBuffer2->getChannelCount() == pBuffer1->getChannelCount() )
	{
		for( unsigned int i=0;i<pBuffer1->getChannelCount();i++ )
		{
			memcpy(
				(void*)(pBuffer1->getData( i ) + pos),
				(const void*)(pBuffer2->getData( i )),
				len * sizeof( FP ) );
		}
	}
	else if( pBuffer2->getChannelCount() == 1 && pBuffer1->getChannelCount() == 2 )
	{
		memcpy(
			(void*)(pBuffer1->getData( 0 ) + pos),
			(const void*)(pBuffer2->getData( 0 )),
			len * sizeof( FP ) );
		memcpy(
			(void*)(pBuffer1->getData( 1 ) + pos),
			(const void*)(pBuffer2->getData( 0 )),
			len * sizeof( FP ) );
	}
	else if( pBuffer2->getChannelCount() == 2 && pBuffer1->getChannelCount() == 1 )
	{
		memcpy(
			(void*)(pBuffer1->getData( 0 ) + pos),
			(const void*)(pBuffer2->getData( 0 )),
			len * sizeof( FP ) );
	}
	else
		ASSERT( false );

}

/**
 * fades a part of the sound buffer
 */
double MSoundBufferUtils::fade( MSoundBuffer* pBuffer, unsigned int start, unsigned int stop, double offset, double alpha )
{
	ASSERT( pBuffer );
	ASSERT( stop >= start );

	double multi = offset;
	for( unsigned int i=0;i<pBuffer->getChannelCount();i++ )
	{
		FP* pt = pBuffer->getData( i ) + start;
		FP* pStop = pt + (stop - start);
		multi = offset;
		for(;pt<pStop;pt++)
		{
			(*pt) *= (float) multi;
			multi += alpha;
		}
	}
	return multi;
}

float MSoundBufferUtils::findMax( MSoundBuffer* ptBuffer )
{
	float max = (float)-1e20;
	for( unsigned int i=0;i<ptBuffer->getChannelCount();i++ )
	{
		FP* ptStart = ptBuffer->getData( i );
		FP* ptStop = ptStart + ptBuffer->getDataLength();
		for( FP* pt=ptStart;pt<ptStop;pt++ )
			if( (*pt) > max )
				max = (*pt);

	}
	return max;
}

float MSoundBufferUtils::findMin( MSoundBuffer* ptBuffer )
{
	float min = (float)+1e20;
	for( unsigned int i=0;i<ptBuffer->getChannelCount();i++ )
	{
		FP* ptStart = ptBuffer->getData( i );
		FP* ptStop = ptStart + ptBuffer->getDataLength();
		for( FP* pt=ptStart;pt<ptStop;pt++ )
			if( (*pt) < min )
				min = (*pt);

	}
	return min;
}

void MSoundBufferUtils::sinegen( MSoundBuffer* ptBuffer, float frequency, float phase, float dc, float amp )
{
	sinegen( ptBuffer, frequency, phase, dc, amp, MW_SAMPLINGRATE );
}

void MSoundBufferUtils::sinegen( MSoundBuffer* ptBuffer, float frequency, float phase, float dc, float amp, unsigned int samplingRate )
{
	if( frequency > samplingRate / 2 )
		return;

	double lvSin, lvCos, x, y;
	FP tmp = frequency * 2 * PI / samplingRate;
	lvSin = sin( tmp );
	lvCos = cos( tmp );
	x = cos( phase * 2 * PI );
	y = sin( phase * 2 * PI );
	if( ptBuffer->getChannelCount() == 1 )
	{
		FP* ptStart = ptBuffer->getData(0);
		FP* ptStop = ptStart + ptBuffer->getDataLength();
		for( FP *pt = ptStart; pt < ptStop; pt++)
		{
			double xTmp = x, yTmp = y;
			x = lvCos*xTmp + lvSin*yTmp;
			y = -lvSin*xTmp + lvCos*yTmp;
			(*pt) = dc + FP(x) * amp;
		}
	}
	else if( ptBuffer->getChannelCount() == 2 )
	{
		FP* ptStart1 = ptBuffer->getData(0);
		FP* ptStop1 = ptStart1 + ptBuffer->getDataLength();
		FP* ptStart2 = ptBuffer->getData(1);
		FP* ptStop2 = ptStart2 + ptBuffer->getDataLength();
		FP* pt1 = ptStart1;
		FP* pt2 = ptStart2;
		for( ; pt1 < ptStop1; pt1++, pt2++ )
		{
			double xTmp = x, yTmp = y;
			x = lvCos*xTmp + lvSin*yTmp;
			y = -lvSin*xTmp + lvCos*yTmp;
			(*pt1) = dc + FP(x) * amp;
			(*pt2) = dc + FP(x) * amp;
		}
	}
}

void MSoundBufferUtils::saveBufferAsWave( String filename, MSoundBuffer* ptBuffer, float amp )
{
	WaveFileHandling f;
	f.openFile( filename );
	if( ptBuffer->getChannelCount() == 1 )
	{
		f.setFormat( MW_SAMPLINGRATE, 16, 1 );

		FP* ptStart = ptBuffer->getData(0);
		FP* ptStop = ptStart + ptBuffer->getDataLength();
		for( FP *pt = ptStart; pt < ptStop; pt++)
		{
			float v = (*pt);
			if( v > MW_MAX )
				v = MW_MAX;
			else if( v < MW_MIN )
				v = MW_MIN;
			MW_SAMPLETYPE stv = (MW_SAMPLETYPE) v;
			f.writeData( ((unsigned char*)(&stv)), 2 );
		}
	}
	else if( ptBuffer->getChannelCount() == 2 )
	{
		f.setFormat( MW_SAMPLINGRATE, 16, 2 );

		FP* ptStart1 = ptBuffer->getData(0);
		FP* ptStop1 = ptStart1 + ptBuffer->getDataLength();
		FP* ptStart2 = ptBuffer->getData(1);
		FP* ptStop2 = ptStart2 + ptBuffer->getDataLength();
		FP* pt1 = ptStart1;
		FP* pt2 = ptStart2;
		for( ; pt1 < ptStop1; pt1++, pt2++ )
		{
			float v1 = (*pt1);
			if( v1 > MW_MAX )
				v1 = MW_MAX;
			else if( v1 < MW_MIN )
				v1 = MW_MIN;
			MW_SAMPLETYPE stv1 = (MW_SAMPLETYPE) v1;
			f.writeData( ((unsigned char*)(&stv1)), 2 );

			float v2 = (*pt2);
			if( v2 > MW_MAX )
				v2 = MW_MAX;
			else if( v2 < MW_MIN )
				v2 = MW_MIN;
			MW_SAMPLETYPE stv2 = (MW_SAMPLETYPE) v2;
			f.writeData( ((unsigned char*)(&stv2)), 2 );
		}
	}
	f.finalizeFile();
	f.closeFile();
}