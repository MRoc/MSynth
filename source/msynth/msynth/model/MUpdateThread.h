#ifndef __MUpdateThread
#define __MUpdateThread

#include <framework/MTypes.h>
#include <framework/thread/MThread.h>
#include <framework/listener/MListenerList.h>

#include "MTimeController.h"
#include "Sequencer.h"

/**
 * defines a play psition listener for components that want
 * the maximal possible exact playposition.
 */
class IPlayPositionListener :
	public IListener
{
public:

	/**
	 * invoked from the update thread when play position changed
	 */
	virtual void playPositionChanged( int pattern, int step ) = 0;
};

class MUpdateThread :
	public MThread,
	public ISyncControl,
	public IBpmListener
{
private:

	DWORD ivSleep; // in msec
	MListenerList ivListeners;
	MTimeController* ivPtTimeController;
	IBpmControl* ivPtBpmControl;

public:

	MUpdateThread(
		MTimeController* ptTimeController,
		IBpmControl* ptBpmControl );

	virtual ~MUpdateThread();

	void sync( bool refire );

	void updateSleep();
	
	virtual void run();

	void fireUpdate();

	void addPlayPositionListener( IPlayPositionListener* ptListener );

	void removePlayPositionListener( IPlayPositionListener* ptListener );

	virtual void bpmChanged( unsigned int bpm );
};

#endif