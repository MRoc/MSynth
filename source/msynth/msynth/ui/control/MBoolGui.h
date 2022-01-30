#ifndef __MBOOLGUI
#define __MBOOLGUI

#include "MControlGui.h"
#include "../../model/control/MBoolControl.h"

class MBoolGui : public MControlGui
{
protected:

	class MControlListener;

	MColor				ivBkColorSelected;
	MColor				ivBkColorBright;
	MColor				ivBkColorDark;
	MColor				ivBkColorDarkest;

	MControlListener*	ivPtListener;

public:

	MBoolGui( MBoolControl* ptControl, MColor bkColor );
	~MBoolGui();

	virtual void		paint( IGraphics* ptGraphics, const MRect &rect );
	virtual void		onMouseUp( MMouseEvent* ptEvent );

	virtual MSize		getPreferredSize();

	FULL_RTTI( MBoolGui, "MBoolGui" );

protected:

	bool				isPtInRect( MPoint point );

	virtual String		getToolTipText();

	friend class MControlListener;
	class MControlListener :
		public MObject,
		public IControlListener
	{
	protected:
		MBoolGui* ivPtGui;
	public:
		FULL_RTTI( MBoolGui::MControlListener, "MBoolGui::MControlListener" );
		MControlListener( MBoolGui* ptGui );
		virtual ~MControlListener();
		virtual void valueChanged( MControlListenerEvent *anEvent );
	};
};

#endif

