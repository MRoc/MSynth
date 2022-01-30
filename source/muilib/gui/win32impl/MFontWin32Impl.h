#ifndef __MFontWin32Impl
#define __MFontWin32Impl

#include <framework/MTypes.h>
#include <framework/MNativeTypes.h>
#include "../MFontImpl.h"

/** a font implementation on win32 following the bridge pattern */
class MFontWin32Impl :
	public MFontImpl
{
protected:

	/** the native font handle */
	HFONT ivFont;

	/** the dc used for text size calculation */
	HDC ivDc;

	/** the alphabet width */
	int ivABCWidths[256];

	/** the std font */
	static const String STD_FONT;

	/** the std font size */
	static const unsigned int STD_FONT_SIZE;

public:

	/** constructor */
	MFontWin32Impl();

	/** constructor */
	MFontWin32Impl( String font, int size );

	/** destructor */
	virtual ~MFontWin32Impl();

	/** returns the internal wrapped font handle */
	virtual HFONT getHandle();

	/** returns the size a text woule require when rendered */
	virtual MSize getTextSize( const String& text ) const;

	/** returns the abc[0-255] width's of characters */
	virtual const int* getABCWidth() const;

protected:

	/** creates a native font handle */
	HFONT createFont( String font, unsigned int size );
};

#endif