#ifndef __MImageImpl
#define __MImageImpl

#include <framework/MTypes.h>

/** interface defining an imageimpl following the bridge pattern  */
class MImageImpl
{
public:

	/** constructor */
	MImageImpl(){}

	/** destructor */
	virtual ~MImageImpl(){}

	/** returns the width of the image */
	virtual unsigned int getWidth() = 0;

	/** returns the height of the image */
	virtual unsigned int getHeight() = 0;

	/** loads a image from a resource */
	virtual void loadImage( unsigned int resourceId ) = 0;

	/** loads image from MResource */
	virtual void loadImage( String resourceName ) = 0;
};

#endif