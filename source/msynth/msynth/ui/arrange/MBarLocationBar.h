/*

  MBarLocationBar (C)2001 MRoc hifiShock

  This class displays a scrollable locationbar to display the location
  of a bar.
  It has a right marker and a left marker to indicate the loop markers.

*/

#ifndef __MBarLocationBar
#define __MBarLocationBar

#include <framework/MUtil.h>
#include <framework/thread/IRunnable.h>

#include <gui/MyInteractiveWnd.h>
#include "../../model/MTimeController.h"
#include "../../model/MUpdateThread.h"

class MBarLocationBar :
	public MyInteractiveWnd
{
protected:
	class MMarkerListener;
	class MPlayPositionListener;
protected:

	bool			ivLButtonDown;
	bool			ivRButtonDown;
	
	int				ivCurrentPattern,
					ivCurrentStep;

	int				ivBarCount;
	int				ivBarWidth;

	MPoint			ivPolyLeft[3];
	MPoint			ivPolyRight[3];
	MPoint			ivPolyPlayPos[3];

	MTimeController*	ivPtTimeController;
	MUpdateThread* ivPtUpdateThread;

	MMarkerListener* ivPtMarkerListener;
	MPlayPositionListener* ivPtPlayPositionListener;

public:

	MBarLocationBar( int noteCount );
	virtual			~MBarLocationBar();

	virtual void	paint( IGraphics* ptGraphics, const MRect &rect );

	void			setTimeController( MTimeController* ptTimeController );
	MTimeController* getTimeController();

	void			setBarCount( int barCount );
	int				getBarCount();

	void			setBarWidth( int barWidth );
	int				getBarWidth();

	void			setUpdateThread( MUpdateThread* ptUpdateThread );

protected:

	virtual void	onMouseUp( MMouseEvent* anEvent );
	virtual void	onMouseDoubleClick( MMouseEvent* anEvent );

protected:

	int getPosition( int xInPixel );

protected:

	friend class MMarkerListener;
	class MMarkerListener :
		public MObject,
		public IMarkerListener
	{
	protected:
		MBarLocationBar* ivPtOwner;
	public:
		FULL_RTTI( MBarLocationBar::MMarkerListener, "MBarLocationBar::MMarkerListener" );
		MMarkerListener( MBarLocationBar* ptOwner );
		virtual ~MMarkerListener();
		virtual void markerChanged();
	};

	friend class MPlayPositionListener;
	class MPlayPositionListener :
		public MObject,
		public IPlayPositionListener,
		public IRunnable
	{
	protected:
		MBarLocationBar* ivPtView;
		MUpdateThread* ivPtModel;
	public:
		FULL_RTTI( MBarLocationBar::MPlayPositionListener, "MBarLocationBar::MPlayPositionListener" );
		MPlayPositionListener( MBarLocationBar* ptView, MUpdateThread* ptModel );
		virtual ~MPlayPositionListener();
		/**
		 * invoked from the update thread when play position changed
		 */
		virtual void playPositionChanged( int pattern, int step );
		virtual void run();
	};
};

#endif
