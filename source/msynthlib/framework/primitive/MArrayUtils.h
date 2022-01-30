#ifndef __MArrayUtils
#define __MArrayUtils

#include "MArray.h"

template <class T> class MArrayUtils
{
public:

	/**
	 * returns the sum of the given array
	 */
	static T sum( MArray<T>* array )
	{
		T back = 0;
		T* pt = array->getData();
		T* ptEnd = pt + array->getDataLength();
		for( ;pt<ptEnd;pt++ )
			back += *pt;
		return back;
	}

	/**
 	 * divides every item in the given array by the given divisor.
	 * divisor must not be null!
	 */
	static void div( MArray<T>* array, T divisor )
	{
		T* pt = array->getData();
		T* ptEnd = pt + array->getDataLength();
		for( ;pt<ptEnd;pt++ )
			(*pt) /= divisor;
	}

	/**
 	 * multiplies
	 */
	static void mul( MArray<T>* array, T multi )
	{
		T* pt = array->getData();
		T* ptEnd = pt + array->getDataLength();
		for( ;pt<ptEnd;pt++ )
			(*pt) *= multi;
	}

	/**
 	 * adds
	 */
	static void add( MArray<T>* array, T toAdd )
	{
		T* pt = array->getData();
		T* ptEnd = pt + array->getDataLength();
		for( ;pt<ptEnd;pt++ )
			(*pt) += toAdd;
	}

	/**
 	 * subtracts
	 */
	static void sub( MArray<T>* array, T toSub )
	{
		T* pt = array->getData();
		T* ptEnd = pt + array->getDataLength();
		for( ;pt<ptEnd;pt++ )
			(*pt) -= toSub;
	}
};

typedef MArrayUtils<float> MFloatArrayUtils;
typedef MArrayUtils<double> MDoubleArrayUtils;
typedef MArrayUtils<signed int> MSIntArrayUtils;
typedef MArrayUtils<unsigned int> MUIntArrayUtils;
typedef MArrayUtils<signed short> MSShortArrayUtils;
typedef MArrayUtils<unsigned short> MUShortArrayUtils;
typedef MArrayUtils<signed char> MSCharArrayUtils;
typedef MArrayUtils<unsigned char> MUCharArrayUtils;

#endif