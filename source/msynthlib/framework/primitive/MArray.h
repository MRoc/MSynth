#ifndef __MArray
#define __MArray

#include "../MObject.h"

template <class T> class MArray
{
protected:

	/**
	 * the data pointer
	 */
	T* ivPtData;

	/**
	 * the data length
	 */
	unsigned int ivDataLength;

public:

	/**
	 * constructor
	 */
	MArray();

	/**
	 * constructor
	 */
	MArray( unsigned int length );

	/**
	 * destructor
	 */
	virtual ~MArray();

	/**
	 * returns the data pointer
	 */
	virtual T* getData();

	/**
	 * returns the data length
	 */
	virtual unsigned int getDataLength();

	/**
	 * sets the array length and allocates new memory
	 */
	virtual void setLength( unsigned int len );

	/**
	 * index operator
	 */
	T& operator[](int _Off);
};

typedef MArray<float> MFloatArray;
typedef MArray<double> MDoubleArray;
typedef MArray<signed int> MSIntArray;
typedef MArray<unsigned int> MUIntArray;
typedef MArray<signed short> MSShortArray;
typedef MArray<unsigned short> MUShortArray;
typedef MArray<signed char> MSCharArray;
typedef MArray<unsigned char> MUCharArray;

#include "MArray.cpp"

#endif