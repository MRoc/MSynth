#ifndef __MIcon
#define __MIcon

#include "MIconImpl.h"

/** a platformindependent icon with bridge impl */
class MIcon
{
protected:

	/** the bridge impl */
	MIconImpl* ivImpl;

public:

	/** default constructor */
	MIcon();

	/** default constructor */
	MIcon( const MIcon& icon );

	/** constructor */
	MIcon( unsigned int id );

	/** destructor */
	virtual ~MIcon();

	/** loads a icon from native resource bundle */
	virtual bool load( unsigned int id );

	/** returns the native impl */
	virtual MIconImpl* getImpl();
};

#endif