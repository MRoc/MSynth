/**
 *
 *	MAudioEngine (C)2000-2002 MRoc hifiShock
 *
 *	v. 1.0		- working version with errorhandling
 *	v. 2.0beta	- Notification-Engine
 *	v. 2.1		- Derived from IAudioEngine
 *	v. 2.2		- Derived from MThread
 *	v. 3.0		- Using a message queue and the MAudioThread
 *				- using exceptions instead of return codes
 *
 */

#include "MAudioEngine.h"
#include "MAudioEngineEvent.h"
#include "../interfaces/MSoundBufferUtils.h"
#include <framework/performance/MClock.h>
#include <framework/io/MLogger.h>
#include <gui/dialog/MMessageBox.h>
#include <framework/MUtil.h>
#include <gui/win32impl/MTopWndWin32Impl.h>

std::vector<LPGUID>	MAudioEngine::gvDxDeviceEnum;
std::vector<String> MAudioEngine::gvDxDescriptionEnum;

const char* MAudioEngine::gvDxDeviceTag			= "DxDevice";
const char* MAudioEngine::gvStickyFocusTag		= "StickyFocus";
const char* MAudioEngine::gvBufferLengthTag		= "BufferLength";

/**
 * constructor
 */
MAudioEngine::MAudioEngine( MTopWnd* pWnd ) :
	MThread(),
	ivPtThreadRender( 0 ),
	ivPtDirectSound( 0 ),
	ivPtSecundarySoundBuffer( 0 ),
	ivPtPrimarySoundBuffer( 0 ),
	ivPtDirectSoundNotify( 0 ), 
	ivNotificationEvents( 0 ),
    ivNotifyCount( MAE_NOTIFY_COUNT ),
	ivNotifySize( MAE_PREFERRED_NOTIFY_LENGTH ),
	ivPtDxDevice( 0 ),
	ivHwnd( 0 )
{
	ivHwnd = ((MTopWndWin32Impl*)pWnd->getImpl())->getHandle();
	// create events for dx communication...
	ivNotificationEvents = new HANDLE[ ivNotifyCount ];
	for( unsigned int i=0;i<ivNotifyCount;i++ )
	{
		ivNotificationEvents[ i ] =
			CreateEvent(
				NULL,
				false,
				false,
				NULL );
		if( ivNotificationEvents[ i ] == 0 )
			MLogger::logError( "MAudioEngine::MAudioEngine: CreateEvent(index=%i) failed", i );
		ASSERT( ivNotificationEvents[ i ] );
	}
}

/**
 * desctructor
 */
MAudioEngine::~MAudioEngine()
{
	//RELEASE_ASSERT( ivIsRunning == false );

	// destroy dx engine...
	destroy();

	// delete notifications...
	for( unsigned int i=0;i<ivNotifyCount;i++ )
	{
		BOOL resClose = CloseHandle( ivNotificationEvents[ i ] );
		if( resClose == false )
			MLogger::logError( "MAudioEngine::~MAudioEngine(): CloseHandle(index=%i) failed", i );
	}
	delete ivNotificationEvents;
}

/**
 * creates a IAudioEngine
 * (inheritanced from IAudioEngine)
 */
