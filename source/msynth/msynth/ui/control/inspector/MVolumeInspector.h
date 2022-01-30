#ifndef __MVOLUMEINSPECTOR
#define __MVOLUMEINSPECTOR

#include "IBarInspector.h"
#include <gui/MWnd.h>

class MVolumeInspector : public IBarInspector
{
private:

	MSize ivSize;
	MSize* ivPtPartSize;

	MBar* ivPtBar;

	MWnd* ivPtContainer;

public:

	MVolumeInspector();
	virtual ~MVolumeInspector();

	virtual String getName();

	virtual void setContainingWnd( MWnd* ptContainer );
	virtual MWnd* getContainingWnd();

	virtual void setBar( MBar* ptBar );
	virtual MBar* getBar();

	virtual ToolTipInfo getToolTip( MPoint point );

	virtual void setSize( MSize dimension );
	virtual MSize getSize();

	virtual void setPartSize( MSize* ptSize );
	virtual MSize* getPartSize();

	virtual void paint( IGraphics* ptGraphics, const MRect& rect );

	virtual bool onMouseDown( MMouseEvent* pEvent );
	virtual bool onMouseUp( MMouseEvent* pEvent );
	virtual bool onMouseMoved( MMouseEvent* pEvent );

	virtual bool getForwardEvents();

	virtual void release();

	virtual void setInspectorInformation( MObject* ptObject );

private:

	virtual MDefaultSynthNote* getNote( MPoint point );
	MRect getBar( int index, FP value );
};

#endif