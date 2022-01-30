#include "MUiGraphics.h"
#include "win32impl/MImageWin32Impl.h"
#include "win32impl/MFontWin32Impl.h"
#include <framework/io/MLogger.h>

/**
 * the runtime type info
 */
MRtti MUiGraphics::gvRtti = MRtti( "MUiGraphics", 0 );

/**
 * constructor
 */
MUiGraphics::MUiGraphics() :
	ivHdcc( 0 ),
	ivSrcHdc( 0 )
{
}

/**
 * constructor
 */
MUiGraphics::MUiGraphics( const MUiGraphics& ui ) :
	ivHdcc( ui.ivHdcc ),
	ivSrcHdc( ui.ivSrcHdc ),
	ivPoint( ui.ivPoint )
{
}

/**
 * constructor
 */
MUiGraphics::MUiGraphics( HDC hdc ) :
	ivHdcc( hdc ),
	ivSrcHdc( 0 )
{
}

/**
 * constructor
 */
MUiGraphics::MUiGraphics( HDC hdc1, HDC hdc2 ) :
	ivHdcc( hdc1 ),
	ivSrcHdc( hdc2 )
{
}

/**
 * destructor
 */
MUiGraphics::~MUiGraphics()
{
}

/**
 * returns the runtime type information
 */
IRtti* MUiGraphics::getRtti() const
{
	return &gvRtti;
}

/**
 * querries for a specified interface
 * (inheritanced from IMUnknown)
 */
void* MUiGraphics::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MUiGraphics*)this);
	else if( className == "IGraphics" )
		return (void*) ((IGraphics*)this);
	else
		return MObject::getInterface( className );
}

/**
 * sets the offset used for all drawing operations
 */
void MUiGraphics::setOffset( const MPoint& p )
{
	ivPoint = p;
}

/**
 *  sets the x offset
 */
void MUiGraphics::setOffsetX( int x )
{
	ivPoint.setX( x );
}

/**
 * returns the offset x
 */
int MUiGraphics::getOffsetX()
{
	return ivPoint.getX();
}

/**
 * sets the y offset
 */
void MUiGraphics::setOffsetY( int y )
{
	ivPoint.setY( y );
}

/**
 * returns the y offset
 */
int MUiGraphics::getOffsetY()
{
	return ivPoint.getY();
}

/**
 * returns the offset used for all drawing operations
 */
MPoint MUiGraphics::getOffset()
{
	return ivPoint;
}

/**
 * draws a line
 */
void MUiGraphics::drawLine( int x1, int y1, int x2, int y2, MColor color )
{
    HPEN pen =
		CreatePen(
			PS_SOLID,	// pen style
			1,			// pen width
			color		// pen color
		);
	ASSERT( pen );

	HGDIOBJ old =
		SelectObject(
			ivHdcc,	// handle to DC
			pen			// handle to object
		);

	BOOL suc1 =
		MoveToEx(
			ivHdcc,				// handle to device context
			ivPoint.getX() + x1,	// x-coordinate of new current position
			ivPoint.getY() + y1,	// y-coordinate of new current position
			NULL					// old current position
		);
	ASSERT( suc1 == TRUE );

	BOOL suc2 =
		LineTo(
			ivHdcc,				// handle to device context
			ivPoint.getX() + x2,	// x-coordinate of new current position
			ivPoint.getY() + y2		// y-coordinate of new current position
		);
	ASSERT( suc2 == TRUE );

	SelectObject(
		ivHdcc,	// handle to DC
		old			// handle to object
	);

	BOOL suc3 = DeleteObject( pen );
	ASSERT( suc3 == TRUE );
}

/**
 * draws a line rectangle
 */
void MUiGraphics::drawRect( int x1, int y1, int x2, int y2, MColor color )
{
    HBRUSH brush =
		CreateSolidBrush( 
			color		// pen color
		);

	HGDIOBJ old =
		SelectObject(
			ivHdcc,	// handle to DC
			brush		// handle to object
		);

	RECT r;
	r.left = ivPoint.getX() + x1;
	r.right = ivPoint.getX() + x2;
	r.top = ivPoint.getY() + y1;
	r.bottom = ivPoint.getY() + y2;

	FrameRect(
		ivHdcc,	// handle to dc
		&r,
		brush );

	SelectObject(
		ivHdcc,	// handle to DC
		old			// handle to object
	);

	DeleteObject( brush );
}

