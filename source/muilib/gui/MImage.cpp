#include "MImage.h"
#include "win32impl/MImageWin32Impl.h"

/**
 * the runtime type info
 */
MRtti MImage::gvRtti = MRtti( "MImage", 0 );

/**
 * constructor
 */
MImage::MImage() :
	ivImpl( new MImageWin32Impl() )
{
}

/**
 * constructor with a windows bitmap
 */
MImage::MImage( MImageImpl* pImpl ) :
	ivImpl( pImpl )
{
}

/**
 * constructor loading the specified image from a resource
 */
MImage::MImage( unsigned int resourceId ) :
	ivImpl( new MImageWin32Impl( resourceId ) )
{
}

/**
 * constructor loading the bitmap from a MResource
 */
MImage::MImage( String resourceName ) :
	ivImpl( new MImageWin32Impl( resourceName ) )
{
}

/** 
 * destructor
 */
MImage::~MImage()
{
	SAFE_DELETE( ivImpl );
}

/**
 * loads a image from a resource
 */
void MImage::loadImage( unsigned int resourceId )
{
	ivImpl->loadImage( resourceId );
}

/**
 * loads image from MResource
 */
void MImage::loadImage( String resourceName )
{
	ivImpl->loadImage( resourceName );
}

/**
 * returns the runtime type info
 */
IRtti* MImage::getRtti() const
{
	return &gvRtti;
}

/**
 * this method returns a pointer to the implementation
 * of specified interface (className), like
 * queryInterface does in Microsoft COM.
 */
void* MImage::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MImage*)this);
	else
		return MObject::getInterface( className );
}

/**
 * returns the width of the image
 */
unsigned int MImage::getWidth()
{
	return ivImpl->getWidth();
}

/**
 * returns the height of the image
 */
unsigned int MImage::getHeight()
{
	return ivImpl->getHeight();
}

/**
 * returns the internal wrapped bitmap
 */
MImageImpl* MImage::getImpl()
{
	return ivImpl;
}