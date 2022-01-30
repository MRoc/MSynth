#include "MPeakFile.h"

/**
 * constructor
 */
MPeakFile::MPeakFile() :
	ivPtPeakData( 0 ),
	ivChannelCount( 0 )
{
}

/**
 * destructor
 */
MPeakFile::~MPeakFile()
{
	reset();
}

/**
 * returns the number of channels stored in the peakfile
 */
unsigned int MPeakFile::getChannelCount()
{
	return this->ivChannelCount;
}

/**
 * returns the number of peaks stored in
 * the buffers. One peak is 2 bytes!
 */
unsigned int MPeakFile::getPeakCount()
{
	if( this->ivPtPeakData )
		return ivPtPeakData[ 0 ]->getLength() / 2;
	else
		return 0;
}


/**
 * returns the peak data for the indexed channel
 */
MByteBuffer* MPeakFile::getData( unsigned int channel )
{
	ASSERT( ivPtPeakData );
	ASSERT( channel < ivChannelCount );
	return this->ivPtPeakData[ channel ];
}

/**
 * resets the peak file, frees all resources
 */
void MPeakFile::reset()
{
	if( ivPtPeakData )
	{
		for( unsigned int i=0;i<ivChannelCount;i++ )
			delete ivPtPeakData[ i ];
		delete ivPtPeakData;
		ivPtPeakData = 0;
		ivChannelCount = 0;
	}
}

/**
 * sets the data used in the peak file.
 * only used when other objects like the MAudioFile
 * is generating the peak and putting it into
 * the peak file.
 */
void MPeakFile::setData( MByteBuffer** ptPeaks, unsigned int channelCount )
{
	ASSERT( ! ivPtPeakData );
	ivPtPeakData = ptPeaks;
	ivChannelCount = channelCount;
}

/**
 * analyzes an amount of mono wave pcm data.
 * this method generates the required buffers and
 * does all the rest.
 *
 * ptWaveData - the raw amount of wave data (signed16bit)
 * dataLength - the length of the wave data in bytes
 * frameCount - the number of peaks that should be generated
 */
void MPeakFile::analyzeMono(
	unsigned char* ptWaveData,
	unsigned int dataLength, 
	unsigned int frameCount )
{
	unsigned int i;

	if( ivPtPeakData )
	{
		for( unsigned int i=0;i<ivChannelCount;i++ )
			delete ivPtPeakData[ i ];
		delete ivPtPeakData;
	}

	unsigned int cCount = 1;
	this->ivChannelCount = cCount;
	ivPtPeakData = new MByteBuffer*[cCount];
	for( i=0;i<cCount;i++ )
		ivPtPeakData[ i ] = new MByteBuffer( frameCount * 2, 10 );

	ASSERT( ivPtPeakData );

	unsigned int bytesPerSample = 2; // 16 bit
	unsigned int stepWidthInBytes = dataLength/ frameCount;
	unsigned int stepWidthInSamples = stepWidthInBytes/bytesPerSample;
	unsigned int offset = 0;
	for( i=0;i<frameCount;i++ )
	{
		analyzeMonoFrame(
			ptWaveData + offset,
			stepWidthInBytes,
			this->ivPtPeakData[0] );
		offset += stepWidthInBytes;
	}
}

/**
 * analyze a wave pcm frame given in the byte* array.
 * the maximum and minimum found in the frame are
 * pushed into the bytebuffer.
 */
void MPeakFile::analyzeMonoFrame( unsigned char* ptFrameData, unsigned int frameLen, MByteBuffer* ptPeaks )
{
	unsigned int samplesInBuffer = frameLen / sizeof( MW_SAMPLETYPE );
	MW_SAMPLETYPE* ptSrc = (MW_SAMPLETYPE*) ptFrameData;
	MW_SAMPLETYPE* ptEnd = ptSrc + samplesInBuffer;
	MW_SAMPLETYPE
		min1 = (MW_SAMPLETYPE)MW_MAX,
		max1 = (MW_SAMPLETYPE)-MW_MAX;

	for( MW_SAMPLETYPE* pt=ptSrc;pt<ptEnd;pt++ )
	{
		if( (*pt) < min1 )
			min1 = (*pt);
		if( (*pt) > max1 )
			max1 = (*pt);
	}

	/*if( min1 < 0 )
		min1 *= (-1);
	if( max1 < 0 )
		max1 *= (-1);*/
	min1 >>= 8;
	max1 >>= 8;

	ptPeaks->append( (unsigned char) max1 );
	ptPeaks->append( (unsigned char) min1 );
}

/**
 * analyze a stereo wave pcm frame given in the byte* array.
 * the maximum and minimum found in the frame are
 * pushed into the bytebuffer. the first channel
 * is pushed in the peaks left buffer, the second
 * channel into the right one.
 */
void MPeakFile::analyzeStereoFrame( unsigned char* ptFrameData, unsigned int frameLen, MByteBuffer* ptPeaksLeft, MByteBuffer* ptPeaksRight )
{
	unsigned int samplesInBuffer = frameLen / sizeof( MW_SAMPLETYPE );
	MW_SAMPLETYPE* ptSrc = (MW_SAMPLETYPE*) ptFrameData;
	MW_SAMPLETYPE* ptEnd = ptSrc + samplesInBuffer;
	MW_SAMPLETYPE
		min1 = (MW_SAMPLETYPE)MW_MAX,
		max1 = (MW_SAMPLETYPE)-MW_MAX,
		min2 = (MW_SAMPLETYPE)MW_MAX,
		max2 = (MW_SAMPLETYPE)-MW_MAX;

	for( MW_SAMPLETYPE* pt=ptSrc;pt<ptEnd;pt++ )
	{
		// analyze left peak
		if( (*pt) < min1 )
			min1 = (*pt);
		if( (*pt) > max1 )
			max1 = (*pt);
		pt++;
		// analyze right peak
		if( (*pt) < min2 )
			min2 = (*pt);
		if( (*pt) > max2 )
			max2 = (*pt);
	}

	/*if( min1 < 0 )
		min1 *= (-1);
	if( max1 < 0 )
		max1 *= (-1);
	if( min2 < 0 )
		min2 *= (-1);
	if( max2 < 0 )
		max2 *= (-1);*/
	min1 >>= 8;
	max1 >>= 8;
	min2 >>= 8;
	max2 >>= 8;
	ptPeaksLeft->append( (unsigned char) max1 );
	ptPeaksLeft->append( (unsigned char) min1 );
	ptPeaksRight->append( (unsigned char) max2 );
	ptPeaksRight->append( (unsigned char) min2 );
}