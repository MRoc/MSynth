#include "MUpdateThread.h"

MUpdateThread::MUpdateThread(
	MTimeController* ptTimeController,
	IBpmControl* ptBpmControl )
	: 
	MThread(),
	ivPtTimeController( ptTimeController ),
	ivPtBpmControl( ptBpmControl )
{
	sync( false );
	updateSleep();
	ivPtTimeController->addSyncListener( this );
	ivPtBpmControl->addBpmListener( this );
}

MUpdateThread::~MUpdateThread()
{
	ivPtBpmControl->removeBpmListener( this );
	ivPtTimeController->removeSyncListener( this );
}

void MUpdateThread::sync( bool refire )
{
	//TRACE( "<mupdatethread::sync/>\n" );
	if( refire )
		fireUpdate();
}

void MUpdateThread::updateSleep()
{
	ivSleep = DWORD( ( 60000.0f / float( ivPtBpmControl->getBpm() ) ) / float( 8 ) );
}

void MUpdateThread::run()
{
	while( ivIsRunning )
	{
		MSG msg;
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) == TRUE ) 
		{ 
			if( msg.message == WM_QUIT )
			{
				ivIsRunning = false;
				TRACE("<mupdatethread::exiting/>\n");
			}
		}
		fireUpdate();
		Sleep( ivSleep );
	}
	ivIsRunning = false;
}

void MUpdateThread::fireUpdate()
{
	// WORK TO DO
	int p, s;
	ivPtTimeController->getRealtimePosition( p, s );
	unsigned int count = ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IPlayPositionListener*)ivListeners.getListener( i ))->playPositionChanged( p, s );
	// WORK TO DO
}

void MUpdateThread::addPlayPositionListener( IPlayPositionListener* ptListener )
{
	ivListeners.addListener( ptListener );
}

void MUpdateThread::removePlayPositionListener( IPlayPositionListener* ptListener )
{
	ivListeners.removeListener( ptListener );
}

void MUpdateThread::bpmChanged( unsigned int bpm )
{
	updateSleep();
}
