#ifndef __MCOLORMAP
#define __MCOLORMAP

#include <framework/MTypes.h>

class MColorMap
{
public:

	static MColor create( unsigned char r, unsigned char g, unsigned char b );

	static MColor BLACK;
	static MColor WHITE;
	static MColor BLUE;
	static MColor GRAY;
	static MColor RED;
	static MColor YELLOW;
	static MColor GREEN;

	static MColor BK_COLOR1;
	static MColor BK_COLOR2;
	static MColor BK_COLOR3;
	static MColor BK_COLOR4;
	static MColor BK_COLOR5;

	static MColor FG_COLOR1;
	static MColor FG_COLOR2;
	static MColor FG_COLOR3;
	static MColor FG_COLOR4;
	static MColor FG_COLOR5;
};

#endif