void MAudioEngine::create( IProcessor* ptStream )
{
	if( ivPtThreadRender )
		destroy();

	// create new audio thread...
	ivPtThreadRender = new MAudioThread( ptStream );
	ivPtThreadRender->start();

	// fetch settings...
	ivNotifySize = MProperties::getInt( gvBufferLengthTag, 8192, 512, 8192 );
	ivPtDxDevice = gvDxDeviceEnum[ MProperties::getInt( gvDxDeviceTag, 0, 0, gvDxDeviceEnum.size() ) ];

	MLogger::logMessage( "<maudioengine::create>\n" );
	MLogger::logMessage( "\t<ivPtDxDevice=\"%i\"/>\n", MProperties::getInt( gvDxDeviceTag, 0, 0, gvDxDeviceEnum.size() ) );
	MLogger::logMessage( "\t<notifysize=\"%i\"/>\n", ivNotifySize );
	MLogger::logMessage( "\t<sickyfocus=\"%s\"/>\n", MProperties::getInt( gvStickyFocusTag, 1, 0, 1 ) ? "true" : "false" );
	MLogger::logMessage( "</maudioengine::create>\n" );

	int bufferFlags =
		DSBCAPS_LOCSOFTWARE |
		DSBCAPS_CTRLPOSITIONNOTIFY |
		DSBCAPS_GETCURRENTPOSITION2;

	if( MProperties::getInt( gvStickyFocusTag, 1, 0, 1 ) == 1 )
		bufferFlags |= DSBCAPS_STICKYFOCUS;
	
	// creating direct sound object...
	if(
		FAILED( err =
			DirectSoundCreate(
				ivPtDxDevice,
				&ivPtDirectSound,
				NULL ) ) )
	{
		ivPtDirectSound = 0;
		throw MException( "MAudioEngine::create: DirectSoundCreate failed! (" + error( err ) + ")" );
	}

	// set cooperative level...
	if(
		FAILED( err =
			ivPtDirectSound->SetCooperativeLevel(
				ivHwnd,
				DSSCL_PRIORITY ) ) )
	{
		SAFE_RELEASE( ivPtDirectSound );
		throw MException( "MAudioEngine::create: SetCooperativeLevel failed! (" + error( err ) + ")" );
	}

	// set primary buffer formatl...
	setPrimaryBufferFormat( MAE_CHANNEL_COUNT );

	// init wave format...
	ivWaveFormat.nChannels			= MAE_CHANNEL_COUNT;
	ivWaveFormat.wFormatTag			= WAVE_FORMAT_PCM;
	ivWaveFormat.wBitsPerSample		= MAE_BITS;
	ivWaveFormat.nSamplesPerSec		= MAE_SAMPLINGRATE;
	ivWaveFormat.nBlockAlign		= ivWaveFormat.nChannels * ivWaveFormat.wBitsPerSample/8;
	ivWaveFormat.nAvgBytesPerSec	= ivWaveFormat.nSamplesPerSec * ivWaveFormat.nBlockAlign;
	ivWaveFormat.cbSize				= 0;

    // calculate buffer size...
    ivBufferLength					= ivNotifySize * ivNotifyCount;
	
	// create secundary buffer...
	DSBUFFERDESC bufferDesc;
    ZeroMemory( &bufferDesc, sizeof(DSBUFFERDESC) );
    bufferDesc.dwSize			= sizeof(DSBUFFERDESC);
    bufferDesc.dwFlags			= bufferFlags;
    bufferDesc.dwBufferBytes	= ivBufferLength;
    bufferDesc.lpwfxFormat		= &ivWaveFormat;
	if(
		FAILED( err =
			ivPtDirectSound->CreateSoundBuffer(
				&bufferDesc,
				&ivPtSecundarySoundBuffer,
				NULL ) ) )
	{
		ivPtSecundarySoundBuffer = 0;
		destroy();
		throw MException( "MAudioEngine::create: CreateSoundBuffer failed! (" + error( err ) + ")" );
	}

    // query for direct sound notify interface...
    if( 
		FAILED( err =
			ivPtSecundarySoundBuffer->QueryInterface(
			IID_IDirectSoundNotify,
			(VOID**)&ivPtDirectSoundNotify ) ) )
    {
		ivPtDirectSoundNotify = 0;
		destroy();
		throw MException( "MAudioEngine::create: QueryInterface(IID_IDirectSoundNotify) failed! (" + error( err ) + ")" );
    }

	// create position notify...
	DSBPOSITIONNOTIFY* pPositionNotifies = new DSBPOSITIONNOTIFY[ ivNotifyCount ];
    for( DWORD i = 0; i < ivNotifyCount; i++ )
    {
        pPositionNotifies[ i ].dwOffset     = (ivNotifySize * i) + ivNotifySize - 1;
        pPositionNotifies[ i ].hEventNotify = ivNotificationEvents[ i ];             
    }
    
    // set dx notifications...
    if(
		FAILED( err =
			ivPtDirectSoundNotify->SetNotificationPositions(
				ivNotifyCount, 
				pPositionNotifies ) ) )
    {
		destroy();
		throw MException( "MAudioEngine::create: SetNotificationPositions failed! (" + error( err ) + ")" );
    }

	// delete position notify...
	delete pPositionNotifies;
}

/**
 * destroys the IAudioEngine created with the create method
 * (inheritanced from IAudioEngine)
 */