/**
 * draws a line rectangle
 */
void MUiGraphics::drawRect( const MRect& ptRect, MColor color )
{
	drawRect(
		ptRect.getX(),
		ptRect.getY(),
		ptRect.getRight(),
		ptRect.getBottom(),
		color );
}

/**
 * draws a solid rectangle
 */
void MUiGraphics::fillRect( int x1, int y1, int x2, int y2, MColor color )
{
    HBRUSH brush =
		CreateSolidBrush( 
			color		// pen color
		);

	HGDIOBJ old =
		SelectObject(
			ivHdcc,	// handle to DC
			brush		// handle to object
		);

	RECT r;
	r.left = ivPoint.getX() + x1;
	r.right = ivPoint.getX() + x2;
	r.top = ivPoint.getY() + y1;
	r.bottom = ivPoint.getY() + y2;

	FillRect(
		ivHdcc,	// handle to dc
		&r,
		brush );

	SelectObject(
		ivHdcc,	// handle to DC
		old			// handle to object
	);

	DeleteObject( brush );
}

/** draws a filled ellipse */
void MUiGraphics::fillEllipse( const MRect& rect, MColor color )
{
    HBRUSH brush =
		CreateSolidBrush( 
			color		// pen color
		);

    HPEN pen =
		CreatePen(
			PS_SOLID,
			1,
			color		// pen color
		);

	HGDIOBJ old =
		SelectObject(
			ivHdcc,	// handle to DC
			brush		// handle to object
		);

	HGDIOBJ old2 =
		SelectObject(
			ivHdcc,	// handle to DC
			pen		// handle to object
		);

	RECT r;
	r.left = ivPoint.getX() + rect.getX();
	r.right = ivPoint.getX() + rect.getRight();
	r.top = ivPoint.getY() + rect.getY();
	r.bottom = ivPoint.getY() + rect.getBottom();

	Ellipse(
		ivHdcc,	// handle to dc
		ivPoint.getX() + rect.getX(),
		ivPoint.getY() + rect.getY(),
		ivPoint.getX() + rect.getRight(),
		ivPoint.getY() + rect.getBottom() );

	SelectObject(
		ivHdcc,	// handle to DC
		old			// handle to object
	);

	SelectObject(
		ivHdcc,	// handle to DC
		old2			// handle to object
	);

	DeleteObject( brush );
}

/**
 * draws a solid rectangle
 */
void MUiGraphics::fillRect( const MRect& rect, MColor color )
{
	fillRect(
		rect.getX(),
		rect.getY(),
		rect.getX() + rect.getWidth(),
		rect.getY() + rect.getHeight(),
		color );
}

/**
 * paints a filled poly
 */
void MUiGraphics::poly( MPoint** ptPoints, unsigned int pCount, MColor color, MColor lineColor )
{
    HPEN pen =
		CreatePen(
			PS_SOLID,	// pen style
			1,			// pen width
			lineColor	// pen color
		);
    HBRUSH brush =
		CreateSolidBrush( 
			color		// pen color
		);

	HPEN oldPen = (HPEN) SelectObject( ivHdcc, pen );
	HBRUSH oldBrush = (HBRUSH) SelectObject( ivHdcc, brush );

	POINT* points = new POINT[pCount];
	for( unsigned int i=0;i<pCount;i++ )
	{
		points[ i ].x = ivPoint.getX() + ptPoints[ i ]->getX();
		points[ i ].y = ivPoint.getY() + ptPoints[ i ]->getY();
	}
	Polygon( ivHdcc, points, pCount );
	delete points;

	SelectObject( ivHdcc, oldPen );
	SelectObject( ivHdcc, oldBrush );

	DeleteObject( pen );
	DeleteObject( brush );
}

/**
 * draws a poly
 */
