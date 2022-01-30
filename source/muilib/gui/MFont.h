#ifndef __MFont
#define __MFont

#include <framework/primitive/MSize.h>
class MFontImpl;

/**
 * implementation of a font
 */
class MFont :
	public MObject
{
public:

	/** the runtime type info */
	static MRtti gvRtti;

protected:

	/** the font implementation (bridge pattern)*/
	MFontImpl* ivImpl;

	/**
	 * used for everything drawing
	 */
	static MFont* gvFont;

public:

	/** constructor */
	MFont();

	/** constructor */
	MFont( String font, int size );

	/** destructor */
	virtual ~MFont();

	/** returns the runtime type information */
	virtual IRtti* getRtti() const;

	/** querries for a specified interface (inheritanced from IMUnknown) */
	virtual void* getInterface( const String &className ) const;

	/** returns the internal implementation */
	virtual MFontImpl* getImpl();

	/** returns the size a text woule require when rendered */
	virtual MSize getTextSize( const String& text ) const;

	/** returns the abc[0-255] width's of characters */
	virtual const int* getABCWidth() const;

	/** returns the shared font object */
	static MFont* getFont();

	/** releases the shared font */
	static void release();
};

#endif