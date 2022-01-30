#include "MImageWin32Impl.h"
#include <framework/io/MLogger.h>
#include "../MApp.h"
#include <framework/resource/MResourceManager.h>
#include "MAppWin32Impl.h"

/** constructor */
MImageWin32Impl::MImageWin32Impl() :
	ivBitmap( 0 )
{
}

/** constructor with a windows bitmap */
MImageWin32Impl::MImageWin32Impl( HBITMAP win32Bitmap ) :
	ivBitmap( win32Bitmap )
{
	if( GetObject( ivBitmap, sizeof(ivBitmapInfo), &ivBitmapInfo ) == 0 )
		MLogger::logError( "MImageWin32Impl::MImageWin32Impl: GetObject failed" );
}

/** constructor loading the specified image from a resource */
MImageWin32Impl::MImageWin32Impl( unsigned int resourceId ) :
	ivBitmap( 0 )
{
	loadImage( resourceId );
}

/** constructor loading the bitmap from a MResource */
MImageWin32Impl::MImageWin32Impl( String resourceName ) :
	ivBitmap( 0 )
{
	loadImage( resourceName );
}

/** destructor */
MImageWin32Impl::~MImageWin32Impl()
{
	if( ivBitmap )
	{
		if( DeleteObject( ivBitmap ) == FALSE )
			MLogger::logError( "MImageWin32Impl::~MImageWin32Impl: DeleteObject failed" );
		ivBitmap = 0;
	}
}

/** loads a image from a resource */
void MImageWin32Impl::loadImage( unsigned int resourceId )
{
	if( ivBitmap )
	{
		if( DeleteObject( ivBitmap ) == FALSE )
			MLogger::logError( "MImageWin32Impl::~loadImage: DeleteObject failed" );
		ivBitmap = 0;
	}

	ivBitmap =
		LoadBitmap(
			((MAppWin32Impl*)MApp::getInstance()->getImpl())->getHandle(),
			MAKEINTRESOURCE( resourceId ) );

	if( ivBitmap )
	{
		if( GetObject( ivBitmap, sizeof(ivBitmapInfo), &ivBitmapInfo ) == 0 )
			MLogger::logError( "MImageWin32Impl::loadImage: GetObject failed" );
	}
	else
		MLogger::logError( "MImageWin32Impl::loadImage: LoadBitmap failed" );
}

/** loads image from MResource */
void MImageWin32Impl::loadImage( String resourceName )
{
	if( ivBitmap )
	{
		if( DeleteObject( ivBitmap ) == FALSE )
			MLogger::logError( "MImageWin32Impl::~MImageWin32Impl: DeleteObject failed" );
		ivBitmap = 0;
	}

	String fileName = MResourceManager::getInstance()->getFile( resourceName );
	ivBitmap = (HBITMAP)
		LoadImage(
			((MAppWin32Impl*)MApp::getInstance()->getImpl())->getHandle(),
			fileName,
			IMAGE_BITMAP,
			0,
			0,
			LR_LOADFROMFILE | LR_DEFAULTSIZE );

	if( ivBitmap )
	{
		if( GetObject( ivBitmap, sizeof(ivBitmapInfo), &ivBitmapInfo ) == 0 )
			MLogger::logError( "MImageWin32Impl::loadImage: GetObject failed" );
	}
	else
		MLogger::logError( "MImageWin32Impl::loadImage: LoadBitmap failed (\"%s\"/\"%s\")", resourceName.getData(), fileName.getData() );
}

/** returns the width of the image */
unsigned int MImageWin32Impl::getWidth()
{
	return ivBitmapInfo.bmWidth;
}

/** returns the height of the image */
unsigned int MImageWin32Impl::getHeight()
{
	return ivBitmapInfo.bmHeight;
}

/** returns the internal wrapped bitmap */
HBITMAP MImageWin32Impl::getHandle()
{
	return ivBitmap;
}