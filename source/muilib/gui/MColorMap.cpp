#include "MColorMap.h"

MColor MColorMap::create( unsigned char r, unsigned char g, unsigned char b )
{
	return r|((unsigned short)g<<8)| ((unsigned int)b<<16);
}

// deep blue
MColor MColorMap::BK_COLOR5 = MColorMap::create(80,80,110);
MColor MColorMap::BK_COLOR2 = MColorMap::create(120,120,150);
MColor MColorMap::BK_COLOR1 = MColorMap::create(170,170,190);
MColor MColorMap::BK_COLOR3 = MColorMap::create(180,180,200);
MColor MColorMap::BK_COLOR4 = MColorMap::create(198,198,230);
MColor MColorMap::FG_COLOR2 = MColorMap::create(240,240,255);
MColor MColorMap::FG_COLOR1 = MColorMap::create(250,250,250);
MColor MColorMap::FG_COLOR3 = MColorMap::create(50,50,62);

MColor MColorMap::BLACK = MColorMap::create(0,0,0);
MColor MColorMap::WHITE = MColorMap::create(255,255,255);
MColor MColorMap::BLUE = MColorMap::create(0,0,255);
MColor MColorMap::GRAY = MColorMap::create(128,128,128);
MColor MColorMap::RED = MColorMap::create( 255, 0, 0 );
MColor MColorMap::YELLOW = MColorMap::create( 0, 255, 0 );
MColor MColorMap::GREEN = MColorMap::create( 0, 0, 255 );

// insane
/*MColor MColorMap::BK_COLOR1 = RGB(170,170,255);
MColor MColorMap::BK_COLOR2 = RGB(120,255,120);
MColor MColorMap::BK_COLOR3 = RGB(255,180,180);
MColor MColorMap::BK_COLOR4 = RGB(200,0,0);
MColor MColorMap::BK_COLOR5 = RGB(200,100,50);
MColor MColorMap::FG_COLOR1 = RGB(0,255,0);
MColor MColorMap::FG_COLOR2 = RGB(0,0,255);
MColor MColorMap::FG_COLOR3 = RGB(255,0,0);*/

// piggy
/*MColor MColorMap::BK_COLOR1 = RGB(240,170,170);
MColor MColorMap::BK_COLOR2 = RGB(140,120,120);
MColor MColorMap::BK_COLOR3 = RGB(230,160,160);
MColor MColorMap::BK_COLOR4 = RGB(200,130,100);
MColor MColorMap::BK_COLOR5 = RGB(200,130,100);
MColor MColorMap::FG_COLOR1 = RGB(255,200,200);
MColor MColorMap::FG_COLOR2 = RGB(255,200,200);
MColor MColorMap::FG_COLOR3 = RGB(62,40,40);*/

// green
/*MColor MColorMap::BK_COLOR1 = RGB(160,220,160);
MColor MColorMap::BK_COLOR2 = RGB(120,140,120);
MColor MColorMap::BK_COLOR3 = RGB(180,200,180);
MColor MColorMap::BK_COLOR4 = RGB(150,198,150);
MColor MColorMap::BK_COLOR5 = RGB(80,110,80);
MColor MColorMap::FG_COLOR1 = RGB(200,250,200);
MColor MColorMap::FG_COLOR2 = RGB(200,250,200);
MColor MColorMap::FG_COLOR3 = RGB(40,62,40);*/