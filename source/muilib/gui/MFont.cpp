#include "MFont.h"
#include <framework/io/MLogger.h>
#include "win32impl/MFontWin32Impl.h"

/** used for everything drawing */
MFont* MFont::gvFont = 0;

/** the runtime type info */
MRtti MFont::gvRtti = MRtti( "MFont", 0 );

/** constructor */
MFont::MFont() :
	ivImpl( new MFontWin32Impl() )
{
}

/** constructor */
MFont::MFont( String font, int size ) :
	ivImpl( new MFontWin32Impl( font, size ) )
{
}

/** destructor */
MFont::~MFont()
{
	SAFE_DELETE( ivImpl );
}

/** returns the runtime type information */
IRtti* MFont::getRtti() const
{
	return &gvRtti;
}

/** querries for a specified interface (inheritanced from IMUnknown) */
void* MFont::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MFont*)this);
	else
		return MObject::getInterface( className );
}

/** returns the internal wrapped font obj */
MFontImpl* MFont::getImpl()
{
	return ivImpl;
}

/** returns the size a text woule require when rendered */
MSize MFont::getTextSize( const String& text ) const
{
	return ivImpl->getTextSize( text );
}

/** returns the abc[0-255] width's of characters */
const int* MFont::getABCWidth() const
{
	return ivImpl->getABCWidth();
}

/** returns the shared font object */
MFont* MFont::getFont()
{
	if( gvFont == 0 )
		gvFont = new MFont();
	return gvFont;
}

/** releases the shared font */
void MFont::release()
{
	if( gvFont )
	{
		delete gvFont;
		gvFont = 0;
	}
}