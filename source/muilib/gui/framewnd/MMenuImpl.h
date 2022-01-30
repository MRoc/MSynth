#ifndef __MMenuImpl
#define __MMenuImpl

/** base class of all native menu implementations */
class MMenuImpl
{
public:

	/** constructor */
	MMenuImpl(){}

	/** destructor */
	virtual ~MMenuImpl(){}

	/** loads a menu from native resource bundle */
	virtual bool load( unsigned int id ) = 0;
};

#endif