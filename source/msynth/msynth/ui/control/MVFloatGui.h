/*

	(C)2000-2002 MRoc hifiShock


	Displays a slider control out of 2 bitmaps
    
	|---------------|
	|  |---------|  |
    |  |         |  |
	|  | bitmap2 |  |
	|  |         |  |
	|  |---------|  |
	| bitmap1       |
	|---------------|
*/

#ifndef __MVFloatGui
#define __MVFloatGui

#include "MControlGui.h"
#include "../../model/MTimecontroller.h"
#include "../../model/control/MFloatControl.h"
#include <framework/thread/IRunnable.h>

class MVFloatGui : public MControlGui
{
protected:

	class MControlListener;

	MControlListener* ivPtListener;

	MPoint		ivLastPoint;
	bool		ivDrawTransparent;

	int			ivRange,
				ivValue;

	MImage*		ivPtBkBitmap;
	MImage*		ivPtKnobBitmap;

public:

	MVFloatGui( MFloatControl* ptFloatControl, MColor bkColor );
	MVFloatGui( unsigned int bmBkId, unsigned int bmKnobId, MFloatControl* ptFloatControl, MColor bkColor );

	virtual ~MVFloatGui();

	virtual void	paint( IGraphics* ptGraphics, const MRect &rect );

	virtual void	onMouseDown( MMouseEvent* ptEvent );
	virtual void	onMouseMove( MMouseEvent* ptEvent );
	virtual void	onMouseDoubleClick( MMouseEvent* ptEvent );

	virtual bool	onSetCursor();

protected:

	void			setControlValue( FP value );

	virtual String	getToolTipText();

	int				Round( float a );

	friend class MControlListener;
	class MControlListener :
		public MObject,
		public IControlListener,
		public IRunnable
	{
	protected:
		MVFloatGui* ivPtGui;
	public:
		FULL_RTTI( MVFloatGui::MControlListener, "MVFloatGui::MControlListener" );
		MControlListener( MVFloatGui* ptGui );
		virtual ~MControlListener();
		virtual void valueChanged( MControlListenerEvent *anEvent );
		virtual void run();
	};
};

#endif