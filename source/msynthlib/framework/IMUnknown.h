#ifndef __IMUnknown
#define __IMUnknown

#include "MTypes.h"

/**
 * the superinterface of all MInterfaces
 */
class IMUnknown
{
public:

	/**
	 * this method returns a pointer to the implementation
	 * of specified interface (className), like
	 * queryInterface does in Microsoft COM.
	 */
	virtual void* getInterface( const String &className ) const = 0;
};

#endif