#include "msequencercontrol.h"
#include "MRenderThread.h"
#include "../ui/dialog/MProgressDialog.h"
#include <framework/io/MIoException.h>
#include <gui/dialog/MMessageBox.h>
#include <gui/MApp.h>

MSequencerControl::MSequencerControl() :
	ivPlayState( NEUTRAL ),
	ivPtSong( 0 ),
	ivPtSequencer( 0 ),
    ivPtAudioEngine( 0 ),
	ivPtTimeControl( 0 ),
	ivPtUpdateThread( 0 ),
	ivPtOutput( 0 )
{
	if( MApp::getInstance()->getMainWnd() == 0 )
		MLogger::logError(
			"MSequencerControl::MSequencerControl: MApp::getInstance()->getMainWnd() failed" );
	ivPtAudioEngine = new MAudioEngine( MApp::getInstance()->getMainWnd() );
}

MSequencerControl::~MSequencerControl()
{
	ivPtTimeControl = 0;

	SAFE_DELETE( ivPtOutput );

	SAFE_DELETE( ivPtSong );
    SAFE_DELETE( ivPtAudioEngine );
	SAFE_DELETE( ivPtOutput );
	SAFE_DELETE( ivPtUpdateThread );
	SAFE_DELETE( ivPtSequencer );
}

void MSequencerControl::setSong( MSong* ptSong )
{
	ivPtSong = ptSong;
}

void MSequencerControl::setSequencer( Sequencer* ptSequencer )
{
	ivPtSequencer = ptSequencer;
}

IAudioEngine* MSequencerControl::getAudioEngine()
{
	return ivPtAudioEngine;
}

void MSequencerControl::setTimeControl( MTimeController* ptTimeControl )
{
	if( ivPtTimeControl && ivPtUpdateThread )
		ivPtTimeControl->removeSyncListener( ivPtUpdateThread );

	ivPtTimeControl = ptTimeControl;

	if( ivPtTimeControl )
	{
		if( ivPtUpdateThread )
			ivPtTimeControl->addSyncListener( ivPtUpdateThread );
	}
}

void MSequencerControl::setUpdateThread( MUpdateThread* ptUpdateThread )
{
	if( ivPtTimeControl && ivPtUpdateThread )
		ivPtTimeControl->removeSyncListener( ivPtUpdateThread );

	ivPtUpdateThread = ptUpdateThread;

	if( ivPtTimeControl && ivPtUpdateThread )
		ivPtTimeControl->addSyncListener( ivPtUpdateThread );
}

IPlayState::PlayState MSequencerControl::getPlayState()
{
	return ivPlayState;
}

void MSequencerControl::setPlayState( PlayState playState )
{
	ivPlayState = playState;
	firePlayStateChanged();
}

bool MSequencerControl::startStreamToDisk( String fileName )
{
	MLogger::logMessage( "<msequencercontrol::startstreamtoDisk file=\"%s\"/>\n", fileName );

	bool back = false;

	if( ! ivPtOutput )
	{
		ivPtOutput = new WaveFileHandling();

		try
		{
			ivPtOutput->openFile( fileName );
		}
		catch( MIoException ae )
		{
			SAFE_DELETE( ivPtOutput );
			MMessageBox::showError( "ERROR_STARTING_SESSION", fileName  );
			return false;
		}

		ivPtOutput->setFormat(
			44100,
			16,
			2 );

		back = true;
	}

	return back;
}

bool MSequencerControl::writeStreamToDisk( MW_SAMPLETYPE *ptBuffer, unsigned int bufferLength )
{
	ASSERT(	ivPtOutput );
	bool back = true;
	try
	{
		ivPtOutput->writeData(
			(unsigned char*)ptBuffer,
			bufferLength*sizeof(MW_SAMPLETYPE) );
	}
	catch( MIoException ae )
	{
		back = false;
	}
	return back;
}

bool MSequencerControl::stopStreamToDisk()
{
	ASSERT(
		ivPlayState == IPlayState::RENDERING ||
		ivPlayState == IPlayState::SESSION_NOT_PLAYING ||
		ivPlayState == IPlayState::SESSION_PLAYING );

	ASSERT( ivPtOutput );

	MLogger::logMessage( "<msequencercontrol::stopstreamtodisk/>\n" );

	bool back = true;
	try
	{
		ivPtOutput->finalizeFile();
	}
	catch( MIoException ae )
	{
		MMessageBox::showError( "ERROR_WRITING_FILEHEADER_SESSION" );
		back = false;
	}
	try
	{
		ivPtOutput->closeFile();
	}
	catch( MIoException ae )
	{
		MMessageBox::showError( "ERROR_CLOSING_FILE_SESSION" );
		back = false;
	}
	SAFE_DELETE( ivPtOutput );

	if( ivPlayState == IPlayState::SESSION_PLAYING )
		setPlayState( IPlayState::PLAYING );
	else if( ivPlayState == IPlayState::RENDERING || ivPlayState == IPlayState::SESSION_NOT_PLAYING )
		setPlayState( IPlayState::NEUTRAL );
	else
		ASSERT( false );

	return back;
}

