#include "MFontWin32Impl.h"

/**
* the std font
*/
const String MFontWin32Impl::STD_FONT = "Courir";

/**
* the std font size
*/
#ifdef __VC6
	const unsigned int MFontWin32Impl::STD_FONT_SIZE = 16;
#else
	const unsigned int MFontWin32Impl::STD_FONT_SIZE = 14;
#endif

/** constructor */
MFontWin32Impl::MFontWin32Impl() :
	ivFont( 0 ),
	ivDc( 0 )
{
	memset( ivABCWidths, 0, 256 );
	
	ivFont = createFont( STD_FONT, STD_FONT_SIZE );
	ASSERT( ivFont );

	ivDc = CreateCompatibleDC( NULL );
	ASSERT( ivDc );

	if( ivFont && ivDc )
		for( int i=0;i<256;i++ )
		{
			MSize size = getTextSize( String( (char)i ) );
			ivABCWidths[ i ] = size.getWidth();
		}
}

/** constructor */
MFontWin32Impl::MFontWin32Impl( String font, int size ) :
	ivFont( createFont( font, size ) ),
	ivDc( 0 )
{
	ivFont = createFont( font, size );
	ASSERT( ivFont );

	ivDc = CreateCompatibleDC( NULL );
	ASSERT( ivDc );
}

/** destructor */
MFontWin32Impl::~MFontWin32Impl()
{
	if( ivFont )
	{
		DeleteObject( ivFont );
		ivFont = 0;
	}

	if( ivDc )
	{
		DeleteDC( ivDc );
		ivDc = 0;
	}
}

/** returns the internal wrapped font handle */
HFONT MFontWin32Impl::getHandle()
{
	return ivFont;
}

/** returns the size a text woule require when rendered */
MSize MFontWin32Impl::getTextSize( const String& text ) const
{
	ASSERT( ivDc );
	ASSERT( ivFont );

	MSize back;
	HGDIOBJ oldFont = SelectObject( ivDc, ivFont );
	SIZE s;

	BOOL ext =
		GetTextExtentPoint32(
			ivDc,          // handle to DC
			text,				// text string
			text.GetLength(),	// characters in string
			&s					// string size
		);
	ASSERT( ext );

	SelectObject( ivDc, oldFont );

	return MSize( s.cx, s.cy );
}

/** returns the abc[0-255] width's of characters */
const int* MFontWin32Impl::getABCWidth() const
{
	return ivABCWidths;
}

/** creates a native font handle */
HFONT MFontWin32Impl::createFont( String font, unsigned int size )
{
	LOGFONT lf;
	memset( &lf, 0, sizeof(LOGFONT) );
	lf.lfHeight	= size;                     
	strcpy( lf.lfFaceName, font.getData() );  
	return CreateFontIndirect( &lf );
}
