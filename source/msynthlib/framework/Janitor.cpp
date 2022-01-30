#ifndef __VC6
	#include "Janitor.h"
#endif

template <class T> Janitor<T>::Janitor(T* const toDelete) :
    fData(toDelete)
{
}

template <class T> Janitor<T>::~Janitor()
{
    reset();
}

template <class T> void
Janitor<T>::orphan()
{
   release();
}


template <class T> T&
Janitor<T>::operator*() const
{
	return *fData;
}


template <class T> T*
Janitor<T>::operator->() const
{
	return fData;
}


template <class T> T*
Janitor<T>::get() const
{
	return fData;
}


template <class T> T*
Janitor<T>::release()
{
	T* p = fData;
	fData = 0;
	return p;
}


template <class T> void Janitor<T>::reset(T* p)
{
	delete fData;
	fData = p;
}

template <class T> ArrayJanitor<T>::ArrayJanitor(T* const toDelete) :
    fData(toDelete)
{
}


template <class T> ArrayJanitor<T>::~ArrayJanitor()
{
	reset();
}

template <class T> void
ArrayJanitor<T>::orphan()
{
   release();
}


template <class T> T&
ArrayJanitor<T>::operator[](int index) const
{
	return fData[index];
}


template <class T> T*
ArrayJanitor<T>::get() const
{
	return fData;
}


template <class T> T*
ArrayJanitor<T>::release()
{
	T* p = fData;
	fData = 0;
	return p;
}


template <class T> void
ArrayJanitor<T>::reset(T* p)
{
	delete [] fData;
	fData = p;
}

