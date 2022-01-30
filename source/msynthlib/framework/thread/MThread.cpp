#include "MThread.h"
#include <framework/io/MLogger.h>

const int MThread::TIMEOUT_FOR_TERMINATION = 500;

MThread::MThread() :
	ivThreadId( 0 ),
	ivThreadHandle( NULL ),
	ivIsRunning( false ),
	ivPriority( PRIORITY_NORMAL )
{
	ivSecurityAttr.nLength = sizeof( SECURITY_ATTRIBUTES );
	ivSecurityAttr.lpSecurityDescriptor = 0;
	ivSecurityAttr.bInheritHandle = false;
}

MThread::~MThread()
{
	if( ivIsRunning )
		stop();
}

bool MThread::start()
{
	ivIsRunning = true;

	ivThreadHandle = CreateThread(
		&ivSecurityAttr,
		0,				// initial stack size
		threadProc,		// thread start procedure
		LPVOID( this ),	// thread func params
		0,				// dwCreationFlags (zero=runsImmediately)
		&ivThreadId		// thread ID
		);

	if( ivThreadHandle == 0 || ivThreadId == 0 )
	{
		MLogger::logError( "<mthread::start: CreateThread failed" );
		return false;
	}

	MLogger::logMessage( "<mthread::start id=\"0x%x\"/>\n", ivThreadId );

	return updatePriority();
}

bool MThread::stop()
{
	ASSERT( ivIsRunning );

	if( PostThreadMessage( ivThreadId, WM_QUIT, 0, 0 ) == FALSE )
	{
		MLogger::logError( "MThread::stop: PostThreadMessage failed" );
		return false;
	}

	switch(
		WaitForSingleObject(
			ivThreadHandle,
			MThread::TIMEOUT_FOR_TERMINATION ) )
	{
		case WAIT_FAILED:
			MLogger::logMessage( "<mthread::stopthread msg=\"Failed waiting for the end of thread!\" id=\"0x%x\"/>\n", ivThreadId );
			break;
		case WAIT_TIMEOUT:
			MLogger::logMessage( "<mthread::stopthread msg=\"Timeout when waiting for end of thread!\" id=\"0x%x\"/>\n", ivThreadId );
			break;
		case WAIT_OBJECT_0:
			MLogger::logMessage( "<mthread::stopthread msg=\"Signal state changed\" id=\"0x%x\"/>\n", ivThreadId );
			break;
	}

	if( CloseHandle( ivThreadHandle ) == FALSE )
	{
		MLogger::logError( "MThread::stop: CloseHandle failed" );
		return false;
	}

	MLogger::logMessage( "<mthread::stop id=\"0x%x\"/>\n", ivThreadId );

	ivThreadHandle = 0;
	ivThreadId = 0;

	return true;
}

void MThread::run()
{
	MLogger::logMessage( "<mthread::run id=\"0x%x\"/>\n", ivThreadId );

	while( ivIsRunning ) 
    { 
		MSG msg;
        while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
        { 
            if( msg.message == WM_QUIT )
			{
                ivIsRunning = false;
				MLogger::logMessage("<mthread::exiting id=\"0x%x\"/>\n", ivThreadId );
			}
        }
    }
}

DWORD WINAPI MThread::threadProc( LPVOID lpParameter )
{
	MThread* ptThread = (MThread*) lpParameter;
	ptThread->run();
	ptThread->ivIsRunning = false;
	return 0;
}

void MThread::setPrority( MThread::Priority p )
{
	this->ivPriority = p;
	if( ivIsRunning )
		updatePriority();
}

bool MThread::updatePriority()
{
	ASSERT( ivIsRunning );

	int w32prio = THREAD_PRIORITY_NORMAL;
	switch( ivPriority )
	{
		case PRIORITY_TIME_CRITICAL:
			w32prio = THREAD_PRIORITY_TIME_CRITICAL;
			break;
		case PRIORITY_HIGHEST:
			w32prio = THREAD_PRIORITY_HIGHEST;
			break;
		case PRIORITY_ABOVE_NORMAL:
			w32prio = THREAD_PRIORITY_ABOVE_NORMAL;
			break;
		case PRIORITY_NORMAL:
			w32prio = THREAD_PRIORITY_NORMAL;
			break;
		case PRIORITY_BELOW_NORMAL:
			w32prio = THREAD_PRIORITY_BELOW_NORMAL;
			break;
		case PRIORITY_LOWEST:
			w32prio = THREAD_PRIORITY_LOWEST;
			break;
		case PRIORITY_IDLE:
			w32prio = THREAD_PRIORITY_IDLE;
			break;
	}
	if( SetThreadPriority( ivThreadHandle, w32prio ) != TRUE )
	{
		MLogger::logError( "MThread::updatePriority: SetThreadPriority failed" );
		return false;
	}

	return true;
}