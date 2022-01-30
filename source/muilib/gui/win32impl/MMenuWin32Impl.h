#ifndef __MMenuWin32Impl
#define __MMenuWin32Impl

#include <framework/MTypes.h>
#include <framework/MNativeTypes.h>
#include "../framewnd/MMenuImpl.h"

/** implementation of a win32 menu */
class MMenuWin32Impl :
	public MMenuImpl
{
protected:

	/** the native win32 menu handle */
	HMENU ivMenu;

public:

	/** constructor */
	MMenuWin32Impl();

	/** destructor */
	virtual ~MMenuWin32Impl();

	/** loads a menu from native resource bundle */
	virtual bool load( unsigned int id );

	/** returns the native win32 menu handle */
	virtual HMENU getHandle();
};


#endif