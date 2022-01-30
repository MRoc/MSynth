#ifndef __MGuiUtils
#define __MGuiUtils

#include "MTopWnd.h"
#include "MFont.h"
#include "MImage.h"

class MGuiUtils
{
public:

	static MColor multiplyColor( MColor color, FP value );

	static void paintRaisedBorder(
		IGraphics* pG, MRect rect, MColor brightColor, MColor bgColor, MColor darkColor, MColor blackColor );

	static void paintSunkenBorder(
		IGraphics* pG, MRect rect, MColor brightColor, MColor bgColor, MColor darkColor, MColor blackColor );

	static void paintRaisedBorder(
		IGraphics* pG, MRect rect, MColor bgColor );

	static void paintSunkenBorder(
		IGraphics* pG, MRect rect, MColor bgColor );

	static void paintGrid(
		IGraphics* g, MRect rect, MPoint point, MSize partSize, unsigned int rows, unsigned int cols, MColor color );

	static void clipWindowRect( MRect& rect );
};

#endif