void MUiGraphics::poly( MPoint* ptPoints, unsigned int pCount, MColor color, MColor lineColor )
{
    HPEN pen =
		CreatePen(
			PS_SOLID,	// pen style
			1,			// pen width
			lineColor	// pen color
		);
    HBRUSH brush =
		CreateSolidBrush( 
			color		// pen color
		);

	HPEN oldPen = (HPEN) SelectObject( ivHdcc, pen );
	HBRUSH oldBrush = (HBRUSH) SelectObject( ivHdcc, brush );

	POINT* points = new POINT[pCount];
	for( unsigned int i=0;i<pCount;i++ )
	{
		points[ i ].x = ivPoint.getX() + ptPoints[ i ].getX();
		points[ i ].y = ivPoint.getY() + ptPoints[ i ].getY();
	}
	Polygon( ivHdcc, points, pCount );
	delete points;

	SelectObject( ivHdcc, oldPen );
	SelectObject( ivHdcc, oldBrush );

	DeleteObject( pen );
	DeleteObject( brush );
}

/**
 * draws a centered text
 */
void MUiGraphics::drawCenteredText( const String& text, const MRect& ptRect, MColor color, MFont* ptFont )
{
	int oldMode = SetBkMode( ivHdcc, TRANSPARENT );
	COLORREF oldColor = SetTextColor( ivHdcc, color );
	HGDIOBJ oldFont = SelectObject( ivHdcc, ((MFontWin32Impl*)ptFont->getImpl())->getHandle() );

	RECT r;
	r.left = ivPoint.getX() + ptRect.getX();
	r.top = ivPoint.getY() + ptRect.getY();
	r.right = ivPoint.getX() + ptRect.getX() + ptRect.getWidth();
	r.bottom = ivPoint.getY() + ptRect.getY() + ptRect.getHeight();
	DrawText( ivHdcc, text, text.GetLength(), &r, DT_CENTER|DT_VCENTER|DT_SINGLELINE );

	SelectObject( ivHdcc, oldFont );
	SetTextColor( ivHdcc, oldColor );
	SetBkMode( ivHdcc, oldMode );
}

/**
 * draws a centered text
 */
void MUiGraphics::drawCenteredText( const String& text, const MRect& rect, MColor color )
{
	int oldMode = SetBkMode( ivHdcc, TRANSPARENT );
	COLORREF oldColor = SetTextColor( ivHdcc, color );
	HGDIOBJ oldFont = SelectObject( ivHdcc, ((MFontWin32Impl*)MFont::getFont()->getImpl())->getHandle() );

	RECT r;
	r.left = ivPoint.getX() + rect.getX();
	r.top = ivPoint.getY() + rect.getY();
	r.right = ivPoint.getX() + rect.getX() + rect.getWidth();
	r.bottom = ivPoint.getY() + rect.getY() + rect.getHeight();
	DrawText( ivHdcc, text, text.GetLength(), &r, DT_CENTER|DT_VCENTER|DT_SINGLELINE );

	SelectObject( ivHdcc, oldFont );
	SetTextColor( ivHdcc, oldColor );
	SetBkMode( ivHdcc, oldMode );
}

/**
* draws a text
*/
void MUiGraphics::drawText( const String& text, const MRect& rect, MColor color, MFont* ptFont )
{
	int oldMode = SetBkMode( ivHdcc, TRANSPARENT );
	COLORREF oldColor = SetTextColor( ivHdcc, color );
	HGDIOBJ oldFont = SelectObject( ivHdcc, ((MFontWin32Impl*)ptFont->getImpl())->getHandle() );

	RECT r;
	r.left = ivPoint.getX() + rect.getX();
	r.top = ivPoint.getY() + rect.getY();
	r.right = ivPoint.getX() + rect.getX() + rect.getWidth();
	r.bottom = ivPoint.getY() + rect.getY() + rect.getHeight();
	DrawText( ivHdcc, text, text.GetLength(), &r, DT_VCENTER|DT_SINGLELINE );

	SelectObject( ivHdcc, oldFont );
	SetTextColor( ivHdcc, oldColor );
	SetBkMode( ivHdcc, oldMode );
}

/**
 * draws a text
 */
