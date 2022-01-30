#ifndef __MIconImpl
#define __MIconImpl

/** base implementation of platform icon */
class MIconImpl
{
public:

	/** constructor */
	MIconImpl(){};

	/** destructor */
	virtual ~MIconImpl(){};

	/** loads a icon from native resource bundle */
	virtual bool load( unsigned int id ) = 0;

	/** increment reference counter */
	virtual void incRef() = 0;

	/** decrement reference counter, if zero returns true */
	virtual bool decRef() = 0;
};

#endif