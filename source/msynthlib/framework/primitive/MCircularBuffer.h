#ifndef __MCircularBuffer
#define __MCircularBuffer

#include "MArray.h"

template <class T> class MCircularBuffer :
	public MArray<T>
{
protected:

	/**
	 * the write index
	 */
	int ivWriteIndex;

	/**
	 * the read index
	 */
	int ivReadIndex;

public:

	/**
	 * constructor
	 */
	MCircularBuffer();

	/**
	 * constructor
	 */
	MCircularBuffer( unsigned int length );

	/**
	 * destructor
	 */
	virtual ~MCircularBuffer();

	/**
	 * sets the offset from write to read index, dephase
	 */
	virtual void setReadOffset( unsigned int dephaseValue );

	/**
	 * returns the write index
	 */
	unsigned int getWriteIndex();

	/**
	 * returns the read index
	 */
	unsigned int getReadIndex();

	/**
	 * sets the array length and allocates new memory
	 */
	virtual void setLength( unsigned int len );

	/**
	 * reads a value from the buffer
	 */
	inline T read( int offset );

	/**
	 * writes a value into the buffer
	 */
	inline void write( T value );

private:

	/**
	 * wraps the given index arround lower and upper border
	 */
	inline void wrap( int &index );

	/**
	 * wrapping the given pointer on the upper side
	 */
	inline void wrapUpper( int& index );

	/**
	 * wrapping the given pointer on the upper side
	 */
	inline void wrapLower( int& index );
};

#include "MCircularBuffer.cpp"

typedef MCircularBuffer<float> MFloatCircularBuffer;

#endif