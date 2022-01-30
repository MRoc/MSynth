/*

(C)2000-2002 MRoc hifiShock


Displays centered bitmap and centered text

|---------------|
|  /---------\  |
| /           \ |
| |  Bitmap   | |
| \           / |
|  \---------/  |
| TEXT_TEXT_TEX |
|---------------|
*/

#ifndef __MFloatGui
#define __MFloatGui

#include <framework/MTypes.h>
#include <gui/MWnd.h>
#include <gui/tooltip/MToolTipCtrl.h>
#include <framework/thread/IRunnable.h>
#include "MControlGui.h"
#include "../../model/MTimeController.h"
#include "../../model/control/MFloatControl.h"
#include "../../../resource.h"

class MFloatGui :
	public MControlGui,
	public IRunnable
{
protected:

	MPoint				ivLastPoint;
	bool				ivDrawTransparent;

	MSize				ivTextExtend;
	MImage*				ivBitmapBackGround;
	MImage*				ivBitmapBackGroundDisabled;
	MPoint				ivCircleCenter;
	short				ivRange,
						ivValue,
						ivInnerRadius,
						ivOuterRadius,
						ivBitmapOffset,
						ivTextOffset;

public:

	MFloatGui( MFloatControl* ptFloatControl, MColor bkColor );
	MFloatGui( unsigned int bmId, unsigned int bmIdDisabled, MFloatControl* ptFloatControl, MColor bkColor );

	virtual ~MFloatGui();

	virtual void		paint( IGraphics* ptGraphics, const MRect &rect );

	virtual void		onMouseDown( MMouseEvent* ptEvent );
	virtual void		onMouseDoubleClick( MMouseEvent* ptEvent );
	virtual void		onMouseMove( MMouseEvent* ptEvent );
	virtual bool		onSetCursor();

	FULL_RTTI( MFloatGui, "MFloatGui" );

protected:

	virtual void		setControlValue( FP value );

	virtual String		getToolTipText();

	int					Round( float a );

	void inline			DrawText( int x, int y, IGraphics* ptGraphics );
	void inline			DrawBitmap( int x, int y, IGraphics* ptGraphics, MImage* ptBitmap );
	void inline			DrawPointer( int x, int y, IGraphics* ptGraphics );

	virtual void		valueChanged( MControlListenerEvent *anEvent );
	virtual String		getClassName();

	virtual void		run();
};

#endif