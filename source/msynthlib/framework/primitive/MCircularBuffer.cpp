#include "MCircularBuffer.h"

/**
 * constructor
 */
template <class T> MCircularBuffer<T>::MCircularBuffer() :
	ivWriteIndex( 0 ),
	ivReadIndex( 0 )
{
}

/**
 * destructor
 */
template <class T> MCircularBuffer<T>::~MCircularBuffer()
{
}

/**
 * sets the offset from write to read index, dephase
 */
template <class T> void MCircularBuffer<T>::setReadOffset( unsigned int dephaseValue )
{
	ASSERT( dephaseValue <= ivDataLength );
	ivReadIndex = ivWriteIndex + dephaseValue;
	wrap( ivReadIndex );
}

/**
 * reads a value from the buffer
 */
template <class T> T MCircularBuffer<T>::read( int offset )
{
	int index = ivReadIndex + offset;
	wrap( index );
	return ivPtData[ index ];
}

/**
 * writes a value into the buffer
 */
template <class T> void MCircularBuffer<T>::write( T value )
{
	ivPtData[ ivWriteIndex ] = value;

	wrapUpper( ++ivWriteIndex );
	wrapUpper( ++ivReadIndex );
}

/**
 * sets the array length and allocates new memory
 */
template <class T> void MCircularBuffer<T>::setLength( unsigned int len )
{
	if( ivPtData )
		delete ivPtData;
	ivDataLength = len;
	ivPtData = new T[ ivDataLength ];
	wrap( ivReadIndex );
	wrap( ivWriteIndex );
	memset( ivPtData, 0, ivDataLength * sizeof(T) );
}

/**
 * wraps the given index arround lower and upper border
 */
template <class T> void MCircularBuffer<T>::wrap( int &index )
{
	if( index >= (int)ivDataLength )
		index -= (int)ivDataLength;
	else if( index < 0 )
		index += (int)ivDataLength;
}

/**
 * wrapping the given pointer on the upper side
 */
template <class T> void MCircularBuffer<T>::wrapUpper( int& index )
{
	if( index >= (int)ivDataLength )
		index -= (int)ivDataLength;
}

/**
 * wrapping the given pointer on the upper side
 */
template <class T> void MCircularBuffer<T>::wrapLower( int& index )
{
	if( index < 0 )
		index += (int)ivDataLength;
}


/**
 * returns the write index
 */
template <class T> unsigned int MCircularBuffer<T>::getWriteIndex()
{
	return ivWriteIndex;
}

/**
 * returns the read index
 */
template <class T> unsigned int MCircularBuffer<T>::getReadIndex()
{
	return ivReadIndex;
}
