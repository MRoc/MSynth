#ifndef __NOTIFYSTREAMINGAUDIO
#define __NOTIFYSTREAMINGAUDIO

#include <framework/MTypes.h>
#include <framework/MNativeTypes.h>
#include <framework/map/MProperties.h>
#include <framework/thread/MThread.h>
#include <gui/MTopWnd.h>

#include <mmsystem.h>
#include <dsound.h>

#include "IAudioEngine.h"
#include "MAudioThread.h"

#include "../../../resource.h"

#define MAE_SAMPLINGRATE 44100
#define MAE_BITS 16
#define MAE_CHANNEL_COUNT 2

#define MAE_NOTIFY_COUNT 4
#define MAE_PREFERRED_NOTIFY_LENGTH 2048

/**
 * implementation of a streaming audioengine.
 * the audio engine requires a IAudioStream
 * that will be played. so it hides things
 * like waiting for filling up the buffer.
 */
class MAudioEngine :
	public MThread,
	public IAudioEngine
{
public:

	static const char*			gvDxDeviceTag;
	static const char*			gvStickyFocusTag;
	static const char*			gvBufferLengthTag;

protected:

	static std::vector<GUID*>	gvDxDeviceEnum;
	static std::vector<String>	gvDxDescriptionEnum;

	HRESULT					err;

	// buffer...
	DWORD					ivNotifyCount;
	DWORD					ivNotifySize;
	DWORD					ivBufferLength;

	HANDLE*					ivNotificationEvents;
	IDirectSoundNotify*		ivPtDirectSoundNotify;

	// direct sound...
	WAVEFORMATEX			ivWaveFormat;
	IDirectSound*			ivPtDirectSound;
	IDirectSoundBuffer*		ivPtSecundarySoundBuffer;
	IDirectSoundBuffer*		ivPtPrimarySoundBuffer;

	HWND					ivHwnd;

	GUID*					ivPtDxDevice;

	MAudioThread*			ivPtThreadRender;

public:

	/**
	 * constructor
	 */
	MAudioEngine( MTopWnd* pWnd );

	/**
	 * destructor
	 */
	virtual ~MAudioEngine();

	/**
	 * creates a IAudioEngine (inheritanced from IAudioEngine)
	 * throws MException
	 */
	virtual void create( IProcessor* ptStream );

	/**
	 * destroys the IAudioEngine created with the create method
	 * (inheritanced from IAudioEngine)
	 */
	virtual void destroy();

	/**
	 * starts playing the stream
	 * (inheritanced from IAudioEngine&MThread)
	 */
	virtual bool start();

	/**
	 * stops playing the stream
	 * (inheritanced from IAudioEngine&MThread)
	 */
	virtual bool stop();

	/**
	 * returns true if is playing
	 * (inheritanced from IAudioEngine)
	 */
	virtual bool isPlaying();

	/**
	 * returns the samples per second played (samplingrate)
	 * (inheritanced from IAudioEngine)
	 */
	virtual unsigned int getSamplingRate();

	/**
	 * returns the bits per sample, e.g. 8 or 16
	 * (inheritanced from IAudioEngine)
	 */
	virtual unsigned short getBitsPerSample();

	/**
	 * returns a available channels
	 * (inheritanced from IAudioEngine)
	 */
	virtual unsigned short getChannelCount();

	/**
	 * returns true if all was valid created
	 */
	virtual bool isAllValid();

	/**
	 * the threads run method
	 * (inheritanced from MThread)
	 */
	virtual void run();

	/**
	 * initialized all available direct sound device descriptions
	 */
	static bool initDxDescs();

	/**
	 * releases all available direct sound device descriptions
	 */
	static void releaseDxDescs();

	/**
	 * returns the number of available direct sound devices
	 */
	static unsigned int getDxDeviceCount();

	/**
	 * returns the direct sound device by index
	 */
	static LPGUID getDxDevice( unsigned int index );

	/**
	 * returns the direct sound device description by index
	 */
	static String getDxDeviceDesc( unsigned int index );

protected:

	/**
	 * static callback required for enumerating direct sound devices
	 */
	static BOOL CALLBACK enumCallBack(
		LPGUID lpGuid,
		LPCSTR lpstrDescription,
		LPCSTR lpstrModule,
		LPVOID hwnd );

	/**
	 * sets the format of the primary direct sound buffer
	 * throws MException
	 */
	void setPrimaryBufferFormat(
		unsigned int dwPrimaryChannels = MAE_CHANNEL_COUNT,
		unsigned int dwPrimaryFreq = MAE_SAMPLINGRATE,
		unsigned int dwPrimaryBitRate = MAE_BITS);

	/**
	 * handles the thread notification, invokes the IAudioStream.render method
	 */
	bool handleWaveStreamNotification( int notifyIndex );

	/**
	 * tries to restore the buffer if lost
	 */
	void restoreBuffer( IDirectSoundBuffer* pDSB);

	/**
	 * tries the fill the secondary sound buffer with silence
	 * throws MException
	 */
	void fillBufferWithSilence();

	/**
	 * error function decoding the errorcode
	 */
	String error( HRESULT err );
};

#endif
