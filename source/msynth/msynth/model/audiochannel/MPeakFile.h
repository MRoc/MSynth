#ifndef __MPeakFile
#define __MPeakFile

#include <framework/MTypes.h>
#include <framework/nio/MBytebuffer.h>
#include "../../../MSynthConstants.h"

/**
 * the peak file is used for painting waves onto screen.
 * 
 * the peakfile has two methods, analyzeMonoFrame and
 * analyzeStereoFrame that are able to analyze amount
 * of raw pcm wave data and to search the minimum and
 * maximum in this data. when finished, it pushes the
 * max and min value into the given bytebuffer.
 */
class MPeakFile
{
protected:

	/**
	 * the number of channels stored in the peakfile.
	 */
	unsigned int ivChannelCount;

	/**
	 * the one or more bytebuffers storing peaks.
	 * a peak aligned as follows:
	 *
	 * one peak has two unsigned char,
	 * the first one for the maximum,
	 * the second one for the minimum.
	 *
	 * for every channel, there is one
	 * bytebuffer.
	 */
	MByteBuffer** ivPtPeakData;

public:

	/**
	 * constructor
	 */
	MPeakFile();

	/**
	 * destructor
	 */
	virtual ~MPeakFile();

	/**
	 * returns the number of peaks stored in
	 * the buffers. One peak is 2 bytes!
	 */
	virtual unsigned int getPeakCount();

	/**
	 * returns the number of channels stored in the peakfile
	 */
	virtual unsigned int getChannelCount();

	/**
	 * returns the peak data for the indexed channel
	 */
	virtual MByteBuffer* getData( unsigned int channel );

	/**
	 * analyzes an amount of mono wave pcm data.
	 * this method generates the required buffers and
	 * does all the rest.
	 *
	 * ptWaveData - the raw amount of wave data (signed16bit)
	 * dataLength - the length of the wave data in bytes
	 * frameCount - the number of peaks that should be generated
	 */
	virtual void analyzeMono(
		unsigned char* ptWaveData,
		unsigned int dataLength, 
		unsigned int frameCount );

	/**
	 * resets the peak file, frees all resources
	 */
	virtual void reset();

	/**
	 * sets the data used in the peak file.
	 * only used when other objects like the MAudioFile
	 * is generating the peak and putting it into
	 * the peak file.
	 */
	virtual void setData( MByteBuffer** ptPeaks, unsigned int channelCount );

	/**
	 * analyze a wave pcm frame given in the byte* array.
	 * the maximum and minimum found in the frame are
	 * pushed into the bytebuffer.
	 */
	virtual void analyzeMonoFrame(
		unsigned char* ptToAnalyze,
		unsigned int frameLength,
		MByteBuffer* ptPeaks );

	/**
	 * analyze a stereo wave pcm frame given in the byte* array.
	 * the maximum and minimum found in the frame are
	 * pushed into the bytebuffer. the first channel
	 * is pushed in the peaks left buffer, the second
	 * channel into the right one.
	 */
	virtual void analyzeStereoFrame(
		unsigned char* ptToAnalyze,
		unsigned int frameLength,
		MByteBuffer* ptPeaksLeft,
		MByteBuffer* ptPeaksRight );
};

#endif