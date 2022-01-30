#ifndef __MTOOLTIPWND
#define __MTOOLTIPWND

#include "../MTopWnd.h"
#include "../MFont.h"

class MToolTipWnd :
	public MTopWnd
{
protected:

	MColor	ivBkColor;
	MColor	ivFgColor;

public:

	MToolTipWnd();
	virtual ~MToolTipWnd();

	virtual bool create();
	virtual void paint( IGraphics* pG, const MRect &rect );

	virtual void show( String text, MPoint point );
	virtual void hide();
};

#endif