void MAudioEngine::destroy()
{
	// stop and terminate render thread...
	ivPtThreadRender->stop();
	if( ivPtThreadRender )
		delete ivPtThreadRender;

	// release dx objects...
	SAFE_RELEASE( ivPtPrimarySoundBuffer );
	SAFE_RELEASE( ivPtSecundarySoundBuffer );
	SAFE_RELEASE( ivPtDirectSoundNotify );
	SAFE_RELEASE( ivPtDirectSound );
}

/**
 * sets the format of the primary direct sound buffer
 */
void MAudioEngine::setPrimaryBufferFormat(
	unsigned int dwPrimaryChannels, 
    unsigned int dwPrimaryFreq, 
    unsigned int dwPrimaryBitRate )
{
    // init buffer desc for primary buffer...
    DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize        = sizeof(DSBUFFERDESC);
    dsbd.dwFlags       = DSBCAPS_PRIMARYBUFFER;
    dsbd.dwBufferBytes = 0;
    dsbd.lpwfxFormat   = NULL;
       
	// fetch primary buffer...
    if(
		FAILED( err =
			ivPtDirectSound->CreateSoundBuffer(
				&dsbd,
				&ivPtPrimarySoundBuffer,
				NULL ) ) )
	{
		ivPtPrimarySoundBuffer = 0;
		throw MException( "MAudioEngine::setPrimaryBufferFormat: CreateSoundBuffer failed (" + error( err ) + ")" );
	}

	// init wave format for primary buffer...
    WAVEFORMATEX wfx;
    wfx.wFormatTag      = WAVE_FORMAT_PCM; 
    wfx.nChannels       = (WORD) dwPrimaryChannels; 
    wfx.nSamplesPerSec  = dwPrimaryFreq; 
    wfx.wBitsPerSample  = (WORD) dwPrimaryBitRate; 
    wfx.nBlockAlign     = wfx.wBitsPerSample / 8 * wfx.nChannels;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	wfx.cbSize			= 0;

	// set wave format of primary buffer...
    if(
		FAILED( err =
			ivPtPrimarySoundBuffer->SetFormat( &wfx ) ) )
	{
		SAFE_RELEASE( ivPtPrimarySoundBuffer );
		throw MException( "MAudioEngine::setPrimaryBufferFormat: SetFormat failed (" + error( err ) + ")" );
	}
}

/**
 * starts playing the stream
 * (inheritanced from IAudioEngine&MThread)
 */
bool MAudioEngine::start()
{
	bool back = true;

	if( isAllValid() &&  !isPlaying() )
	{
		try
		{
			// set playpos...
			if( FAILED( err = ivPtSecundarySoundBuffer->SetCurrentPosition( 0 ) ) )
			{
				throw MException(
					"MAudioEngine::start: SetCurrentPosition failed ("+error(err)+")");
			}

			// zerorize buffer...
			fillBufferWithSilence();

			// play buffer...
			if( FAILED( err =
				ivPtSecundarySoundBuffer->Play(
					0,
					0,
					DSBPLAY_LOOPING ) ) )
			{
				throw MException(
					"MAudioEngine::start: Play failed ("+error(err)+")");
			}

			// start notify thread...
			setPrority( MThread::PRIORITY_HIGHEST );
			if( MThread::start() == false )
			{
				throw MException(
					"MAudioEngine::start: MThread::start failed" );
			}
		}
		catch( MException ae )
		{
			back = false;
			MMessageBox::showError( ae.getExceptionDescripton() );
		}
	}
	else
		back = false;

    return back;
}

/**
 * stops playing the stream
 * (inheritanced from IAudioEngine&MThread)
 */
bool MAudioEngine::stop()
{
	MLogger::logMessage( "<maudioengine::stop/>\n" );

	bool back = true;
    
	try
	{
		if( isAllValid() && isPlaying() )
		{
			if( FAILED( err = ivPtSecundarySoundBuffer->Stop()) )
			{
				throw MException( "MAudioEngine::stop: Stop failed (" + error(err) + ")" );
			}	

			if( MThread::stop() == false )
			{
				throw MException( "MAudioEngine::stop: MThread::stop failed" );
			}
		}
		else
			back = false;
	}
	catch( MException ae )
	{
		back = false;
		MMessageBox::showError( ae.getExceptionDescripton() );
	}

	return true;
}

/**
 * returns true if is playing
 * (inheritanced from IAudioEngine)
 */
bool MAudioEngine::isPlaying()
{
	if( isAllValid() )
	{
		DWORD status;
		ivPtSecundarySoundBuffer->GetStatus(&status);
		if(
			( status & (DSBSTATUS_LOOPING|DSBSTATUS_PLAYING) ) &&
			ivIsRunning )
		{	
			return true;
		}
	}

	return false;
}

