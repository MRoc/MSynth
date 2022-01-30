#ifndef __MTimeController
#define __MTimeController

#include <framework/MTypes.h>

#include "MMarkerListener.h"
#include "MSystemListener.h"

#include "interfaces/IBpmListener.h"
#include "streamingengine/IAudioEngine.h"

class ISyncControl
{
public:
	virtual void sync( bool fireUpdate ) = 0;
};

class MTimeController
{
protected:

	static MTimeController*			gvSharedInstance;

	int								ivCurrentPattern,
									ivCurrentSystemStep,
									ivNextPattern,
									ivNextSystemStep,
									ivLeftMarker,
									ivRightMarker;

	std::vector<IMarkerListener*>	ivMarkerListeners;

	std::vector<ISystemListener*>	ivSystemListeners;

	std::vector<ISyncControl*>		ivSyncListeners;

#ifdef __THREAD_SAFE
	MCriticalSection				ivCriticalSection;
#endif

	MTimeController();
	virtual ~MTimeController();

public:

	static MTimeController* getInstance();
	static void release();

	void			setCurrentPatternEx( int index );

	// the input of the gui ( inperformant )
	void			setCurrentPattern( int pattern );
	int				getCurrentPattern();

	void			setCurrentSystemStep( int currentSystemStep );
	int				getCurrentSystemStep();

	void			setNextPattern( int nextPattern );
	int				getNextPattern();

	void			setNextSystemStep( int sysStep );
	int				getNextSystemStep();
					

	// the input from the sequencer ( performant )
	void			systemPatternChanged( int currentPattern );
	void			systemStepChanged( int currentPattern );

	void			setLeftMarker( int leftMarker );
	int				getLeftMarker();

	void			setRightMarker( int rightMarker );
	int				getRightMarker();

	// marker listener
	void			addMarkerListener( IMarkerListener* ptWnd );
	void			removeMarkerListener( IMarkerListener* ptWnd );

	// low performant system listeners ( e.g. the sequencer )
	void			addSystemListener( ISystemListener* ptSystemListener );
	void			removeSystemListener( ISystemListener* ptSystemListener );

	void			sync( bool fireUpdate );
	void			addSyncListener( ISyncControl* ptSync );
	void			removeSyncListener( ISyncControl* ptSync );

	/**
		* returns true if pattern change
		*/
	bool			calculateNextSteps();
	void			updateNext();

	virtual void	getRealtimePosition( int& pattern, int& step );

protected:

	void			fireMarkerChanged();

	void			fireSystemStepChanged();
	void			fireSystemPatternChanged();
};

#endif