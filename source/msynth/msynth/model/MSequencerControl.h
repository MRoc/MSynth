#ifndef __MSequencerControl
#define __MSequencerControl

#include "Sequencer.h"
#include "MTimeController.h"
#include "MUpdateThread.h"

#include "interfaces/IPlayState.h"
#include "streamingengine/MAudioEngine.h"

class MSequencerControlWnd;

class MSequencerControl :
	public IPlayState
{
protected:

	IPlayState::PlayState	ivPlayState;
	MListenerList			ivPlayStateListeners;

	MSong*					ivPtSong;
	Sequencer*				ivPtSequencer;
    MAudioEngine*			ivPtAudioEngine;
	WaveFileHandling*		ivPtOutput;
	MTimeController*		ivPtTimeControl;
	MUpdateThread*			ivPtUpdateThread;

public:

	MSequencerControl();
	virtual ~MSequencerControl();

	virtual IPlayState::PlayState getPlayState();
	virtual void setPlayState( PlayState playState );

	void setSong( MSong* ptSong );
	MSong* getSong();

	void setSequencer( Sequencer* ptSequencer );
	Sequencer* getSequencer();

	IAudioEngine* getAudioEngine();

	void setTimeControl( MTimeController* ptTimeControl );
	MTimeController* getTimeControl();

	void setUpdateThread( MUpdateThread* ptUpdateThread );
	MUpdateThread* getUpdateThread();

	void start();
	void stop(); // <- stop streaming if on!!!
	bool record( String fileName );

	bool isPlaying();

	void addPlayStateListener( IPlayStateListener* ptWnd );
	void removePlayStateListener( IPlayStateListener* ptWnd );

	bool startStreamToDisk( String fileName );
	bool writeStreamToDisk( MW_SAMPLETYPE *ptBuffer, unsigned int bufferLength );
	bool stopStreamToDisk();

protected:

	void firePlayStateChanged();

	bool render( String fileName );

friend MSequencerControlWnd;
};

#endif