/**
 * returns true if all was valid created
 */
bool MAudioEngine::isAllValid()
{
	return
		ivPtDirectSound != NULL &&
		ivPtSecundarySoundBuffer != NULL; 
}

/**
 * returns the samples per second played (samplingrate)
 * (inheritanced from IAudioEngine)
 */
unsigned int MAudioEngine::getSamplingRate()
{
	return (unsigned int) ivWaveFormat.nSamplesPerSec;
}

/**
 * returns the bits per sample, e.g. 8 or 16
 * (inheritanced from IAudioEngine)
 */
unsigned short MAudioEngine::getBitsPerSample()
{
	return (unsigned short) ivWaveFormat.wBitsPerSample;
}

/**
 * returns a available channels
 * (inheritanced from IAudioEngine)
 */
unsigned short MAudioEngine::getChannelCount()
{
	return (unsigned short) ivWaveFormat.nChannels;
}

/**
 * the threads run method
 * (inheritanced from MThread)
 */
void MAudioEngine::run()
{
    MSG     msg;
    DWORD   dwResult;

	while( ivIsRunning ) 
    { 
        dwResult =
			MsgWaitForMultipleObjects(
				ivNotifyCount,
				ivNotificationEvents,
				false,
				INFINITE,
				QS_ALLEVENTS );

		dwResult -= WAIT_OBJECT_0;

		if( dwResult >= 0 && dwResult < ivNotifyCount )
		{
			if( handleWaveStreamNotification( dwResult ) == false )
			{
				MMessageBox::showError( "DS_HANDLING_DS_NOTIFY_FAILED" );
				ivIsRunning = false;
				MLogger::logWarning("<maudioengine::run msg=\"exiting thread because of failure!\"/>\n");
			}
		}
		else
		{
            while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
            { 
                if( msg.message == WM_QUIT )
				{
                    ivIsRunning = false;
					MLogger::logMessage( "<maudioengine::run msg=\"WM_QUIT\"/>\n" );
				}
				else
					MLogger::logMessage( "<maudioengine::run msg=\"unknown\" value=\"%i\"/>\n", msg.message );
            }
        }
    }
}

/**
 * tries the fill the secondary sound buffer with silence
 */
void MAudioEngine::fillBufferWithSilence()
{
	LPVOID pv_writePos1 = 0, pv_writePos2 = 0;
	DWORD pdw_blockSize1 = 0, pdw_blockSize2 = 0;
	
    // lock entire buffer...
    if(
		FAILED( err =
			ivPtSecundarySoundBuffer->Lock(
				0,
				ivBufferLength, 
				&pv_writePos1,
				&pdw_blockSize1, 
				&pv_writePos2,
				&pdw_blockSize2,
				DSBLOCK_ENTIREBUFFER ) ) )
	{
		throw MException( "MAudioEngine::fillBufferWithSilence: Lock failed ("+error(err)+")" );
	}

//	RELEASE_ASSERT( pdw_blockSize2 == 0 );
//	RELEASE_ASSERT( ivBufferLength == pdw_blockSize1 );

	// zerorize buffer...
    ZeroMemory( (unsigned char*)pv_writePos1, pdw_blockSize1 );

    // unlock buffer...
    if( 
		FAILED( err =
			ivPtSecundarySoundBuffer->Unlock(
				pv_writePos1,
				pdw_blockSize1,
				pv_writePos2,
				pdw_blockSize2 ) ) )
	{
        throw MException( "MAudioEngine::fillBufferWithSilence: Unlock failed ("+error(err)+")" );
	}
}

/**
 * handles the thread notification, invokes the IAudioStream.render method
 */
bool MAudioEngine::handleWaveStreamNotification( int notifyIndex )
{
	/*unsigned int samplesToRender = ivNotifySize / (2 * sizeof( MW_SAMPLETYPE ));
	FP timeToRender = FP(samplesToRender) / MW_SAMPLINGRATE; // seconds required for next dx notify

	__int64 timestamp = MClock::getTimeStamp();
	__int64 timestampTooLate = timestamp + __int64(timeToRender * FP(MClock::getFrequency()));*/

	return
		ivPtThreadRender->postJob(
			new MAudioEngineEvent(
				ivPtSecundarySoundBuffer,	// direct sound buffer
				notifyIndex * ivNotifySize,	// write offset
				ivNotifySize,				// length of buffer
				0 /*timestamp*/,			// time stamp creating
				0 /*timestampTooLate*/		// time stamp too late
				) );
}

