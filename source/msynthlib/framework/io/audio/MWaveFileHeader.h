#ifndef __MWaveFileHeader
#define __MWaveFileHeader

#include "../IOutputStream.h"
#include "../ISeekableInputStream.h"
#include "../../MObject.h"
#include "../../MException.h"

/**
 * this class encapsulates the header of a wave file
 */
class MWaveFileHeader :
	public MObject
{
public:

	/**
	 * runtime support
	 */
	static MRtti gvRtti;

protected:

	/**
	 * the datalength in bytes
	 */
	unsigned int ivDataLength;

	/**
	 * the channel cound of the file, maybe 1 or 2
	 */
	unsigned short ivChannelCount;

	/**
	 * the samples per second per one channel, e.g. 44100
	 */
	unsigned int ivSamplingRate;

	/**
	 * the bits per sample, e.g. 8 or 16
	 */
	unsigned short ivBitsPerSample;

public:

	/**
	 * constructor
	 */
	MWaveFileHeader();

	/**
	 * constructor
	 */
	MWaveFileHeader(
		unsigned int dataLength,
		unsigned short channelCount,
		unsigned int samplingRate,
		unsigned short bitsPerSample
		);

	/**
	 * destructor
	 */
	virtual ~MWaveFileHeader();

	/**
	 * returns the class name for runtime purpose
	 */
	virtual IRtti* getRtti() const;

	/**
	 * querries for a specified interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * returns a string representation
	 */
	virtual String toString() const;

	/**
	 * sets the datalength in bytes
	 */
	virtual void setDataLength( unsigned int dataLength );

	/**
	 * returns the datalength in bytes
	 */
	virtual unsigned int getDataLength();

	/**
	 * sets the number of channels, e.g. 1 or 2
	 */
	virtual void setChannelCount( unsigned short channelCount );

	/**
	 * returns the number of channels, e.g. 1 or 2
	 */
	virtual unsigned short getChannelCount();

	/**
	 * sets the samples per second per one channel, e.g. 44100
	 */
	virtual void setSamplingRate( unsigned int samplesPerSecond );

	/**
	 * returns the samples per second per one channel, e.g. 44100
	 */
	virtual unsigned int getSamplingRate();

	/**
	 * sets the bits per sample, e.g. 8 or 16
	 */
	virtual void setBitsPerSample( unsigned short bitsPerSample );

	/**
	 * returns the bits per sample, e.g. 8 or 16
	 */
	virtual unsigned short getBitsPerSample();

	/**
	 * writes the wave file header to the given outputstream
	 */
	virtual void write( IOutputStream* ptOut );

	/**
	 * reads the header from file and returns the offset of the data,
	 * if a error occured, a MException is thrown
	 */
	virtual unsigned int read( ISeekableInputStream* ptIn );

	/**
	 * creates an instance of a MWaveFileHeader
	 */
	static MObject* createInstance();
};

#endif