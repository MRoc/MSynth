#include "MArray.h"

/**
 * constructor
 */
template <class T> MArray<T>::MArray() :
	ivPtData( 0 ),
	ivDataLength( 0 )
{
}

/**
 * constructor
 */
template <class T> MArray<T>::MArray( unsigned int length ) :
	ivPtData( 0 ),
	ivDataLength( length )
{
	ivPtData = new T[ ivDataLength ];
}

/**
 * destructor
 */
template <class T> MArray<T>::~MArray()
{
	if( ivPtData )
		delete ivPtData;
}


/**
 * returns the data pointer
 */
template <class T> T* MArray<T>::getData()
{
	return ivPtData;
}

/**
 * returns the data length
 */
template <class T> unsigned int MArray<T>::getDataLength()
{
	return ivDataLength;
}

/**
 * sets the array length and allocates new memory
 */
template <class T> void MArray<T>::setLength( unsigned int len )
{
	if( ivPtData )
		delete ivPtData;
	ivDataLength = len;
	ivPtData = new T[ ivDataLength ];
}

/**
 * index operator
 */
template <class T> T& MArray<T>::operator[](int _Off)
{
	return ivPtData[_Off];
}