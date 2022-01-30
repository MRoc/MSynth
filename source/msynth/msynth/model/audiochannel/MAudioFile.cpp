#include "maudiofile.h"

#define DEFAULT_BUFFER_LEN 8192

/**
 * runtime support
 */
MRtti MAudioFile::gvRtti = MRtti( "MAudioFile", MAudioFile::createInstance );

/**
 * the xml attribute name storing the filename
 */
const String MAudioFile::ATTR_FILENAME = "filename";

/**
 * the xml attribute name storing the id
 */
const String MAudioFile::ATTR_ID = "id";

/**
 * constructor
 */
MAudioFile::MAudioFile() :
	ivPtFile( 0 ),
	ivHeaderLength( 0 ),
	ivPtBuffer( 0 ),
	ivPlayPosition( 0 ),
	ivPtPeakFile( 0 ),
	ivLoadedId( -1 )
{
	ivPtBuffer = new MStaticByteBuffer( 2*DEFAULT_BUFFER_LEN );
	ivPtPeakFile = new MPeakFile();
}	

/**
 * destructor
 */
MAudioFile::~MAudioFile()
{
	SAFE_DELETE( ivPtPeakFile );
	if( ivPtFile )
		closeAudioFile();
	SAFE_DELETE( ivPtBuffer );
}

/**
 * opens the audio file stream
 */
void MAudioFile::openAudioFile( String fileName )
{
	ASSERT( ! ivPtFile );

	ivFileName = fileName;
	TRACE( "setting filename to: \"%s\"\n", ivFileName );

	TRACE( "filename: %s\n", fileName );

	ivPtFile = new MFileInputStream();

	try
	{
		ivPtFile->open( fileName );
	}
	catch( MException ae )
	{
		delete ivPtFile;
		ivPtFile = 0;
		return;
	}

	this->ivHeaderLength = ivHeader.read( ivPtFile );


	//Channel-Check
	if( ivHeader.getChannelCount() != 1 && ivHeader.getChannelCount() != 2 )
	{
		throw MException(
			"The loaded file has more than two channels,\nonly mono and stereo wavefiles can be loaded!");
	}	

	//Bit-Check
	if( ivHeader.getBitsPerSample() != 16 )
	{
		throw MException( "The loaded file was not saved with 16 bit.\nOnly 16-bit files supported!");
	}

	this->analyzePeaks( this );
}

/**
 * closes the audio file stream
 */
void MAudioFile::closeAudioFile()
{
	if( ivPtFile )
	{
		ivPtFile->close();
		SAFE_DELETE( ivPtFile );
	}
}

/**
 * querries for a specified interface
 */
void* MAudioFile::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MAudioFile*)this);
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else if( className == "IProcessor" )
		return (void*) ((IProcessor*)this);
	else if( className == "IProgress" )
		return (void*) ((IProgress*)this);
	else
		return MObject::getInterface( className );
}

/**
 * returns a string representation
 */
String MAudioFile::toString()
{
	return ivFileName;
}

/**
 * returns the runtime type information
 */
IRtti* MAudioFile::getRtti() const
{
	return &gvRtti;
}

/**
 * stores as tree node
 */
MTreeNode* MAudioFile::save()
{
	MTreeNode* ptBack = new MTreeNode( ELEM_OBJECT );
	ptBack->setAttribute( ATTR_CLASS, getRtti()->getClassName() );
	ptBack->setAttribute( ATTR_FILENAME, ivFileName );
	ptBack->setAttribute( ATTR_ID, MInt::toString( getSerId() ) );
	return ptBack;
}

/**
 * loads from the given tree node
 */
void MAudioFile::load( MTreeNode* ptNode )
{
	ASSERT( ptNode->getName() == ELEM_OBJECT );
	ASSERT( ptNode->getAttribute( ATTR_CLASS ) == getRtti()->getClassName() );

	ivFileName = ptNode->getAttribute( ATTR_FILENAME );
	ivLoadedId = MInt::parse( ptNode->getAttribute( ATTR_ID ), -1 );

	MIdMap::getInstance()->store( ivLoadedId, this );

	openAudioFile( this->ivFileName );
}

/**
 * loads next data from file to buffer
 */
