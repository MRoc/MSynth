#include "MTimer.h"
#include "../win32impl/MTimerWin32Impl.h"

/** constructor */
MTimer::MTimer() :
	ivImpl( new MTimerWin32Impl() )
{
}

/** destructor */
MTimer::~MTimer()
{
	if( isRunnning() )
		stopTimer();
	SAFE_DELETE( ivImpl );
}

/** starts the timer */
void MTimer::startTimer( int millis ){ ivImpl->startTimer( millis ); }

/** stops the timer */
void MTimer::stopTimer(){ ivImpl->stopTimer(); }

/** returns true if timer is running */
bool MTimer::isRunnning(){ return ivImpl->isRunnning(); }

/** adds a timer listener */
void MTimer::addListener( IActionListener* pListener ){ ivImpl->addListener( pListener ); }

/** removes a timer listener */
void MTimer::removeListener( IActionListener* pListener ) { ivImpl->removeListener( pListener ); }

/** returns the number of cycles the timer was called back since last start */
unsigned int MTimer::getTimerCycles(){ return ivImpl->getTimerCycles(); }

/** returns the internal impl */
MTimerImpl* MTimer::getImpl(){ return ivImpl; }