/*
  MBeatBoxMessageInspector (C)2002 MRoc hifiShock
*/

#ifndef __MBeatBoxMessageInspector
#define __MBeatBoxMessageInspector

#include <gui/MWnd.h>
#include "../../control/inspector/IBarInspector.h"
#include "../../../model/note/MDefaultMessageNoteList.h"

class MBeatBoxMessageInspector : public IBarInspector
{
public:

	static int					VOLUME_MODE;

protected:

	unsigned int ivEditIndex;
	MSize ivSize;
	MSize* ivPtPartSize;
	int ivOffsetX;

	MBar* ivPtBar;

	MWnd* ivPtContainer;

public:

	MBeatBoxMessageInspector();
	virtual ~MBeatBoxMessageInspector();

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

	virtual void setOffsetX( int x );
	virtual int getOffsetX();

	virtual void paint( IGraphics* ptGraphics, const MRect& rect );

	virtual bool onMouseDown( MMouseEvent* pEvent );
	virtual bool onMouseUp( MMouseEvent* pEvent );
	virtual bool onMouseMoved( MMouseEvent* pEvent );

	virtual bool getForwardEvents();

	virtual void release();

	void setEditIndex( unsigned int index );
	unsigned int MBeatBoxMessageInspector::getEditIndex();

protected:

	MBeatBoxMessage* getMessage( MPoint p );
	MRect getBar( int index, FP value );
};

#endif