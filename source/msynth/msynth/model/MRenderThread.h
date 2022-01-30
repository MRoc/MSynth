#ifndef __MRenderThread
#define __MRenderThread

#include <framework/thread/MThread.h>
#include <framework/thread/IProgress.h>

#include "MSong.h"
#include "MTimeController.h"
#include "Sequencer.h"
#include "file/WaveFileHandling.h"

/**
 * thread used for async rendering of a song to a wave file.
 */
class MRenderThread :
	public MThread
{
protected:

	/**
	 * progress interface for telling the progress of rendering
	 */
	IProgress* ivPtProgress;

	/**
	 * the time controller
	 */
	MTimeController* ivPtTimeControl;

	/**
	 * the sequencer
	 */
	Sequencer* ivPtSequencer;

	/** 
	 * the song to render
	 */
	MSong* ivPtSong;

	/**
	 * the filename to render in
	 */
	String ivFileName;

public:

	/**
	 * constructor
	 */
	MRenderThread( String fileName, MTimeController* ptController, Sequencer* ptSequencer, MSong* ptSong );

	/**
	 * destructor
	 */
	virtual ~MRenderThread();

	/**
	 * does the rendering in a iwn thread
	 */
	virtual void run();

	/**
	 * to set the progress object
	 */
	virtual void setProgressCallback( IProgress* ivPtProgress );

	/**
	 * returns the progress object
	 */
	virtual IProgress* getProgressCallback();
};

#endif