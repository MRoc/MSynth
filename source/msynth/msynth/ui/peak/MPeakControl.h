#ifndef __MPeakControl
#define __MPeakControl

#include <framework/thread/IRunnable.h>
#include <gui/MWnd.h>
#include "../../model/util/MEnvelopeFollower.h"

/**
 * control for the envelope follower
 */
class MPeakControl :
	public MWnd
{
protected:

	/** 
	 * forward definition
	 */
	class MEnvelopeFollowerListener;

	/**
	 * the listener handling event input
	 */
	MEnvelopeFollowerListener* ivPtListener;

	/**
	 * reference to the envelope follower
	 */
	IEnvelopeFollower* ivPtEnvelopeFollower;

	/** 
	 * the current envelpoe value (0.0-1.0>)
	 */
	FP ivCurrentEnvelope;

public:

	/**
	 * constructor
	 */
	MPeakControl();

	/**
	 * destructor
	 */
	virtual ~MPeakControl();

	/**
	 * sets the envelope follower
	 */
	virtual void setEnvelopeFollower( IEnvelopeFollower* pFollower );

	/**
	 * paints the window
	 */
	virtual void paintWnd( IGraphics* ptGraphics, MRect& rect );

protected:

	friend class MEnvelopeFollowerListener;
	/**
	 * the listener handling incoming events
	 * from the MEnvelopeFollower
	 */
	class MEnvelopeFollowerListener :
		public MObject,
		public IEnvelopeFollowerListener,
		public IRunnable
	{
	protected:

		/**
		 * runtime type info
		 */
		static MRtti gvRtti;

		/**
		 * reference to the peak control
		 */
		MPeakControl* ivPtPeakControl;

	public:

		/**
		 * constructor
		 */
		MEnvelopeFollowerListener( MPeakControl* pPeakControl );

		/**
		 * destructor
		 */
		virtual ~MEnvelopeFollowerListener();
	
		/** 
		 * invoked when envelope changed
		 * (inheritanced from IEnvelopeFollowerListener)
		 */
		virtual void onEnvelopeChanged( FP curEnvelope );

		/** 
		 * does the repaint
		 * (inheritanced from IRunnable)
		 */
		virtual void run();

		/**
		 * returns the runtime type info
		 */
		virtual IRtti* getRtti() const;

		/**
		 * query interface
		 */
		virtual void* getInterface( const String &className ) const;
	};
};

#endif