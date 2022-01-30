#include "MSyncThread.h"
#include <framework/io/MLogger.h>

/**
 * constructor
 */
MSyncThread::MSyncThread( IRunnable* pRunnable ) :
	MThread(),
	ivHandleEvent( 0 ),
	ivPtRunnable( pRunnable )
{
	ivHandleEvent =
		CreateEvent(
			NULL,
			FALSE,
			FALSE,
			NULL );
	if( ! ivHandleEvent )
		MLogger::logError(
			"MSyncThread::MSyncThread could not create event!" );
}


/**
 * destructor
 */
MSyncThread::~MSyncThread()
{
	if( ivHandleEvent )
	{
		if( CloseHandle( ivHandleEvent ) == FALSE )
			MLogger::logError(
				"MSyncThread::~MSyncThread: CloseHandle failed" );
		ivHandleEvent = 0;
	}
}

/**
 * waits for the event until resume
 * is invoked from another thread.
 */
void MSyncThread::run()
{
	ASSERT( ivHandleEvent );
	ASSERT( ivPtRunnable );

	while( ivIsRunning ) 
    { 
		DWORD res =
			MsgWaitForMultipleObjects(
				1,
				&ivHandleEvent,
				FALSE,
				INFINITE,
				QS_ALLEVENTS );

		switch( res )
		{
		case 0:
			{
				ivPtRunnable->run();
			}
			break;
		default:
			{
				MSG msg;
				while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
				{ 
					if( msg.message == WM_QUIT )
					{
						ivIsRunning = false;
						TRACE( "<msyncthread::wait msg=\"WM_QUIT\"/>\n" );
					}
					else
					{
						TRACE( "<msyncthread::wait msg=\"unknown\"/>\n" );
					}
				}
			}
			break;
		}
	}
}

/**
 * triggers the event,
 * invoked from another thread.
 */
bool MSyncThread::trigger()
{
	ASSERT( ivHandleEvent );
	return ::SetEvent( this->ivHandleEvent ) == TRUE;
}