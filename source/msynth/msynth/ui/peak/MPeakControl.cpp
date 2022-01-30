#include "MPeakControl.h"
#include <gui/MApp.h>

/**
 * constructor
 */
MPeakControl::MPeakControl() :
	ivPtEnvelopeFollower( 0 ),
	ivPtListener( 0 )
{
}

/**
 * destructor
 */
MPeakControl::~MPeakControl()
{
	if( ivPtEnvelopeFollower )
		setEnvelopeFollower( 0 );
}

/**
 * sets the envelope follower
 */
void MPeakControl::setEnvelopeFollower( IEnvelopeFollower* pFollower )
{
	if( ivPtListener )
		delete ivPtListener;

	ivPtEnvelopeFollower = pFollower;

	if( ivPtEnvelopeFollower )
		ivPtListener = new MEnvelopeFollowerListener( this );
}

/**
 * paints the window
 */
void MPeakControl::paintWnd( IGraphics* ptGraphics, MRect& rect )
{
	ptGraphics->fillRect(
		rect,
		MColorMap::BLACK );

	ptGraphics->fillRect(
		MRect(
			0,
			(int)(getHeight()*(1.0f-ivCurrentEnvelope)),
			getWidth(),
			(int)(getHeight() * ivCurrentEnvelope) ),
			MColorMap::GREEN );
}

/**
 * the runtime type info
 */
MRtti MPeakControl::MEnvelopeFollowerListener::gvRtti = MRtti( "MEnvelopeFollowerListener", 0 );

/**
 * constructor
 */
MPeakControl::MEnvelopeFollowerListener::MEnvelopeFollowerListener( MPeakControl* pPeakControl ) :
	ivPtPeakControl( pPeakControl )
{
	ivPtPeakControl->ivPtEnvelopeFollower->addListener( this );
}

/**
 * destructor
 */
MPeakControl::MEnvelopeFollowerListener::~MEnvelopeFollowerListener()
{
	ivPtPeakControl->ivPtEnvelopeFollower->removeListener( this );
}

/** 
 * invoked when envelope changed
 * (inheritanced from IEnvelopeFollowerListener)
 */
void MPeakControl::MEnvelopeFollowerListener::onEnvelopeChanged( FP curEnvelope )
{
	ivPtPeakControl->ivCurrentEnvelope = curEnvelope;
	MApp::getInstance()->doWorkInGuiThread( this );
}

/** 
 * does the repaint
 * (inheritanced from IRunnable)
 */
void MPeakControl::MEnvelopeFollowerListener::run()
{
	ivPtPeakControl->repaint();
}

/**
* returns the runtime type info
*/
IRtti* MPeakControl::MEnvelopeFollowerListener::getRtti() const
{
	return &MPeakControl::MEnvelopeFollowerListener::gvRtti;
}

/**
* query interface
*/
void* MPeakControl::MEnvelopeFollowerListener::getInterface( const String &className ) const
{
	if( className == "MEnvelopeFollowerListener" )
		return (void*) ((MEnvelopeFollowerListener*)this);
	else
		return MObject::getInterface( className );
}