#ifndef __MIconWin32Impl
#define __MIconWin32Impl

#include <framework/MTypes.h>
#include <framework/MNativeTypes.h>
#include "../framewnd/MIconImpl.h"

class MIconWin32Impl :
	public MIconImpl
{
protected:

	/** the native windows icon handle */
	HICON ivIconHandle;

	/** number of references */
	int ivRefCount;

public:

	/** constructor */
	MIconWin32Impl();

	/** constructor */
	MIconWin32Impl( unsigned int id );

	/** destructor */
	virtual ~MIconWin32Impl();

	/** loads a icon from native resource bundle */
	virtual bool load( unsigned int id );

	/** returns the native windows icon handle */
	virtual HICON getIcon();

	/** increment reference counter */
	virtual void incRef();

	/** decrement reference counter, if zero returns true */
	virtual bool decRef();
};

#endif