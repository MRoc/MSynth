#include "MArrowRenderer.h"

/** the UP flag as string */
const char* MArrowRenderer::DirectionStr[] = 
{
	"UP",
	"DOWN",
	"LEFT",
	"RIGHT",
	0
};

/** returns the direction as string */
const char* MArrowRenderer::toString( MArrowRenderer::Direction direction )
{
	return DirectionStr[ direction ];
}

/** returns the direction parsed from the string */
MArrowRenderer::Direction MArrowRenderer::parse( const char* text )
{
	int i = 0;
	const char** pt = DirectionStr;
	while( *pt )
	{
		if( strcmp( *pt, text ) == 0 )
			return (MArrowRenderer::Direction) i;
		pt++; i++;
	}
	return DOWN;
}

void MArrowRenderer::paintArrow( IGraphics* ptDc, MRect r, MPoint offset, Direction direction )
{
	switch( direction )
	{
		case UP:
			paintUpperArrow( ptDc, r, offset );
			break;
		case DOWN:
			paintDownArrow( ptDc, r, offset );
			break;
		case LEFT:
			paintLeftArrow( ptDc, r, offset );
			break;
		case RIGHT:
			paintRightArrow( ptDc, r, offset );
			break;
	}
}

void MArrowRenderer::paintUpperArrow( IGraphics* ptDc, MRect r, MPoint offset )
{
	int arrowWidth = 8;
	int arrowHeight = 5;
	int upperLeftX = (r.getWidth() - arrowWidth) / 2;
	int upperLeftY = (r.getHeight() - arrowHeight) / 2;
	int x = offset.getX() + r.getX() + upperLeftX;
	int y = offset.getY() + r.getY() + upperLeftY;
	MPoint points[ 3 ];
	points[0].setX( x + arrowWidth / 2 );
	points[0].setY( y );
	points[1].setX( x + arrowWidth );
	points[1].setY( y + arrowHeight );
	points[2].setX( x );
	points[2].setY( y + arrowHeight );
	ptDc->poly( points, 3, MColorMap::FG_COLOR2, MColorMap::FG_COLOR3 );
}

void MArrowRenderer::paintDownArrow( IGraphics* ptDc, MRect r, MPoint offset )
{
	int arrowWidth = 8;
	int arrowHeight = 5;
	int upperLeftX = (r.getWidth() - arrowWidth) / 2;
	int upperLeftY = (r.getHeight() - arrowHeight) / 2;
	int x = offset.getX() + r.getX() + upperLeftX;
	int y = offset.getY() + r.getY() + upperLeftY;
	MPoint points[ 3 ];
	points[0].setX( x );
	points[0].setY( y );
	points[1].setX( x + arrowWidth );
	points[1].setY( y );
	points[2].setX( x + arrowWidth / 2 );
	points[2].setY( y + arrowHeight );
	ptDc->poly( points, 3, MColorMap::FG_COLOR2, MColorMap::FG_COLOR3 );
}

void MArrowRenderer::paintRightArrow( IGraphics* ptDc, MRect r, MPoint offset )
{
	int arrowWidth = 5;
	int arrowHeight = 8;
	int upperLeftX = (r.getWidth() - arrowWidth) / 2;
	int upperLeftY = (r.getHeight() - arrowHeight) / 2;
	int x = offset.getX() + r.getX() + upperLeftX;
	int y = offset.getY() + r.getY() + upperLeftY;
	MPoint points[ 3 ];
	points[0].setX( x );
	points[0].setY( y );
	points[1].setX( x + arrowWidth );
	points[1].setY( y + arrowHeight / 2 );
	points[2].setX( x );
	points[2].setY( y + arrowHeight );
	ptDc->poly( points, 3, MColorMap::FG_COLOR2, MColorMap::FG_COLOR3 );
}

void MArrowRenderer::paintLeftArrow( IGraphics* ptDc, MRect r, MPoint offset )
{
	int arrowWidth = 5;
	int arrowHeight = 8;
	int upperLeftX = (r.getWidth() - arrowWidth) / 2;
	int upperLeftY = (r.getHeight() - arrowHeight) / 2;
	int x = offset.getX() + r.getX() + upperLeftX;
	int y = offset.getY() + r.getY() + upperLeftY;
	MPoint points[ 3 ];
	points[0].setX( x + arrowWidth );
	points[0].setY( y );
	points[1].setX( x + arrowWidth );
	points[1].setY( y + arrowHeight );
	points[2].setX( x );
	points[2].setY( y + arrowHeight / 2 );
	ptDc->poly( points, 3, MColorMap::FG_COLOR2, MColorMap::FG_COLOR3 );
}
