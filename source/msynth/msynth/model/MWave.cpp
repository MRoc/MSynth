/*

	MWave (C)2000-2003 MRoc hifiShock

	TODO: - compare saved wave with cool edit output

*/

#include "MWave.h"
#include <framework/io/MIoException.h>

/**
 * runtime support
 */
MRtti MWave::gvRtti = MRtti( "MWave", MWave::createInstance );

/**
 * the attribute name for samplingrate (xml)
 */
const String MWave::ATTR_SAMPLINGRATE = "samplingrate";

/**
 * the attribute name for bits per sample (xml)
 */
const String MWave::ATTR_BITS_PER_SAMPLE = "bitspersample";

/**
 * the attribute name for channelcount (xml)
 */
const String MWave::ATTR_CHANNELCOUNT = "channelcount";

/**
 * the attribute name for data (xml)
 */
const String MWave::ATTR_DATA = "data";

/**
 * constructor
 */
MWave::MWave() :
	ivPtData( 0 )
{
	ivHeader.setSamplingRate( 44100 );
	ivHeader.setBitsPerSample( 16 );
	ivHeader.setChannelCount( 1 );
}

/**
 * constructor loading the wave from a wave file
 */
MWave::MWave( const char* fileName ) : // throws MException
	ivPtData( 0 )
{
	loadFile( fileName );
}

/**
 * destructor
 */
MWave::~MWave()
{
	clear();
}

/**
 * clears the data allocated by this class
 */
void MWave::clear()
{
	if( ivPtData )
	{
		delete ivPtData;
		ivPtData = 0;
	}
	ivHeader.setDataLength( 0 );
}

/**
 * returns the datalength of this wave in bytes
 */
unsigned int MWave::getDataLength()
{
	return ivHeader.getDataLength();
}

/**
 * returns the samplingrate of this wave
 */
unsigned int MWave::getSamplingRate()
{
	return ivHeader.getSamplingRate();
}

/**
 * returns the number of bits per sample
 */
unsigned short MWave::getBitsPerSample()
{
	return ivHeader.getBitsPerSample();
}

/**
 * returns the number of channels of this wave
 */
unsigned short MWave::getChannelCount()
{
	return ivHeader.getChannelCount();
}

/**
 * returns the data of this wave
 */
unsigned char* MWave::getData()
{
	return ivPtData;
}

/**
 * sets the data of this wave
 */
void MWave::setData( unsigned char* data, unsigned int length )
{
	if( ivPtData )
		delete ivPtData;
	ivPtData = data;
	ivHeader.setDataLength( length );
}

/**
 * loads the wave from a wave file
 */
void MWave::loadFile( const char* fileName ) // throws MException
{
	MFileInputStream in;
	try
	{
		in.open( fileName );
		loadFile( &in );
	}
	catch( MIoException ae )
	{
		in.close();
		throw ae;
	}
	catch( MException ae )
	{
		in.close();
		throw ae;
	}
	in.close();
}

/**
 * loads the wave file from input stream
 */
void MWave::loadFile( ISeekableInputStream* pIn )
{
	clear();
	int dataOffset = ivHeader.read( pIn );
	TRACE( "%s\n", ivHeader.toString() );

	unsigned char* data = new unsigned char[ ivHeader.getDataLength() ];
	if(!data)
		throw MException("Error allocating memory for wave!");
	pIn->read( (unsigned char*)data, ivHeader.getDataLength() );
	ivPtData = data;
}

/** 
 * flats the file to a mono file if stereo
 */
void MWave::makeMono()
{
	ASSERT( ivHeader.getChannelCount() == 2 );

	TRACE( "<mwave::makemono/>\n" );

	if( ivHeader.getBitsPerSample() == 8 )
	{
		unsigned char* ptNewData = new unsigned char[ getDataLength() / 2 ];
		if( ! ptNewData )
			throw MException( "File too big. Could not allocate enough memory to make mono!" );
		unsigned char* ptStop = ivPtData + getDataLength();
		for(
			unsigned char* ptSrc=ivPtData, *ptDest=ptNewData;
			ptSrc<ptStop;
			ptSrc+=2,ptDest++ )
		{
			(*ptDest) = (*ptSrc);
		}
		delete ivPtData;
		ivPtData = ptNewData;
		ivHeader.setDataLength( ivHeader.getDataLength() / 2 );
		ivHeader.setChannelCount( 1 );
	}
	else if( ivHeader.getBitsPerSample() == 16 )
	{
		signed short* ptNewData = new signed short[ getDataLength() / 4 ];
		if( ! ptNewData )
			throw MException( "File too big. Could not allocate enough memory to make mono!" );
		signed short* ptSrc = (signed short*) ivPtData;
		signed short* ptStop = (signed short*)(ivPtData + getDataLength());
		for(
			signed short *ptDest=ptNewData;
			ptSrc<ptStop;
			ptSrc+=2,ptDest++ )
		{
			(*ptDest) = (*ptSrc);
		}
		delete this->ivPtData;
		ivPtData = (unsigned char*) ptNewData;
		ivHeader.setDataLength( ivHeader.getDataLength() / 2 );
		ivHeader.setChannelCount( 1 );
	}

	/*unsigned char* ptSrc = this->ivPtData;
	unsigned char* ptSrcEnd = ptSrc + this->ivHeader.getDataLength();*/
}

