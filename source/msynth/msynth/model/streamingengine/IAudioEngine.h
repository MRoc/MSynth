/**
 *
 * IAudioEngine (C)2001 MRoc hifiShock
 *
 *	v. 1.0 - design of a abstract StreamingEngine
 *	v. 1.1 - introduced the IAudioStream + working
 *
 */

#ifndef __IAudioEngine
#define __IAudioEngine

#include <framework/MTypes.h>
#include <framework/MNativeTypes.h>
#include <mmsystem.h>
#include <dsound.h>

#include "../interfaces/IProcessor.h"

/**
 * defines a streaming audioengine. the audio engine requires a IAudioStream
 * that will be played. so it hides things like waiting for filling up the buffer.
 */
class IAudioEngine
{
public:

	/**
	 * creates a IAudioEngine
	 * throws MException
	 */
	virtual void create( IProcessor* ptStream ) = 0;

	/**
	 * destroys the IAudioEngine created with the create method
	 */
	virtual void destroy() = 0;

	/**
	 * starts playing the stream
	 */
	virtual bool start() = 0;

	/**
	 * stops playing the strean
	 */
	virtual bool stop() = 0;

	/**
	 * returns true if is playing
	 */
	virtual bool isPlaying() = 0;

	/**
	 * returns the samples per second played (samplingrate)
	 */
	virtual unsigned int getSamplingRate() = 0;

	/**
	 * returns the bits per sample, e.g. 8 or 16
	 */
	virtual unsigned short getBitsPerSample() = 0;

	/**
	 * returns a available channels
	 */
	virtual unsigned short getChannelCount() = 0;
};

#endif