unsigned int MAudioFile::loadBuffer()
{
	ASSERT( ivPtFile );
	ASSERT( ivPtBuffer );
	ASSERT( ivPtBuffer->checksum() );

	unsigned int fptPos = ivPtFile->getPosition();
	unsigned int fLen = ivPtFile->getFileLength();
	unsigned int bytesLeftInFile = ((fLen-ivHeaderLength) - fptPos);
	unsigned int bytesToLoad = ivPtBuffer->getLength();

	if( bytesToLoad > bytesLeftInFile )
		bytesToLoad = (unsigned int) bytesLeftInFile;

	if( bytesToLoad )
	{
		ivPtFile->read( (unsigned char*)ivPtBuffer->getData(), bytesToLoad );
		ivPtBuffer->setFill( bytesToLoad );
	}
	ASSERT( ivPtBuffer->checksum() );

	return bytesToLoad;
}

/**
 * clears the buffer
 */
void MAudioFile::zeroBuffer()
{
	memset( (void*)ivPtBuffer->getData(), 0, ivPtBuffer->getLength() );
	ivPtBuffer->setFill( ivPtBuffer->getLength() );
	ASSERT( ivPtBuffer->checksum() );
}

/**
 * fire a event into the processor
 */
void MAudioFile::processEvent( MEvent* ptEvent )
{
	if( ptEvent->getType() == MEvent::RESET )
		reset();
}

/**
 * the IProcessor method streaming the audio file to a soundbuffer
 */
void MAudioFile::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt)
{
	if( ivPtFile )
	{
		ASSERT( ivPtFile );
		ASSERT( buffer );
		ASSERT( ivPtBuffer->checksum() );

		unsigned int samplesToRender = stopAt - startFrom;
		unsigned int samplesRendered = 0;
		while( samplesRendered < samplesToRender )
		{
			unsigned int samplesLeft = samplesToRender - samplesRendered;

			// is playposition at begin of buffer ???
			if( ivPlayPosition ==  0  ) // -> YES, load new data from file
				if( ! loadBuffer() )
					zeroBuffer();

			ASSERT( this->ivPtBuffer->getFill() );
			ASSERT( ivPlayPosition < ivPtBuffer->getFill() );

			unsigned int bytesLeftInBuffer = ivPtBuffer->getFill() - ivPlayPosition;
			unsigned int samplesLeftInBuffer = bytesLeftInBuffer / ( ivHeader.getChannelCount() * sizeof( MW_SAMPLETYPE ) );
			ASSERT( samplesLeftInBuffer );
			if( ivHeader.getChannelCount() == 1 )
			{
				unsigned int renderingNow = samplesLeft;
				if( renderingNow > samplesLeftInBuffer )
					renderingNow = samplesLeftInBuffer;

				MW_SAMPLETYPE* ptScr = (MW_SAMPLETYPE*)( ivPtBuffer->getData() + ivPlayPosition );
				MW_SAMPLETYPE* ptEnd = ptScr + renderingNow;
				FP* ptTarget1 = buffer->getData(0) + startFrom;
				FP* ptTarget2 = buffer->getData(1) + startFrom;
				for(
					MW_SAMPLETYPE* pt=ptScr;
					pt<ptEnd;
					pt++,ptTarget1++,ptTarget2++)
				{
					FP v = (float)(*pt);
					v /= MW_MAX;
					(*ptTarget1) = v;
					(*ptTarget2) = v;
				}
				samplesRendered += renderingNow;
				ivPlayPosition += (renderingNow * ivHeader.getChannelCount() * sizeof( MW_SAMPLETYPE ) );
			}
			else if( ivHeader.getChannelCount() == 2 )
			{
				unsigned int renderingNow = samplesLeft;
				if( renderingNow > samplesLeftInBuffer )
					renderingNow = samplesLeftInBuffer;
				ASSERT( renderingNow );

				MW_SAMPLETYPE* ptScr = ((MW_SAMPLETYPE*)( ivPtBuffer->getData() + ivPlayPosition ));
				MW_SAMPLETYPE* ptEnd = ptScr + (renderingNow*ivHeader.getChannelCount());

				FP* ptTarget1 = buffer->getData(0) + startFrom + samplesRendered;
				FP* ptTarget2 = buffer->getData(1) + startFrom + samplesRendered;

				for(
					MW_SAMPLETYPE* pt=ptScr;
					pt<ptEnd;
					ptTarget1++,ptTarget2++ )
				{
					(*ptTarget1) = float(*pt) / MW_MAX;
					pt++;
					(*ptTarget2) = float(*pt) / MW_MAX;
					pt++;
				}

				samplesRendered += renderingNow;
				ivPlayPosition += ( renderingNow * ivHeader.getChannelCount() * sizeof( MW_SAMPLETYPE ) );
			}
			else
				ASSERT( false );

			if( ivPlayPosition >= ivPtBuffer->getFill() )
				ivPlayPosition = 0;
		}
	}
}

