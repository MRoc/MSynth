#ifndef __IElementMatch
#define __IElementMatch

#include <framework/MTypes.h>

#include "IXmlMatchingNode.h"

/**
 * interface defining a element conversion
 * with a new name and som old names
 */
class IElementMatch
{
public:

	/**
	 * returns the name of the new element (can be the same as old one)
	 */
	virtual String getElementName() = 0;

	/**
	 * returns the number of old names this tag had
	 */
	virtual unsigned int getOldNameCount() = 0;

	/**
	 * returns the old name at the specified index
	 */
	virtual String getOldName( unsigned int index ) = 0;
};

#endif