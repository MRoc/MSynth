#ifndef __MSoundBuffer
#define __MSoundBuffer

#include <framework/MTypes.h>
#include "../../../MSynthConstants.h"

/**
 * MSoundBuffer (C)2000-2003 MRoc
 */
class MSoundBuffer
{
protected:

	/**
	 * number of channels
	 */
	unsigned int ivChannelCount;

	/**
	 * length of data
	 */
	unsigned int ivDataLength;

	/**
	 * the channels' data
	 */
	FP **ivData;

public:

	/**
	 * constructor
	 */
	MSoundBuffer( unsigned int channelCount, unsigned int dataLength );

	/**
	 * constructor
	 */
	MSoundBuffer( MSoundBuffer* buffer );

	/**
	 * destructor
	 */
	virtual ~MSoundBuffer();

	/**
	 * returns the channel data
	 */
	virtual FP* getData( unsigned int index );

	/**
	 * returns the number of channels
	 */
	virtual unsigned int getChannelCount();

	/**
	 * returns the data's length
	 */
	virtual unsigned int getDataLength();

	/**
	 * initializes the buffer and
	 * installs a checksum.
	 */
	void createBuffer();

	/**
	 * checks the checksum and
	 * deletes the buffer.
	 */
	void deleteBuffer();

	/**
	 * returns true if the buffer contains any data
	 */
	bool isBufferValid();

	/**
	 * zerosizes the buffer
	 */
	void zeroBuffer();

//	void addBuffer( MSoundBuffer* buffer );

	void fadeOut( unsigned int start, unsigned int stop );

	void export2pcm16bit2channel( MW_SAMPLETYPE *ptData );
	void export2pcm16bit2channel( MW_SAMPLETYPE *ptData, FP pan, FP vol );
};

class MBYTEBuffer
{
protected:

	unsigned long m_size;
	unsigned char* m_data;

public:

	MBYTEBuffer();
	~MBYTEBuffer();

	unsigned char* getData();
	unsigned long getSize();

	void importSoundBuffer( MSoundBuffer* ptBuffer, unsigned int start, unsigned int stop );
	void exportSoundBuffer( MSoundBuffer* ptBuffer, unsigned int start, unsigned int stop );
};

#endif