void MUiGraphics::drawText( const String& text, const MRect& rect, MColor color )
{
	ASSERT( ivHdcc );

	int oldMode = SetBkMode( ivHdcc, TRANSPARENT );
	COLORREF oldColor = SetTextColor( ivHdcc, color );
	HGDIOBJ oldFont = SelectObject( ivHdcc, ((MFontWin32Impl*)MFont::getFont()->getImpl())->getHandle() );

	RECT r;
	r.left = ivPoint.getX() + rect.getX();
	r.top = ivPoint.getY() + rect.getY();
	r.right = r.left + rect.getWidth();
	r.bottom = r.top + rect.getHeight();

	DrawText(
		ivHdcc,
		text.getData(),
		text.GetLength(),
		&r,
		DT_VCENTER|DT_SINGLELINE );

	SelectObject( ivHdcc, oldFont );
	SetTextColor( ivHdcc, oldColor );
	SetBkMode( ivHdcc, oldMode );
}

/**
* returns the size a text woule require when rendered
*/
MSize MUiGraphics::getTextSize( const String& text, MFont* ptFont )
{
	if( ptFont == 0 )
		ptFont = MFont::getFont();

	MSize back;
	HGDIOBJ oldFont = SelectObject( ivHdcc, ((MFontWin32Impl*)MFont::getFont()->getImpl())->getHandle() );
	SIZE s;

	BOOL ext =
		GetTextExtentPoint32(
			ivHdcc,          // handle to DC
			text,				// text string
			text.GetLength(),	// characters in string
			&s					// string size
		);
	ASSERT( ext );

	SelectObject( ivHdcc, oldFont );

	return MSize( s.cx, s.cy );
}

/**
* draws a image
*/
void MUiGraphics::drawImage( MImage* ptImage, int x, int y )
{
	BITMAP bm;
	HBITMAP bmp = ((MImageWin32Impl*)ptImage->getImpl())->getHandle();
	if( GetObject( bmp, sizeof( BITMAP ), &bm ) == 0 )
		MLogger::logError( "MUiGraphics::drawImage.GetObject failed" );

	HDC imageHdc =
		CreateCompatibleDC(
			ivHdcc
		);
	ASSERT( imageHdc );

	HGDIOBJ old =
		SelectObject(
			imageHdc,
			bmp
		);

	BOOL bb =
		BitBlt(
			ivHdcc,
			ivPoint.getX() + x,
			ivPoint.getY() + y,
			bm.bmWidth,
			bm.bmHeight,
			imageHdc,
			0,
			0,
			SRCCOPY );
	ASSERT( bb == TRUE );

	SelectObject( imageHdc, old );

	BOOL dc = DeleteDC( imageHdc );
	ASSERT( dc == TRUE );
}

/**
 * draws a image
 */
void MUiGraphics::drawImageTransparent( MImage* ptImage, int x, int y, MColor transparentColor )
{
	drawTransparent(
		ivPoint.getX() + x,
		ivPoint.getY() + y,
		ivHdcc,
		((MImageWin32Impl*)ptImage->getImpl())->getHandle(),
		transparentColor );
}

