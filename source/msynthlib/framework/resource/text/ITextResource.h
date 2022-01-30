#ifndef __ITextResource
#define __ITextResource

#include "../../MTypes.h"

/**
 * defines a text resource
 */
class ITextResource
{
public:

	/**
	 * translates the given string and inserts the given values
	 */
	virtual String getString( String id, ... ) = 0;
};

#endif