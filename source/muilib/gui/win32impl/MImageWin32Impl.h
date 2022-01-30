#ifndef __MImageWin32Impl
#define __MImageWin32Impl

#include <framework/MTypes.h>
#include <framework/MNativeTypes.h>
#include "../MImageImpl.h"

/** bridge pattern/impl idiom/compiler firewall for a native implementation of a image */
class MImageWin32Impl :
	public MImageImpl
{
protected:

	/** the internal bitmap */
	HBITMAP ivBitmap;

	/** the bitmap info */
	BITMAP ivBitmapInfo;

public:

	/** constructor */
	MImageWin32Impl();

	/** constructor with a windows bitmap */
	MImageWin32Impl( HBITMAP win32Bitmap );

	/** constructor loading the specified image from a resource */
	MImageWin32Impl( unsigned int resourceId );

	/** constructor loading the bitmap from a MResource */
	MImageWin32Impl( String resourceName );

	/** destructor */
	virtual ~MImageWin32Impl();

	/** loads a image from a resource */
	void loadImage( unsigned int resourceId );

	/** loads image from MResource */
	void loadImage( String resourceName );

	/** returns the width of the image */
	unsigned int getWidth();

	/** returns the height of the image */
	unsigned int getHeight();

	/** returns the internal wrapped bitmap */
	HBITMAP getHandle();
};

#endif