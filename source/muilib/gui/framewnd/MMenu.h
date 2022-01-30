#ifndef __MMenu
#define __MMenu

#include "MMenuImpl.h"

/** a platform independent menu with bridge impl */
class MMenu
{
protected:
	
	/** the native implementation */
	MMenuImpl* ivImpl;

public:

	/** constructor */
	MMenu();

	/** destructor */
	virtual ~MMenu();

	/** loads the menu from native resource bundle */
	virtual bool load( unsigned int id );

	/** returns the native impl */
	virtual MMenuImpl* getImpl();
};

#endif