/**
 * converts the sample if the quantisation is not 16 bit
 */
void MWave::make16bit()
{
	switch( this->ivHeader.getBitsPerSample() )
	{
	case 8:
		{
			// convert from 8 bit to 16 bit

			unsigned int dLen1 = getDataLength();
			unsigned int dLen2 = dLen1 * 2;
			unsigned char* pData = new unsigned char[ dLen2 ];

			unsigned char* pt1 = (unsigned char*) ivPtData;
			unsigned char* pt1End = pt1 + dLen1;

			signed short* pt2 = (signed short*) pData;
			signed short* pt2End = pt2 + dLen1;

			for( ;pt1<pt1End; pt1++, pt2++ )
				(*pt2) = (((signed short)(*pt1))-128)*255;

			ivHeader.setBitsPerSample( 16 );
			ivHeader.setDataLength( dLen2 );
			delete ivPtData;
			ivPtData = pData;
		}
		break;
	case 16:
		break;
	default:
		break;
	}
}

/**
 * saves the wave to a wavefile
 */
void MWave::saveFile( const char* fileName )
{
	MFileOutputStream out;

	out.open( fileName );

	try
	{
		ivHeader.write( &out );
		out.write( (const unsigned char*) ivPtData, ivHeader.getDataLength() );
		out.write( (unsigned int) 0 ); // 4 bytes zero padding
		TRACE( "%s\n", ivHeader.toString() );
	}
	catch( MException ae )
	{
		out.close();
		throw ae;
	}
	out.close();
}

/**
 * stores as tree node
 */
MTreeNode* MWave::save()
{
	MTreeNode* ptBack = new MTreeNode( ELEM_OBJECT );
	ptBack->setAttribute( ATTR_CLASS, getRtti()->getClassName() );
	ptBack->setAttribute( ATTR_SAMPLINGRATE, MInt::toString( (int)ivHeader.getSamplingRate() ) );
	ptBack->setAttribute( ATTR_BITS_PER_SAMPLE, MInt::toString( (int)ivHeader.getBitsPerSample() ) );
	ptBack->setAttribute( ATTR_CHANNELCOUNT, MInt::toString( (int)ivHeader.getChannelCount() ) );

	if( getData() && ivHeader.getDataLength() > 0 )
	{
		unsigned int outLenTmp = 0;
		String base64sample = MXmlUtil::getBase64String( getData(), ivHeader.getDataLength() );
		ptBack->setAttribute( ATTR_DATA, base64sample );
	}

	return ptBack;
}

/**
 * loads from the given tree node
 */
void MWave::load( MTreeNode* ptNode )
{
	ivHeader.setSamplingRate(
		MInt::parse(
			ptNode->getAttribute( ATTR_SAMPLINGRATE ),
			44100 ) );
	ivHeader.setChannelCount(
		MInt::parse(
			ptNode->getAttribute( ATTR_CHANNELCOUNT ),
			1 ) );
	ivHeader.setBitsPerSample(
		MInt::parse(
			ptNode->getAttribute( ATTR_BITS_PER_SAMPLE ),
			16 ) );
	String data = ptNode->getAttribute( ATTR_DATA );
	if( data != "" )
	{
		unsigned int dataLen = 0;
		unsigned char* ptData = MXmlUtil::getBase64Binary( data, &dataLen );
		setData( ptData, dataLen );
	}
}

/**
 * returns a string representation of this wave
 */
String MWave::toString()
{
	String buffer("");
	buffer.Format(
		"MWave length: %i samplingrate: %i bits: %i channels: %i",
		this->ivHeader.getDataLength(),
		this->ivHeader.getSamplingRate(),
		this->ivHeader.getBitsPerSample(),
		this->ivHeader.getChannelCount() );
	return buffer;
}

/**
 * returns the runtime type information
 */
IRtti* MWave::getRtti() const
{
	return &gvRtti;
}

/**
 * for dynamic casting
 */
void* MWave::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MWave*)this);
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else
		return MObject::getInterface( className );
}

/**
 * creates an instance of this class
 */
MObject* MWave::createInstance()
{
	return new MWave();
}