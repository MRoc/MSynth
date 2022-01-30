#ifndef __MAudioFile
#define __MAudioFile

#include <framework/MTypes.h>
#include <framework/MObject.h>
#include <framework/MException.h>
#include <framework/map/MIdMap.h>
#include <framework/treedocument/ISerializeable.h>
#include <framework/nio/MStaticBytebuffer.h>
#include <framework/io/MFileInputStream.h>
#include <framework/io/audio/MWaveFileHeader.h>
#include <framework/thread/IProgress.h>

#include "MPeakFile.h"
#include "../interfaces/IProcessor.h"

/**
 * implementation of a streamable audio file
 * that can be played from disk.
 */
class MAudioFile :
	public MObject,
	public ISerializeable,
	public IProcessor, 
	public IProgress
{
public:

	/**
	 * runtime support
	 */
	static MRtti gvRtti;

protected:

	/**
	 * the xml attribute name storing the id
	 */
	static const String ATTR_ID;

	/**
	 * the xml attribute name storing the filename
	 */
	static const String ATTR_FILENAME;

	/**
	 * the file name + full path of the file on disk
	 */
	String ivFileName;

	/**
	 * a file handle
	 */
	MFileInputStream* ivPtFile;

	/**
	 * the channelcount of the file
	 */
	MWaveFileHeader ivHeader;

	/**
	 * the length of the file header
	 */
	unsigned int ivHeaderLength;

	/**
	 * the current play position in file
	 */
	unsigned int ivPlayPosition;

	/**
	 * the buffer for buffering the streaming...
	 */
	MStaticByteBuffer* ivPtBuffer;

	/**
	 * the peak file used for drawing
	 */
	MPeakFile* ivPtPeakFile;

	/**
	 * the loaded id for serialisation
	 */
	int ivLoadedId;

public:

	/**
	 * constructor
	 */
	MAudioFile();

	/**
	 * destructor
	 */
	virtual ~MAudioFile();

	/**
	 * opens the audio file stream
	 */
	virtual void openAudioFile( String fileName );

	/**
	 * closes the audio file stream
	 */
	virtual void closeAudioFile();

	/**
	 * returns a string representation
	 */
	virtual String toString();

	/**
	 * returns the class name for runtime purpose
	 */
	virtual IRtti* getRtti() const;

	/**
	 * querries for a specified interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * fire a event into the processor
	 */
	virtual void processEvent( MEvent* ptEvent );

	/**
	 * the IProcessor method streaming the audio file to a soundbuffer
	 */
	virtual void goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt);

	/**
	 * resets the playstate
	 */
	virtual void reset();

	/**
	 * returns the channelcount
	 */
	virtual unsigned int getChannelCount();

	/**
	 * generates the peakfile data
	 */
	virtual void analyzePeaks( IProgress* ivPtProgress );

	/**
	 * returns the sample data length in file in bytes
	 */
	virtual __int64 getSampleDataLength();

	virtual void onProgress( float value );
	virtual void onFinished();

	/**
	 * returns the peak file
	 */
	virtual MPeakFile* getPeakFile();

	/**
	 * loads from the given tree node
	 */
	virtual void load( MTreeNode* ptNode );

	/**
	 * stores as tree node
	 */
	virtual MTreeNode* save();

	/**
	 * returns the serialized id (casted this)
	 */
	virtual int getSerId();

	/**
	 * returns the loaded id
	 */
	virtual int getLoadedId();

	/**
	 * returns a instance of this class
	 */
	static MObject* createInstance();

protected:

	/**
	 * reads an amount of bytes from file
	 * to the specified buffer
	 */
	virtual unsigned int read( unsigned char* ptBuffer, unsigned int length );

	/**
	 * loads next data from file to buffer
	 */
	virtual unsigned int loadBuffer();

	/**
	 * clears the buffer
	 */
	virtual void zeroBuffer();
};

#endif