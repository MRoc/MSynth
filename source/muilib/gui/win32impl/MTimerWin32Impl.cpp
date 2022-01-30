#include "MTimerWin32Impl.h"
#include <framework/io/MLogger.h>

/** the timer map */
Handle2TimerMap MTimerWin32Impl::gvTimerMap;

/** constructor */
MTimerWin32Impl::MTimerWin32Impl() :
	ivWin32TimerHandle( 0 ),
	ivTimerCycles( 0 )
{
}

/** destructor */
MTimerWin32Impl::~MTimerWin32Impl()
{
	if( ivWin32TimerHandle != 0 )
		stopTimer();
}

/** starts the timer */
void MTimerWin32Impl::startTimer( int millis )
{
	if( ivWin32TimerHandle )
	{
		MLogger::logError( "MTimerWin32Impl::startTimer: timer already running" );
		ASSERT( FALSE );
	}

	ivWin32TimerHandle = SetTimer( 0, 0, millis, MTimerWin32Impl::TimerProc );
	ivTimerCycles = 0;

	if( ivWin32TimerHandle == 0 )
	{
		MLogger::logError( "MTimerWin32Impl::startTimer: SetTimer failed" );
		ASSERT( FALSE );
	}
	else
	{
		gvTimerMap[ ivWin32TimerHandle ] = this;
	}
}

/** stops the timer */
void MTimerWin32Impl::stopTimer()
{
	if( ivWin32TimerHandle == 0 )
	{
		MLogger::logError( "MTimerWin32Impl::stopTimer: timer not running" );
		ASSERT( FALSE );
	}

	BOOL result = KillTimer( 0, ivWin32TimerHandle );

	if( result != TRUE )
	{
		MLogger::logError( "MTimerWin32Impl::stopTimer: KillTimer failed" );
		ASSERT( FALSE );
	}
	else
	{
		gvTimerMap.erase( gvTimerMap.find( ivWin32TimerHandle ) );
		ivWin32TimerHandle = 0;
	}
}

/** returns true if timer is running */
bool MTimerWin32Impl::isRunnning()
{
	return ivWin32TimerHandle != 0;
}

/** adds a timer listener */
void MTimerWin32Impl::addListener( IActionListener* pListener )
{
	ivListeners.addListener( pListener );
}

/** removes a timer listener */
void MTimerWin32Impl::removeListener( IActionListener* pListener )
{
	ivListeners.removeListener( pListener );
}

/** the timer callback */
void MTimerWin32Impl::onTimer()
{
	ivListeners.fireActionPerformed( (void*)this );
}

/** the static win32 timer callback */
VOID CALLBACK MTimerWin32Impl::TimerProc(
	HWND hwnd,         // handle to window
	UINT uMsg,         // WM_TIMER message
	UINT_PTR idEvent,  // timer identifier
	DWORD dwTime       // current system time
	)
{
	Handle2TimerMapIter i = gvTimerMap.find( idEvent );
	if( i == gvTimerMap.end()  )
	{
		MLogger::logError(
			"MTimerWin32Impl::TimerProc: timer map lookup failed" );
		ASSERT( FALSE );
	}
	else
	{
		i->second->ivTimerCycles++;
		i->second->onTimer();
	}
}

/** returns the number of cycles the timer was called back since last start */
unsigned int MTimerWin32Impl::getTimerCycles() const
{
	return ivTimerCycles;
}