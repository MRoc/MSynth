#ifndef __MUiGraphics
#define __MUiGraphics

#include <framework/MTypes.h>
#include <framework/MNativeTypes.h>
#include "../gui/IGraphics.h"
#include "../gui/MFont.h"

/** wrapper arround the HDC */
class MUiGraphics :
	public MObject,
	public IGraphics
{
public:

	/** the runtime type info */
	static MRtti gvRtti;

protected:

	/** the internal hdc */
	HDC ivHdcc;

	/** the original hdc used for all clippings */
	HDC ivSrcHdc;

	/** the offset used for all draw methods */
	MPoint ivPoint;

public:

	/** constructor */
	MUiGraphics();

	/** constructor */
	MUiGraphics( const MUiGraphics& ui );

	/** constructor */
	MUiGraphics( HDC pHdc );

	/** constructor */
	MUiGraphics( HDC pHdc1, HDC pHdc2 );

	/** destructor */
	virtual ~MUiGraphics();

	/** returns the runtime type information */
	virtual IRtti* getRtti() const;

	/** querries for a specified interface */
	virtual void* getInterface( const String &className ) const;

	/** sets the offset used for all drawing operations */
	virtual void setOffset( const MPoint& p );

	/** sets the x offset */
	virtual void setOffsetX( int x );

	/** returns the offset x */
	virtual int getOffsetX();

	/** sets the y offset */
	virtual void setOffsetY( int y );

	/** returns the y offset */
	virtual int getOffsetY();

	/** returns the offset used for all drawing operations */
	virtual MPoint getOffset();

	/** draws a line */
	virtual void drawLine( int x1, int y1, int x2, int y2, MColor color );

	/** draws a line rectangle */
	virtual void drawRect( int x1, int y1, int x2, int y2, MColor color );

	/** draws a line rectangle */
	virtual void drawRect( const MRect& rect, MColor color );

	/** draws a solid rectangle */
	virtual void fillRect( int x1, int y1, int x2, int y2, MColor color );

	/** draws a solid rectangle */
	virtual void fillRect( const MRect& rect, MColor color );

	/** draws a filled ellipse */
	virtual void fillEllipse( const MRect& rect, MColor color );

	/** draws a poly */
	virtual void poly( MPoint** ptPoints, unsigned int pCount, MColor color, MColor lineColor );

	/** draws a poly */
	virtual void poly( MPoint* ptPoints, unsigned int pCount, MColor color, MColor lineColor );

	/** draws a centered text */
	virtual void drawCenteredText( const String& text, const MRect& ptRect, MColor color, MFont* ptFont );

	/** draws a centered text */
	virtual void drawCenteredText( const String& text, const MRect& rect, MColor color );

	/** draws a text */
	virtual void drawText( const String& text, const MRect& rect, MColor color, MFont* ptFont );

	/** draws a text */
	virtual void drawText( const String& text, const MRect& ptRect, MColor color );

	/** returns the size a text woule require when rendered */
	virtual MSize getTextSize( const String& text, MFont* ptFont );

	/** draws a image */
	virtual void drawImage( MImage* ptImage, int x, int y );

	/** draws a image */
	virtual void drawImageTransparent( MImage* ptImage, int x, int y, MColor transparentColor );

	/** clips the given rect from beeing redrawn */
	virtual void clipRect( const MRect& rectToClip );

	/** sets the clipping rectangle */
	virtual void setClippingRect( const MRect& clippingRect );

protected:

	void drawTransparent( int x, int y, HDC hdc, HBITMAP ptBitmap, MColor color );
};

#endif