/**
 * tries to restore the buffer if lost
 */
void MAudioEngine::restoreBuffer( IDirectSoundBuffer* pDSB )
{
    //RELEASE_ASSERT( pDSB );

	// fetch buffer state...
    DWORD dwStatus;
    if( FAILED( err = pDSB->GetStatus( &dwStatus ) ) )
	{
		throw MException( "MAudioEngine::restoreBuffer: GetStatus failed ("+error(err)+")");
	}

	// restore buffer if nescessary...
    if( dwStatus & DSBSTATUS_BUFFERLOST )
    {
        // Since the app could have just been activated, then
        // DirectSound may not be giving us control yet, so 
        // the restoring the buffer may fail.  
        // If it does, sleep until DirectSound gives us control.

		// this is a typical microsoft sleep ;-)
        do 
        {
            err = pDSB->Restore();
            if( err == DSERR_BUFFERLOST )
			{
                Sleep( 10 );
			}
        }
        while( err = pDSB->Restore() );

		// clear buffer...
		fillBufferWithSilence();
    }
}

/**
 * inits the global direct x device descriptions
 */
bool MAudioEngine::initDxDescs()
{
	bool back = true;
	MLogger::logMessage( "<maudioengine::initdxdevice/>\n" );
	if(
		FAILED(
			DirectSoundEnumerate(
				(LPDSENUMCALLBACK)enumCallBack,
				0 ) ) )
	{
		MMessageBox::showError( "DS_ENUM_DEVICE_FAILED" );
		back = false;
	}
	return back;
}

/**
 * releases the global direct x device descriptions
 */
void MAudioEngine::releaseDxDescs()
{
	gvDxDeviceEnum.clear();
	gvDxDescriptionEnum.clear();
}

/**
 * callback used to enumerate over the direct x devices
 */
BOOL CALLBACK MAudioEngine::enumCallBack(
	LPGUID lpGuid,
	LPCSTR lpstrDescription,
	LPCSTR lpstrModule,
	LPVOID hwnd )
{
	gvDxDeviceEnum.push_back(lpGuid);
	gvDxDescriptionEnum.push_back(lpstrDescription);
	return TRUE;
}

/**
 * returns the global number of direct x devices
 */
unsigned int MAudioEngine::getDxDeviceCount()
{
	ASSERT( gvDxDeviceEnum.size() == gvDxDescriptionEnum.size() );
	return gvDxDeviceEnum.size();
}

/**
 * returns the direct x device at the specified index
 */
LPGUID MAudioEngine::getDxDevice( unsigned int index )
{
	ASSERT( index < getDxDeviceCount() );
	return gvDxDeviceEnum[ index ];
}

/**
 * returns the direct x device description at the specified index
 */
String MAudioEngine::getDxDeviceDesc( unsigned int index )
{
	ASSERT( index < getDxDeviceCount() );
	return gvDxDescriptionEnum[ index ];
}

/**
 * does the errorhandling
 */
String MAudioEngine::error( HRESULT error )
{
	switch(error)
	{
		case DSERR_ALLOCATED:
			return "allocation";
		case DSERR_INVALIDPARAM:
			return "invalid param";
		case DSERR_NOAGGREGATION:
			return "no aggregation";
		case DSERR_NODRIVER:
			return "no driver";
		case DSERR_OUTOFMEMORY:
			return "out of memory";
		case DSERR_UNINITIALIZED:
			return "uninitialized";
		case DSERR_UNSUPPORTED:
			return "unsupported";
		case DSERR_BADFORMAT:
			return "bad format";
		case DSERR_CONTROLUNAVAIL:
			return "control unavailable";
		case DSERR_INVALIDCALL:
			return "invalid call";
		case DSERR_PRIOLEVELNEEDED:
			return "prioritylevel needed";
		case DSERR_NOINTERFACE:
			return "no interface (COM)";
		case DSERR_GENERIC:
			return "generic, internal direct x error";
		case DSERR_BUFFERLOST:
			return "buffer lost";
		case DSERR_ALREADYINITIALIZED:
			return "object already initialized";
		default:
			return "unknown error code: " + MUtil::toString( int(error) );
	}
}