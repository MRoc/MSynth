#include "MGUIUtils.h"
#include "MUiGraphics.h"
#include <framework/io/MLogger.h>
#include "win32impl/MFontWin32Impl.h"

MColor MGuiUtils::multiplyColor( MColor color, FP value )
{
	return
		( ( (DWORD) ((color&0xFF)*value) ) & 0xFF ) |
		( ( (DWORD) ((color&0xFF00)*value) ) & 0xFF00 ) |
		( ( (DWORD) ((color&0xFF0000)*value) ) & 0xFF0000 );
};

void MGuiUtils::paintRaisedBorder( IGraphics *g, MRect rect, MColor brightColor, MColor bgColor, MColor darkColor, MColor blackColor )
{
	g->fillRect( rect, bgColor );

	g->drawLine( rect.getX(), rect.getBottom()-1, rect.getX(), rect.getY(), brightColor );
	g->drawLine( rect.getX(), rect.getY(), rect.getRight(), rect.getY(), brightColor );
	g->drawLine( rect.getX()+1, rect.getBottom()-2, rect.getX()+1, rect.getY()+1, brightColor );
	g->drawLine( rect.getX()+1, rect.getY()+1, rect.getRight() - 1, rect.getY()+1, brightColor );

	g->drawLine( rect.getRight()-1, rect.getY()+1, rect.getRight()-1, rect.getBottom() - 1, darkColor );
	g->drawLine( rect.getX()-1, rect.getBottom() - 1, rect.getX(), rect.getBottom() - 1, darkColor );

	g->drawLine( rect.getX(), rect.getBottom(), rect.getRight(), rect.getBottom(), blackColor );
	g->drawLine( rect.getRight(), rect.getBottom(), rect.getRight(), rect.getY()-1, blackColor );
};

void MGuiUtils::paintSunkenBorder( IGraphics *g, MRect rect, MColor brightColor, MColor bgColor, MColor darkColor, MColor blackColor )
{
	g->fillRect( rect, bgColor );

	g->drawLine( rect.getX(), rect.getBottom()-1, rect.getX(), rect.getY(), blackColor );
	g->drawLine( rect.getX(), rect.getY(), rect.getRight(), rect.getY(), blackColor );

	g->drawLine( rect.getX()+1, rect.getBottom() - 1, rect.getX()+1, rect.getY() + 1, darkColor );
	g->drawLine( rect.getX()+1, rect.getY() + 1, rect.getRight() - 1, rect.getY() + 1, darkColor );

	g->drawLine( rect.getRight() - 1, rect.getY() + 1, rect.getRight() - 1, rect.getBottom() - 1, brightColor );
	g->drawLine( rect.getRight() - 1, rect.getBottom() - 1, rect.getX(), rect.getBottom() - 1, brightColor );

	g->drawLine( rect.getX(), rect.getBottom(), rect.getRight(), rect.getBottom(), brightColor );
	g->drawLine( rect.getRight(), rect.getBottom(), rect.getRight() , rect.getY()-1, brightColor );

	//g->SelectObject( oldBrush );
}

void MGuiUtils::paintRaisedBorder( IGraphics *g, MRect rect, MColor bgColor )
{
	MGuiUtils::paintRaisedBorder(
		g,
		rect,
		MGuiUtils::multiplyColor( bgColor, 1.1f ),
		bgColor,
		MGuiUtils::multiplyColor( bgColor, 0.9f ),
		MGuiUtils::multiplyColor( bgColor, 0.5 ) );
}

void MGuiUtils::paintSunkenBorder( IGraphics *g, MRect rect, MColor bgColor )
{
	MGuiUtils::paintSunkenBorder(
		g,
		rect,
		MGuiUtils::multiplyColor( bgColor, 1.1f ),
		bgColor,
		MGuiUtils::multiplyColor( bgColor, 0.9f ),
		MGuiUtils::multiplyColor( bgColor, 0.5 ) );
}

/*
*	This method draws a grid in the visible given rect of a CDC
*
*	CDC - the CDC with scrolled viewport at position x/y
*	rect - the visible rect
*	point - the upper left corner of the rect
*	rows - the number of rows in the grid
*	cols - the number of cols in the grid
*/
void MGuiUtils::paintGrid(
	IGraphics* g,
	MRect rect,
	MPoint point,
	MSize partSize,
	unsigned int rows,
	unsigned int cols,
	MColor color )
{
	//TRACE( "MGuiUtils::paintGrid: %s\n", rect.toString().getData() );
	if( partSize.getWidth() == 0 ||
		partSize.getHeight() == 0 )
		return;
	// calculate offsets, the offset is the amount of pixels not visible of the cutted partsize
	int offsetX = point.getX() % partSize.getWidth();
	int offsetY = point.getY() % partSize.getHeight();
	// the target points begins painting at the first not visible line
	int x = point.getX() - offsetX;
	int y = point.getY() - offsetY;
	// calculate the number of parts maximal viewable on the screen (worst case +2)
	unsigned int countX = rect.getWidth() / partSize.getWidth() + 2;
	unsigned int countY = rect.getHeight() / partSize.getHeight() + 2;

	// but not more than the defined max size
	if( countY > rows )
		countY = rows;
	if( countX > cols )
		countX = cols;

	// Draw vertical lines
	int iterativeX = x;
	int y1 = y;
	int y2 = y1 + countY * partSize.getHeight();
	int dX = partSize.getWidth();
	for( unsigned int i=0;i<=countX;i++ )
	{
		g->drawLine( iterativeX, y1, iterativeX, y2, color );
		iterativeX += dX;
	}

	// Draw horizontal lines
	int x1 = x;
	int x2 = x + countX * partSize.getWidth();
	int iterativeY = y;
	int dY = partSize.getHeight();
	for( i=0;i<=countY;i++)
	{
		g->drawLine( x1, iterativeY, x2, iterativeY, color );
		iterativeY += dY;
	}
}

void MGuiUtils::clipWindowRect( MRect& rectToClip )
{
	int screenWidth = GetSystemMetrics( SM_CXSCREEN );
	int screenHeight = GetSystemMetrics( SM_CYSCREEN );

	int width = rectToClip.getWidth();
	if( width > screenWidth )
		width = screenWidth;

	int height = rectToClip.getHeight();
	if( height > screenHeight )
		height = screenHeight;

	int x = rectToClip.getX();
	int y = rectToClip.getY();
	
	if( x + width > screenWidth )
	{
		x = screenWidth - width;
		if( x < 0 )
			x = 0;
	}
	    
	if( y + height > screenHeight )
	{
		y = screenHeight - height;
		if( y < 0 )
			y = 0;
	}

	rectToClip.setX( x );
	rectToClip.setWidth( width );
	rectToClip.setY( y );
	rectToClip.setHeight( height );
}