/*

  (C)2000 MRoc hifiShock

	v 0.1	-> Design
	v 0.2	-> Bugfix zeroEnd and wholeDataLength
	v 0.3	-> MW_SAMPLETYPE = signed short
	v 0.4	-> Throws MException
	v 0.5	-> rewrote with MFileOutputStream and MWaveFileHeader

*/

#include "WaveFileHandling.h"
#include <framework/io/MIoException.h>

/**
 * constructor
 */
WaveFileHandling::WaveFileHandling() :
	ivPtOut( 0 )
{
}

/**
 * destructor
 */
WaveFileHandling::~WaveFileHandling()
{
	ASSERT( ivPtOut == 0 );
}

/**
 * sets the format used for writing
 */
void WaveFileHandling::setFormat(
	unsigned int samplingRate,
	unsigned short bitsPerSample,
	unsigned short channelCount )
{
	ivHeader.setSamplingRate( samplingRate );
	ivHeader.setBitsPerSample( bitsPerSample );
	ivHeader.setChannelCount( channelCount );
}

/**
 * opens a wave file for writing
 */
void WaveFileHandling::openFile( String fileName )
{
#ifdef _DEBUG
	ivHeaderWritten = false;
#endif

	ASSERT( ivPtOut == 0 );
	
	this->ivPtOut = new MFileOutputStream();

	try
	{
		ivPtOut->open( fileName );
	}
	catch( MIoException ae )
	{
		SAFE_DELETE( ivPtOut );
		throw ae;
	}

	ivPtOut->write( "                                        ", 44 );
}

/**
 * writes data to the wave file
 */
void WaveFileHandling::writeData( unsigned char* ptData, unsigned int dataLength )
{
	ASSERT( ivPtOut );

	ivPtOut->write( ptData, dataLength );
	ivHeader.setDataLength( ivHeader.getDataLength() + dataLength );
}

/**
 * writes the file header
 */
void WaveFileHandling::finalizeFile()
{
#ifdef _DEBUG
	ASSERT( ! ivHeaderWritten );
	ivHeaderWritten = true;
#endif

	ASSERT( ivPtOut );

	ivPtOut->seek( 0 );
	ivHeader.write( ivPtOut );
	ivPtOut->seek( 44 + ivHeader.getDataLength() );
	ivPtOut->write( (int) 0x0 );
}

/**
 * closes the wave file
 */
void WaveFileHandling::closeFile()
{
	ASSERT( ivPtOut );
	try
	{
		ivPtOut->close();
	}
	catch( MIoException ae )
	{
		SAFE_DELETE( ivPtOut );
		throw ae;
	}
	SAFE_DELETE( ivPtOut );
}

/**
 * returns the amount of wave data written to disk in bytes
 */
unsigned int WaveFileHandling::getDataLength()
{
	return ivHeader.getDataLength();
}