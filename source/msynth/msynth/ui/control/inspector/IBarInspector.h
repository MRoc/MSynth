/*

  IBarInspector (C)2002 MRoc hifiShock

*/

#ifndef __IBARINSPECTOR
#define __IBARINSPECTOR

#include <framework/MObject.h>
#include <framework/primitive/MSize.h>
#include <gui/event/MMouseEvent.h>
#include <gui/MWnd.h>

#include "../../../model/MBar.h"

struct ToolTipInfo
{
	bool show;
	MPoint toolTipPoint;
	String toolTipText;
};

class IBarInspector
{
public:

	virtual String getName() = 0;

	virtual void setContainingWnd( MWnd* ptContainer ) = 0;
	virtual MWnd* getContainingWnd() = 0;

	virtual void setBar( MBar* ptBar ) = 0;
	virtual MBar* getBar() = 0;

	virtual ToolTipInfo getToolTip( MPoint point ) = 0;

	virtual void setSize( MSize dimension ) = 0;
	virtual MSize getSize() = 0;

	virtual void setPartSize( MSize* ptSize ) = 0;
	virtual MSize* getPartSize() = 0;

	virtual void paint( IGraphics* ptGraphics, const MRect& rect ) = 0;

	virtual bool onMouseDown( MMouseEvent* pEvent ) = 0;
	virtual bool onMouseUp( MMouseEvent* pEvent ) = 0;
	virtual bool onMouseMoved( MMouseEvent* pEvent ) = 0;

	virtual bool getForwardEvents() = 0;

	virtual void release() = 0;
};

#endif