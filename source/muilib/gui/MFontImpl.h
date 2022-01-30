#ifndef __MFontImpl
#define __MFontImpl

#include <framework/primitive/MSize.h>

/**
 * interface definition of a 
 * font object
 */
class MFontImpl
{
public:

	/** constructor */
	MFontImpl(){};

	/** destructor */
	virtual ~MFontImpl(){};

	/** returns the size a text woule require when rendered */
	virtual MSize getTextSize( const String& text ) const = 0;

	/** returns the abc[0-255] width's of characters */
	virtual const int* getABCWidth() const = 0;
};

#endif