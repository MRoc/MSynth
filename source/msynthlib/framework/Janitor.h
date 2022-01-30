#ifndef __JANITOR
#define __JANITOR

template <class T> class Janitor
{
public:

    Janitor(T* const toDelete);
    ~Janitor();

    void orphan();

	T&	operator*() const;
	T*	operator->() const;
	T*	get() const;
	T*	release();
	void reset(T* p = 0);

private:

	Janitor();
    Janitor(const Janitor<T>&);

	T*  fData;
};

template <class T> class ArrayJanitor
{
public:

    ArrayJanitor(T* const toDelete);
    ~ArrayJanitor();

    void orphan();

	T&	operator[](int index) const;
	T*	get() const;
	T*	release();
	void reset(T* p = 0);

private:

	ArrayJanitor();
    ArrayJanitor(const ArrayJanitor<T>& copy);

    T*  fData;
};

//#ifndef __VC6
#include "Janitor.cpp"
//#endif

#endif
