#ifndef __MArrowRenderer
#define __MArrowRenderer

#include "../IGraphics.h"

class MArrowRenderer
{
public:

	/** the arrow direction */
	enum Direction
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	/** the UP flag as string */
	static const char* DirectionStr[];

	/** paints an arrow */
	static void paintArrow( IGraphics* ptDc, MRect r, MPoint offset, Direction direction );

	/** returns the direction as string */
	static const char* toString( MArrowRenderer::Direction direction );

	/** returns the direction parsed from the string */
	static MArrowRenderer::Direction parse( const char* text );

private:

	/** paints a upper arrow */
	static void paintUpperArrow( IGraphics* ptDc, MRect r, MPoint offset);

	/** paints a down arrow */
	static void paintDownArrow( IGraphics* ptDc, MRect r, MPoint offset );

	/** paints a right arrow */
	static void paintRightArrow( IGraphics* ptDc, MRect r, MPoint offset );

	/** paints a left arrow */
	static void paintLeftArrow( IGraphics* ptDc, MRect r, MPoint offset );
};

#endif