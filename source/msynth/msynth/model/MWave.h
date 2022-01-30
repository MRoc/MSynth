/**
 *
 *	MWave (C)2000-2003 MRoc hifiShock
 *
 */

#ifndef __MWave
#define __MWave

#include <framework/MObject.h>
#include <framework/primitive/MInt.h>
#include <framework/MException.h>
#include <framework/treedocument/ISerializeable.h>
#include <framework/io/MFileOutputStream.h>
#include <framework/io/MFileInputStream.h>
#include <framework/io/audio/MWaveFileHeader.h>
#include <framework/xml/MXmlUtil.h>

/**
 * MWave is the implementation of a audio wave file
 */
class MWave :
	public MObject,
	public ISerializeable
{
public:

	/**
	 * runtime support
	 */
	static MRtti gvRtti;

protected:

	/**
	 * the attribute name for samplingrate (xml)
	 */
	static const String ATTR_SAMPLINGRATE;

	/**
	 * the attribute name for bitsPerSample (xml)
	 */
	static const String ATTR_BITS_PER_SAMPLE;

	/**
	 * the attribute name for channelcount (xml)
	 */
	static const String ATTR_CHANNELCOUNT;

	/**
	 * the attribute name for data (xml)
	 */
	static const String ATTR_DATA;

	/**
	 * the sample data
	 */
	unsigned char* ivPtData;

	/**
	 * stores all information about the wave
	 */
	MWaveFileHeader ivHeader;

public:

	/**
	 * constructor
	 */
	MWave();

	/**
	 * constructor loading a wavefile
	 */
	MWave( const char* fileName ); // throws MException

	/**
	 * destructor
	 */
	virtual ~MWave();

	/**
	 * clears the data allocated by this class
	 */
	virtual void clear();

	/**
	 * returns the datalength of this wave in bytes
	 */
	virtual unsigned int getDataLength();

	/**
	 * returns the samplingrate of this wave
	 */
	virtual unsigned int getSamplingRate();

	/**
	 * returns the number of bits per sample
	 */
	virtual unsigned short getBitsPerSample();

	/**
	 * returns the number of channels of this wave
	 */
	virtual unsigned short getChannelCount();

	/**
	 * returns the data of this wave
	 */
	virtual unsigned char* getData();

	/**
	 * sets the data of this wave
	 */
	virtual void setData( unsigned char* data, unsigned int dataLength );

	/**
	 * loads the wave from a wave file
	 */
	virtual void loadFile( const char* fileName );// throws MException, MIoException

	/**
	 * loads the wave file from input stream
	 */
	virtual void loadFile( ISeekableInputStream* pIn );// throws MException, MIoException

	/**
	 * saves the wave to a wavefile
	 */
	virtual void saveFile( const char* fileName ); // throws MIoException

	/** 
	 * flats the file to a mono file if stereo
	 */
	virtual void makeMono();

	/**
	 * converts the sample if the quantisation is not 16 bit
	 */
	virtual void make16bit();

	/**
	 * loads from the given tree node
	 */
	virtual void load( MTreeNode* ptNode );

	/**
	 * stores as tree node
	 */
	virtual MTreeNode* save();

	/**
	 * returns a string representation of this wave
	 */
	virtual String toString();

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * for dynamic casting
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * creates an instance of this class
	 */
	static MObject* createInstance();
};

#endif