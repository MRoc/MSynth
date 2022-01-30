#ifndef __WaveFileHandling
#define __WaveFileHandling

#include <framework/MException.h>
#include <framework/io/MFileOutputStream.h>
#include <framework/io/audio/MWaveFileHeader.h>

/**
 * this class encapsulates some algos for
 * streaming a wavefile to disk
 * and finally write the header.
 *
 *	1. openFile
 *	2. writeData
 *	3. setFormat
 *	4. writeFileHeader
 *	5. closeFile
 */
class WaveFileHandling
{
private:

	/**
	 * the header used for the wave file and format styles
	 */
	MWaveFileHeader		ivHeader;

	/**
	 * the output stream used to write to disk
	 */
	MFileOutputStream*	ivPtOut;

#ifdef _DEBUG
	/**
	 * flag used for debug purpose
	 */
	bool			ivHeaderWritten;
#endif

public:

	/**
	 * constructor
	 */
	WaveFileHandling();

	/**
	 * destructor
	 */
	virtual ~WaveFileHandling();

	/**
	 * opens a wave file for writing
	 */
	virtual void openFile( String fileName ); // throws MIoException

	/**
	 * writes data to the wave file
	 */
	virtual void writeData( unsigned char *data, unsigned int dataLength ); // throws MIoException

	/**
	 * sets the format used for writing
	 */
	virtual void setFormat( unsigned int samplingRate, unsigned short bitsPerSample, unsigned short channelCount );

	/**
	 * writes the file header and zero padding
	 */
	virtual void finalizeFile(); // throws MIoException

	/**
	 * closes the wave file
	 */
	virtual void closeFile(); // throws MIoException

	/**
	 * returns the amount of wave data written to disk in bytes
	 */
	virtual unsigned int getDataLength();
};

#endif