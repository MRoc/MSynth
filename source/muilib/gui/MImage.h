/**
 * (C) 2003 MRoc
 */
#ifndef __MImage
#define __MImage

#include <framework/MObject.h>

class MImageImpl;

/**
 * implementation of a image object
 * wrapping a MFC CBitmap and a
 * BITMAP info struct.
 */
class MImage :
	public MObject
{
public:

	/** the runtime type info */
	static MRtti gvRtti;

protected:

	/** the native impl (bridge pattern) */
	MImageImpl* ivImpl;

public:

	/** constructor */
	MImage();

	/** constructor with a native impl */
	MImage( MImageImpl* impl );

	/** constructor loading the specified image from a resource */
	MImage( unsigned int resourceId );

	/** constructor loading the bitmap from a MResource */
	MImage( String resourceName );

	/** destructor */
	virtual ~MImage();

	/** loads a image from a resource */
	virtual void loadImage( unsigned int resourceId );

	/** loads image from MResource */
	virtual void loadImage( String resourceName );

	/** returns the width of the image */
	virtual unsigned int getWidth();

	/** returns the height of the image */
	virtual unsigned int getHeight();

	/** returns the internal implementation */
	virtual MImageImpl* getImpl();

	/**
	 * this method returns a pointer to the implementation
	 * of specified interface (className), like
	 * queryInterface does in Microsoft COM.
	 */
	virtual void* getInterface( const String &className ) const;

	/** returns the runtime type info */
	virtual IRtti* getRtti() const;
};

#endif