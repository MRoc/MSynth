#ifndef __MSoundBufferUtils
#define __MSoundBufferUtils

#include "MSoundBuffer.h"
#include <math.h>
#include "../file/WaveFileHandling.h"

class MSoundBufferUtils
{
private:

	MSoundBufferUtils(){};
	~MSoundBufferUtils(){};

public:

	/**
	 * pans and volumes the given soundbuffer,
	 * uses MSoundBufferUtils::stereoMultiply.
	 *
	 * volume - the multiplicator
	 * pan - the panning between 0.0f and 1.0f
	 */
	static void steroVolume(
		MSoundBuffer* pBuffer,
		float volume,
		float pan );

	/**
	 * pans and volumes the given soundbuffer,
	 * uses MSoundBufferUtils::stereoMultiply.
	 *
	 * volume - the multiplicator
	 * pan - the panning between 0.0f and 1.0f
	 */
	static void steroVolume(
		MSoundBuffer* pBuffer,
		unsigned int startAt,
		unsigned int stopAt,
		float volume,
		float pan );

	/**
	 * does a stereo multiplication
	 *
	 * left - the left multiplicator
	 * right - the right multiplicator
	 */
	static void stereoMultiply(
		MSoundBuffer* pBuffer,
		float left,
		float right );

	/**
	 * does a stereo multiplication
	 *
	 * left - the left multiplicator
	 * right - the right multiplicator
	 */
	static void stereoMultiply(
		MSoundBuffer* pBuffer,
		unsigned int startAt,
		unsigned int stopAt,
		float left,
		float right );

	/**
	 * multiplies the given buffer with the given
	 * multiplicator from start to stop.
	 */
	static void multiply(
		MSoundBuffer* pBuffer,
		unsigned int start,
		unsigned int stop,
		float multiplicator );

	/**
	 * multiplies the given buffer with the given multiplicator
	 */
	static void multiply(
		MSoundBuffer* pBuffer,
		float multiplicator );

	/**
	 * adds pBuffer2 to pBuffer1 at the given
	 * index and the given amount of samples
	 */
	static void add(
		MSoundBuffer* pBuffer1,
		MSoundBuffer* pBuffer2,
		unsigned int insertPos,
		unsigned int length );

	/**
	 * adds buffer 2 to buffer 1
	 */
	static void add(
		MSoundBuffer* pBuffer1,
		MSoundBuffer* pBuffer2,
		FP mixValue );

	/**
	 * adds buffer 2 to buffer 1 with the mix and pan value
	 */
	static void addStereo(
		MSoundBuffer* pbuffer1,
		MSoundBuffer* pbuffer2,
		FP mixValue,
		FP panorama );

	/**
	 * pastes buffer 2 into buffer 1
	 */
	static void pasteBuffer(
		MSoundBuffer* pBuffer1,
		MSoundBuffer* pBuffer2,
		unsigned int pos,
		unsigned int len );

	/**
	 * fades a part of the sound buffer.
	 * returns the last value.
	 *
	 * because of iterative addition and hi roinding noize
	 * this method uses double precision.
	 */
	static double fade( MSoundBuffer* pBuffer, unsigned int start, unsigned int stop, double offset, double alpha );

	static float findMax( MSoundBuffer* ptBuffer );
	static float findMin( MSoundBuffer* ptBuffer );

	static void sinegen( MSoundBuffer* ptBuffer, float frequency, float phase, float dc, float amp );
	static void sinegen( MSoundBuffer* ptBuffer, float frequency, float phase, float dc, float amp, unsigned int samplingRate );

	static void saveBufferAsWave( String filename, MSoundBuffer* ptBuffer, float amp );
};

#endif