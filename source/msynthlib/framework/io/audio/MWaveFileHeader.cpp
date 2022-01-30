#include "MWaveFileHeader.h"

/*
	unsigned int ivDataLength; // in bytes
	unsigned short ivChannelCount; // 1 or 2
	unsigned int ivSamplingRate; // e.g. 44100
	unsigned short ivBitsPerSample; // 8 or 16
*/

/**
 * runtime support
 */
MRtti MWaveFileHeader::gvRtti = MRtti( "MWaveFileHeader", MWaveFileHeader::createInstance );

/**
 * constructor
 */
MWaveFileHeader::MWaveFileHeader() :
	MObject(),
	ivDataLength( 0 ),
	ivChannelCount( 0 ),
	ivSamplingRate( 0 ),
	ivBitsPerSample( 0 )
{
}

/**
 * constructor
 */
MWaveFileHeader::MWaveFileHeader(
	unsigned int dataLength,
	unsigned short channelCount,
	unsigned int samplingrate,
	unsigned short bitsPerSample
	) :
	MObject(),
	ivDataLength( dataLength ),
	ivChannelCount( channelCount ),
	ivSamplingRate( samplingrate ),
	ivBitsPerSample( bitsPerSample )
{
}

/**
 * destructor
 */
MWaveFileHeader::~MWaveFileHeader()
{
}

/**
 * querries for a specified interface
 */
void* MWaveFileHeader::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MWaveFileHeader*)this);
	else
		return MObject::getInterface( className );
}

/**
 * returns the class name for runtime purpose
 */
IRtti* MWaveFileHeader::getRtti() const
{
	return &gvRtti;
}

/**
 * returns a string representation
 */
String MWaveFileHeader::toString() const
{
	String back("");
	back.Format(
		"MWaveFileHeader\n\tdatalength....: %i\n\tchannelcount..: %i\n\tsamplingrate..: %i\n\tbitsPerSample.: %i",
		(int) this->ivDataLength,
		(int) this->ivChannelCount,
		(int) this->ivSamplingRate,
		(int) this->ivBitsPerSample );
	return back;
}

/**
 * sets the datalength in bytes
 */
void MWaveFileHeader::setDataLength( unsigned int dataLength )
{
	ivDataLength = dataLength;
}

/**
 * returns the datalength in bytes
 */
unsigned int MWaveFileHeader::getDataLength()
{
	return ivDataLength;
}

/**
 * sets the number of channels, e.g. 1 or 2
 */
void MWaveFileHeader::setChannelCount( unsigned short channelCount )
{
	ASSERT( channelCount == 1 || channelCount == 2 );
	ivChannelCount = channelCount;	
}

/**
 * returns the number of channels, e.g. 1 or 2
 */
unsigned short MWaveFileHeader::getChannelCount()
{
	return ivChannelCount;
}

/**
 * sets the samples per second, e.g. 44100
 */
void MWaveFileHeader::setSamplingRate( unsigned int samplingRate )
{
	ASSERT( samplingRate == 44100 );
	ivSamplingRate = samplingRate;
}

/**
 * returns the samples per second, e.g. 44100
 */
unsigned int MWaveFileHeader::getSamplingRate()
{
	return ivSamplingRate;
}

/**
 * sets the bits per sample, e.g. 8 or 16
 */
void MWaveFileHeader::setBitsPerSample( unsigned short bitsPerSample )
{
	ASSERT( bitsPerSample == 8 || bitsPerSample == 16 );
	ivBitsPerSample = bitsPerSample;
}

/**
 * returns the bits per sample, e.g. 8 or 16
 */
unsigned short MWaveFileHeader::getBitsPerSample()
{
	return ivBitsPerSample;
}

/**
 * writes the wave file header to the given outputstream
 */
void MWaveFileHeader::write( IOutputStream* ptOut )
{
	unsigned short blockAlign = (ivChannelCount * ivBitsPerSample) / 8; // = bytes per sample(for all channels)

	ptOut->write( "RIFF", 4 );										// 4 bytes = RIFF
	ptOut->write( ivDataLength + 36 );								// 4 bytes = (fileLen - 8)
	ptOut->write( "WAVEfmt ", 8 );									// 8 bytes = WAVEfmt
	ptOut->write( (unsigned int) 0x00000010 );						// 4 bytes = chunk length = 16
	ptOut->write( (unsigned short) 0x0001 );						// 2 bytes = PCM type = 1
	ptOut->write( ivChannelCount );									// 2 bytes = channel count
	ptOut->write( ivSamplingRate );									// 4 bytes = samplingrate = e.g. 44100
	ptOut->write( (unsigned int)(ivSamplingRate * blockAlign) );	// 4 bytes = bytes/second = samplingRate * blockAlign
	ptOut->write( blockAlign );										// 2 bytes = blockAlign = channels * bitsPerSample / 8 = bytes per sample(for all channels)
	ptOut->write( ivBitsPerSample );								// 2 bytes = bitsPerSample = e.g. 8 or 16
	ptOut->write( "data", 4 );										// 4 bytes = data
	ptOut->write( ivDataLength );									// 4 bytes = dataLength
																	// +-------------------------------------------
																	// 44 bytes
}

