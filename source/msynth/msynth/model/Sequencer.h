/**
 * (C)2000-2003 MRoc
 */

#ifndef __SEQUENCER
#define __SEQUENCER

#include <framework/MTypes.h>
#include <framework/MNativeTypes.h>
#include <framework/performance/MClock.h>
#include <framework/treedocument/ISerializeable.h>
#include <framework/primitive/MInt.h>
#include <framework/primitive/MFloat.h>

#include <mmsystem.h>

#include "MFxRack.h"
#include "MSong.h"
#include "MTimeController.h"
#include "MSystemListener.h"

#include "mixer/MDefaultMixer.h"
#include "file/MFileHandling.h"
#include "file/WaveFileHandling.h"
#include "converter/MXMLConverter.h"
#include "automation/ITargetRegistry.h"
#include "interfaces/IBpmListener.h"

typedef std::vector<IBpmListener*> BpmListenerList;
typedef BpmListenerList::iterator BpmIterator;

class Sequencer :
	public IProcessor,
	public ISystemListener,
	public ISerializeable,
	public IBpmControl
{
public:

	static const unsigned int SYSTEM_STEPS;
	static const FP SYSTEM_SHUFFLE[];

protected:

	unsigned int				ivBpm,
						ivCurStep,
						ivStepWidth;

	FP					ivShuffleValue; // from 0.0 to 1.0

	MSong*				ivPtSong;
	MTimeController*	ivPtTimeController;
	MDefaultMixer*		ivPtMixer;

	BpmListenerList		ivBpmListeners;

public:

	Sequencer();
	~Sequencer();

	unsigned int				getStepsCurrentSample();

	void				newSong();

	void				play();
	void				stop();

	void				setSong( MSong* ptSong );
	MSong*				getSong();

	MDefaultMixer*		getMixer();

	void				setTimeController( MTimeController* ptController );
	MTimeController*	getTimeController();

	void				setShuffle(FP value);
	FP					getShuffle();

	void				setBpm( unsigned int value );
	unsigned int		getBpm();

	virtual void		systemStepChanged( int currentStep );
	virtual void		systemPatternChanged( int currentPattern );

	/**
	 * loads from the given tree node
	 */
	virtual void load( MTreeNode* ptNode );

	/**
	 * stores as tree node
	 */
	virtual MTreeNode* save();

	virtual void		addBpmListener( IBpmListener* ptListener );
	virtual void		removeBpmListener( IBpmListener* ptListener );

	virtual void		goNext( MSoundBuffer* pBuffer, unsigned int start, unsigned int stop );
	virtual void		processEvent( MEvent* pEvent );

	int					getStepwidth();
	int					getStepsPerBar();

	void				triggerNoteOn();

	void				triggerNoteOff( int patternStep, int noteStep );
	void				triggerFallDown( int patternStep, int noteStep );

	void				triggerNoteOn( int instrumentNo, int patternStep, int noteStep );
	void				triggerNoteOff( int instrumentNo, int patternStep, int noteStep );
	void				triggerFallDown(  int instrumentNo, int patternStep, int noteStep );
	void				triggerReset();

	void				triggerOfflineUpdate();

protected:

	void				fireBpmChanged();

	bool				render(int startPattern, int count, String fileName);

	bool				hasOverlappingBar( unsigned int instrumentNumber, unsigned int pattern );

	FP					getCurrentWidth( int systemStep, FP shuffle );
};

#endif