bool MSequencerControl::render( String fileName )
{
	ASSERT( ivPlayState == NEUTRAL );

	MLogger::logMessage( "<msequencercontrol::render file=\"%s\"/>\n", fileName );
	
	setPlayState( RENDERING );
	MRenderThread t( fileName, ivPtTimeControl, ivPtSequencer, ivPtSong );
	MProgressDialog d( &t );
	d.doModal();
	setPlayState( NEUTRAL );
	return true;
}

void MSequencerControl::start()
{
	MLogger::logMessage( "<msequencercontrol::start/>\n" );
	if( getPlayState() == IPlayState::NEUTRAL )
	{
		ivPtSequencer->play();
		ivPtAudioEngine->start();
		setPlayState( IPlayState::PLAYING );
		if( ivPtUpdateThread )
			ivPtUpdateThread->start();
	}
	else if( getPlayState() == IPlayState::SESSION_NOT_PLAYING )
	{
		ivPtSequencer->play();
		ivPtAudioEngine->start();
		setPlayState( IPlayState::SESSION_PLAYING );
		if( ivPtUpdateThread )
			ivPtUpdateThread->start();
	}
}

void MSequencerControl::stop()
{
	MLogger::logMessage( "<msequencercontrol::stop/>\n" );
	// <- stop streaming if on!!!
	if( getPlayState() == IPlayState::PLAYING )
	{
		if( ivPtUpdateThread )
			ivPtUpdateThread->stop();
		ivPtAudioEngine->stop();
		setPlayState( IPlayState::NEUTRAL );
	}
	else if( getPlayState() == IPlayState::SESSION_PLAYING )
	{
		if( ivPtUpdateThread )
			ivPtUpdateThread->stop();
		ivPtAudioEngine->stop();
		stopStreamToDisk();
		setPlayState( IPlayState::NEUTRAL );
	}
	else if( getPlayState() == IPlayState::SESSION_NOT_PLAYING )
	{
		stopStreamToDisk();
		setPlayState( IPlayState::NEUTRAL );
	}
	else if( getPlayState() == IPlayState::RENDERING )
	{
		stopStreamToDisk();
		setPlayState( IPlayState::NEUTRAL );
	}
}

bool MSequencerControl::record( String fileName )
{
	MLogger::logMessage( "<msequencercontrol::record/>\n" );

	bool back = false;
	if( ivPlayState == IPlayState::NEUTRAL ||
		ivPlayState == IPlayState::PLAYING )
	{
		if( ( back = startStreamToDisk( fileName ) ) )
		{
			if( ivPlayState == IPlayState::NEUTRAL )
				setPlayState( IPlayState::SESSION_NOT_PLAYING );
			else if( ivPlayState == IPlayState::PLAYING )
				setPlayState( IPlayState::SESSION_PLAYING );
		}
	}
#ifdef _DEBUG
	else
		ASSERT( false );
#endif
	return back;
}

Sequencer* MSequencerControl::getSequencer()
{
	return ivPtSequencer;
}

MUpdateThread* MSequencerControl::getUpdateThread()
{
	return ivPtUpdateThread;
}

MSong* MSequencerControl::getSong()
{
	return ivPtSong;
}

MTimeController* MSequencerControl::getTimeControl()
{
	return ivPtTimeControl;
}

bool MSequencerControl::isPlaying()
{
	return
		ivPlayState == IPlayState::PLAYING ||
		ivPlayState == IPlayState::SESSION_PLAYING;
}

void MSequencerControl::addPlayStateListener( IPlayStateListener* ptListener )
{
	ivPlayStateListeners.addListener( ptListener );
}

void MSequencerControl::removePlayStateListener( IPlayStateListener* ptListener )
{
	ivPlayStateListeners.removeListener( ptListener );
}

void MSequencerControl::firePlayStateChanged()
{
	unsigned int count = ivPlayStateListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		( (IPlayStateListener*) ivPlayStateListeners.getListener( i ) )->onPlayStateChanged( ivPlayState );
}