/**
 * reads the header from file and returns the offset of the data,
 * if a error occured, a MException is thrown
 */
unsigned int MWaveFileHeader::read( ISeekableInputStream* ptIn )
{
	char			riff[4];			// = 'RIFF'
	unsigned int	fileLenMin8;		// = file length - 8
	char			wave[4];			// = 'WAVE'
	char			fmt[4];				// = 'fmt '
	unsigned int	chunkLength;		// = 16
	unsigned short	formatType;			// = 1 = PCM

	unsigned int	bytesPerSecond;		// = ivSamplesPerSecond * blockAlign;
	unsigned short	blockAlign;			// = blockAlign = channels * bitsPerSample / 8 = bytes per sample(for all channels)
	char			data[4];			// = 'data'

	unsigned int	dataOffset = 0;

	ptIn->read( (signed char*)riff, 4 );
	if( riff[0] != 'R' ||
		riff[1] != 'I' ||
		riff[2] != 'F' ||
		riff[3] != 'F' )
	{
		throw MException( "Unknown file format! 'RIFF' chunk not found" );
	}

	ptIn->read( fileLenMin8 );

	ptIn->read( (signed char*) wave, 4 );
	if( wave[0] != 'W' ||
		wave[1] != 'A' ||
		wave[2] != 'V' ||
		wave[3] != 'E'  )
	{
		throw MException( "Unknown file format! 'WAVE' chunk not found" );
	}

	ptIn->read( (signed char*) fmt, 4 );
	if( fmt[0] != 'f' ||
		fmt[1] != 'm' ||
		fmt[2] != 't' ||
		fmt[3] != ' '  )
	{
		throw MException( "Unknown file format! 'fmt' chunk not found" );
	}

	ptIn->read( chunkLength );

	ptIn->read( formatType );
	if( formatType != 1 )
	{
		throw MException( "Unknown format type found!" );
	}

	ptIn->read( ivChannelCount );
	if( ivChannelCount != 1 &&
		ivChannelCount != 2 )
	{
		throw MException( "Invalid channel count found. Only 1 and 2 channels allowed!" );
	}

	ptIn->read( ivSamplingRate );
	ptIn->read( bytesPerSecond );
	ptIn->read( blockAlign );
	ptIn->read( ivBitsPerSample );

	// revision: ignore because MSynth wrote invlid headers somtimes
	/*if( blockAlign != ivChannelCount * ivBitsPerSample / 8  )
	{
		throw MException( "Wave header validation failed! blockAlign != channelCount * bitsPerSample / 8" );
	}*/

	ptIn->read( (signed char*) data, 4 );
	if( data[0] != 'd' ||
		data[1] != 'a' ||
		data[2] != 't' ||
		data[3] != 'a'  )
	{
		// if no data chunk was found,
		// perhaps overread 14 bytes wave2.0? has introduced

		ptIn->seek( 36 );
		short testDummyBegin;
		ptIn->read( testDummyBegin );
		if( testDummyBegin != 0 )
			throw MException( "The file is not a old datachunked or new one (unkwown)" );

		char fact[4];
		ptIn->read( (signed char*) fact, 4 );
		if( fact[0] != 'f' ||
			fact[1] != 'a' ||
			fact[2] != 'c' ||
			fact[3] != 't'  )
		{
			throw MException( "The file is not a new chunked one (unkwown)" );
		}

		char dummy[8];
		ptIn->read( (signed char*) dummy, 8 );

		ptIn->read( (signed char*) data, 4 );
		if( data[0] != 'd' ||
			data[1] != 'a' ||
			data[2] != 't' ||
			data[3] != 'a'  )
		{
			throw MException( "File has no datachunk!" );
		}
		dataOffset = 56;
	}
	else
		dataOffset = 44;

	ptIn->read( ivDataLength );

	return dataOffset;
}

/**
 * creates an instance of a MWaveFileHeader
 */
MObject* MWaveFileHeader::createInstance()
{
	return new MWaveFileHeader();
}