void MUiGraphics::drawTransparent( int x, int y, HDC hdc, HBITMAP bitmap, MColor color )
{
	BITMAP bm;
	if( GetObject( bitmap, sizeof( BITMAP ), &bm ) == 0 )
		MLogger::logError(
			"MUiGraphics::drawTransparent: GetObject failed" );

	HDC imageDc = CreateCompatibleDC( hdc );
	if( imageDc == 0 )
		MLogger::logError(
			"MUiGraphics::drawTransparent: imageDc CreateCompatibleDC failed" );
	SelectObject( imageDc, bitmap );
	SetBkColor( imageDc, color );

	HDC maskDc = CreateCompatibleDC( hdc );
	if( maskDc == 0 )
		MLogger::logError(
			"MUiGraphics::drawTransparent: maskDc CreateCompatibleDC failed" );
	HBITMAP maskBitmap = CreateBitmap( bm.bmWidth, bm.bmHeight, 1, 1, NULL );
	if( maskBitmap == 0 )
		MLogger::logError(
			"MUiGraphics::drawTransparent: maskBitmap CreateBitmap failed" );
	SelectObject( maskDc, maskBitmap );
	if( BitBlt( maskDc, 0, 0, bm.bmWidth, bm.bmHeight, imageDc, 0, 0, SRCCOPY ) == FALSE )
		MLogger::logError(
			"MUiGraphics: BitBlt1 failed" );

	HDC orDc = CreateCompatibleDC( hdc );
	if( orDc == 0 )
		MLogger::logError(
			"MUiGraphics::drawTransparent: orDc CreateCompatibleDC failed" );
	HBITMAP orBitmap = CreateCompatibleBitmap( hdc, bm.bmWidth, bm.bmHeight );
	if( orBitmap == 0 )
		MLogger::logError(
			"MUiGraphics::drawTransparent: orBitmap CreateCompatibleBitmap failed" );
	SelectObject( orDc, orBitmap );
	if( BitBlt( orDc, 0, 0, bm.bmWidth, bm.bmHeight, imageDc, 0, 0, SRCCOPY ) == FALSE )
		MLogger::logError(
			"MUiGraphics: BitBlt2 failed" );

	if( BitBlt( orDc, 0, 0, bm.bmWidth, bm.bmHeight, maskDc, 0, 0, 0x220326 ) == FALSE )
		MLogger::logError(
			"MUiGraphics: BitBlt3 failed" );
	
	HDC tempDc = CreateCompatibleDC( hdc );
	if( tempDc == 0 )
		MLogger::logError(
			"MUiGraphics::drawTransparent: tempDc CreateCompatibleDC failed" );
	HBITMAP tempBitmap = CreateCompatibleBitmap( imageDc, bm.bmWidth, bm.bmHeight );
	if( tempBitmap == 0 )
		MLogger::logError(
			"MUiGraphics::drawTransparent: tempBitmap CreateCompatibleBitmap failed" );
	SelectObject( tempDc, tempBitmap );
	
	// copy original background...
	if( BitBlt( tempDc, 0, 0, bm.bmWidth, bm.bmHeight, hdc, x, y, SRCCOPY ) == FALSE )
		MLogger::logError(
			"MUiGraphics: BitBlt4 failed" );

	// AND mask...
	if( BitBlt( tempDc, 0, 0, bm.bmWidth, bm.bmHeight, maskDc, 0, 0, SRCAND ) == FALSE )
		MLogger::logError(
			"MUiGraphics: BitBlt5 failed" );

	// OR orMask...
	if( BitBlt( tempDc, 0, 0, bm.bmWidth, bm.bmHeight, orDc, 0, 0, SRCPAINT ) == FALSE )
		MLogger::logError(
			"MUiGraphics: BitBlt6 failed" );


	// copy temp to dc...
	if( BitBlt( hdc, x, y, bm.bmWidth, bm.bmHeight, tempDc, 0, 0, SRCCOPY ) == FALSE )
		MLogger::logError(
			"MUiGraphics: BitBlt7 failed" );

	BOOL ddc1 = DeleteDC( tempDc );
	ASSERT( ddc1 == TRUE );
	BOOL ddc2 = DeleteDC( orDc );
	ASSERT( ddc2 == TRUE );
	BOOL ddc3 = DeleteDC( maskDc );
	ASSERT( ddc3 == TRUE );
	BOOL ddc4 = DeleteDC( imageDc );
	ASSERT( ddc4 == TRUE );

	BOOL do1 = DeleteObject( tempBitmap );
	ASSERT( do1 == TRUE );
	BOOL do2 = DeleteObject( orBitmap );
	ASSERT( do2 == TRUE );
	BOOL do3 = DeleteObject( maskBitmap );
	ASSERT( do3 == TRUE );
}

/**
 * clips the given rect from beeing redrawn
 */
void MUiGraphics::clipRect( const MRect& rectToClip )
{
	ASSERT( ivHdcc );
	if( ExcludeClipRect(
			ivHdcc, 
			rectToClip.getX(),
			rectToClip.getY(),
			rectToClip.getRight(),
			rectToClip.getBottom() ) == ERROR )
	{
		MLogger::logError(
			"MUiGraphics::clipRect: ExcludeClipRect failed" );
	}
}

/**
 * sets the clipping rectangle
 */
void MUiGraphics::setClippingRect( const MRect &clippingRect )
{
	//TRACE( "setClippingRect( %s )\n", clippingRect.toString().getData() );
	HRGN region =
		CreateRectRgn(
			clippingRect.getX(),   // x-coordinate of upper-left corner
			clippingRect.getY(),    // y-coordinate of upper-left corner
			clippingRect.getRight(),  // x-coordinate of lower-right corner
			clippingRect.getBottom()  // y-coordinate of lower-right corner
		);
	SelectClipRgn( ivHdcc, region );
	DeleteObject( region );
}