#ifndef __MPlayPositionLabel
#define __MPlayPositionLabel

#include <framework/MUtil.h>

#include "../../model/MUpdateThread.h"
#include <gui/MWndCollection.h>
#include <gui/label/MLabel.h>
#include <gui/counter/MCounter.h>
#include <framework/thread/IRunnable.h>

class MPlayPositionLabel :
	public MWndCollection
{
protected:

	class MPlayPositionListener;

	MUpdateThread* ivPtUpdateThread;

	MLabel* ivPtLabel;
	MCounter* ivPtCounterPattern;
	MCounter* ivPtCounterStep;

	MPlayPositionListener* ivPtPlayPositionListener;

public:

	MPlayPositionLabel( MColor bkColor, MColor fontColor, const String& text );
	virtual ~MPlayPositionLabel();

	virtual void doLayout();
	virtual void setUpdateThread( MUpdateThread* ptThread );
	virtual void setFgColor( MColor color );
	virtual void setText( const String& text );
	virtual void paint( IGraphics* pG, const MRect &rect );

protected:

	friend class MPlayPositionListener;
	/**
	 * hook for receiving play position changes
	 */
	class MPlayPositionListener :
		public MObject,
		public IPlayPositionListener,
		public IRunnable
	{
	protected:

		int ivPattern, ivStep;

		/**
		 * reference to the view
		 */
		MPlayPositionLabel* ivPtView;

		/**
		 * reference to the model
		 */
		MUpdateThread* ivPtModel;

	public:
		FULL_RTTI( MPlayPositionLabel::MPlayPositionListener, "MPlayPositionLabel::MPlayPositionListener" );
		/**
		 * constuctor
		 */
		MPlayPositionListener( MPlayPositionLabel* ptView, MUpdateThread* ptModel );

		/**
		 * destructor
		 */
		virtual ~MPlayPositionListener();

		/**
		 * invoked from the update thread when play position changed
		 */
		virtual void playPositionChanged( int pattern, int step );

		virtual void run();
	};
};

#endif
