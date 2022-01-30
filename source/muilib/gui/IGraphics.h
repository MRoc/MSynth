#ifndef __IGraphics
#define __IGraphics

#include <framework/primitive/MRect.h>
#include "MImage.h"
#include "MFont.h"
#include "MColorMap.h"

/** definition of a graphics object */
class IGraphics :
	public IMUnknown
{
public:

	/** draws a line */
	virtual void drawLine( int x1, int y1, int x2, int y2, MColor color ) = 0;

	/** draws a line rectangle */
	virtual void drawRect( int x1, int y1, int x2, int y2, MColor color ) = 0;

	/** draws a line rectangle */
	virtual void drawRect( const MRect& rect, MColor color ) = 0;

	/** draws a solid rectangle */
	virtual void fillRect( int x1, int y1, int x2, int y2, MColor color ) = 0;

	/** draws a solid rectangle */
	virtual void fillRect( const MRect& rect, MColor color ) = 0;

	/** draws a filled ellipse */
	virtual void fillEllipse( const MRect& rect, MColor color ) = 0;

	/** draws a poly */
	virtual void poly( MPoint** ptPoints, unsigned int pCount, MColor color, MColor lineColor ) = 0;

	/** draws a poly */
	virtual void poly( MPoint* ptPoints, unsigned int pCount, MColor color, MColor lineColor ) = 0;

	/** draws a centered text */
	virtual void drawCenteredText( const String& text, const MRect& rect, MColor color, MFont* ptFont ) = 0;

	/** draws a centered text */
	virtual void drawCenteredText( const String& text, const MRect& rect, MColor color ) = 0;

	/** draws a text */
	virtual void drawText( const String& text, const MRect& rect, MColor color, MFont* ptFont ) = 0;

	/** draws a text */
	virtual void drawText( const String& text, const MRect& rect, MColor color ) = 0;

	/** returns the size a text woule require when rendered */
	virtual MSize getTextSize( const String& text, MFont* ptFont ) = 0;

	/** draws a image */
	virtual void drawImage( MImage* ptImage, int x, int y ) = 0;

	/** draws a image */
	virtual void drawImageTransparent( MImage* ptImage, int x, int y, MColor transparentColor ) = 0;

	/** clips the given rect from beeing redrawn */
	virtual void clipRect( const MRect& rectToClip ) = 0;

	/** sets the offset used for all drawing operations */
	virtual void setOffset( const MPoint& p ) = 0;

	/** sets the x offset */
	virtual void setOffsetX( int x ) = 0;

	/** returns the offset x */
	virtual int getOffsetX() = 0;

	/** sets the y offset */
	virtual void setOffsetY( int y ) = 0;

	/** returns the y offset */
	virtual int getOffsetY() = 0;

	/** returns the offset used for all drawing operations */
	virtual MPoint getOffset() = 0;

	/** sets the clipping rectangle */
	virtual void setClippingRect( const MRect& clippingRect ) = 0;
};

#endif