/**
 * resets the playstate
 */
void MAudioFile::reset()
{
	if( ivPtFile )
	{
		ivPlayPosition = 0;
		ivPtFile->seek( this->ivHeaderLength );
	}
}

/**
 * reads bytes from file to the buffer
 */
unsigned int MAudioFile::read( unsigned char* ptBuffer, unsigned int length )
{
	unsigned int bytesLeftInFile = ((ivPtFile->getFileLength() - ivHeaderLength) - ivPtFile->getPosition());
	if( length > bytesLeftInFile )
		length = (unsigned int) bytesLeftInFile;
	ivPtFile->read( (unsigned char*) ptBuffer, length );
	ivPlayPosition += length;

	if( ivPlayPosition >= ivPtBuffer->getLength()-1 )
		ivPlayPosition = 0;

	return length;
}

/**
 * returns the channel count
 */
unsigned int MAudioFile::getChannelCount()
{
	return this->ivHeader.getChannelCount();
}

/**
 * generates the peakfile data
 */
void MAudioFile::analyzePeaks( IProgress* ptProgress )
{
	unsigned int i;
	reset();

	unsigned int channelCount = getChannelCount();
	MStaticByteBuffer* ptReadBuffer = new MStaticByteBuffer( 1000*channelCount );

	MByteBuffer** ivPtPeakData = new MByteBuffer*[ channelCount ];
	for( i=0;i<channelCount;i++ )
		ivPtPeakData[ i ] = new MByteBuffer( 100, 100 );

	unsigned int len = 0;
	unsigned int iterations = 0;
	while( (len = read( ptReadBuffer->getData(), ptReadBuffer->getLength() ) ) )
	{
		if( ptProgress )
			ptProgress->onProgress( 
				( (float)ptReadBuffer->getLength() / (float)(unsigned int)getSampleDataLength()) * (iterations++) );
		if( channelCount == 1 )
			ivPtPeakFile->analyzeMonoFrame(
				ptReadBuffer->getData(),
				len,
				ivPtPeakData[ 0 ] );
		else if( channelCount == 2 )
			ivPtPeakFile->analyzeStereoFrame(
				ptReadBuffer->getData(),
				len,
				ivPtPeakData[ 0 ],
				ivPtPeakData[ 1 ] );
		else
			ASSERT( false );
	}
	for( i=0;i<channelCount;i++ )
		ivPtPeakData[ i ]->trim();

	this->ivPtPeakFile->setData( ivPtPeakData, channelCount );

	delete ptReadBuffer;

	reset();
}

/**
 * returns the sample data length in file in bytes
 */
__int64 MAudioFile::getSampleDataLength()
{
	return ivPtFile->getFileLength() - ivHeaderLength;
}

void MAudioFile::onProgress( float value )
{
	TRACE( "progress: %i percent\n", (int) (value*100) );
}

void MAudioFile::onFinished()
{
	TRACE( "void MAudioFile::onFinished()\n" );
}

/**
 * returns the peak file
 */
MPeakFile* MAudioFile::getPeakFile()
{
	return this->ivPtPeakFile;
}

/**
 * returns the serialized id (casted this)
 */
int MAudioFile::getSerId()
{
	return (int) this;
}

/**
 * returns the loaded id
 */
int MAudioFile::getLoadedId()
{
	return this->ivLoadedId;
}

/**
 * returns a instance of this class
 */
MObject* MAudioFile::createInstance()
{
	return new MAudioFile();
}