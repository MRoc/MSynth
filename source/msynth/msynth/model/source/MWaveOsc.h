/**
 * MWaveOsc (C)2000-2003 MRoc hifiShock
 */

#ifndef __MWAVEOSC
#define __MWAVEOSC

#include <framework/sync/MSynchronize.h>

#include "MOsc.h"
#include "../../../resource.h"
#include "../MWave.h"
#include "../MWaveRenderer.h"

#define MWAVEOSC_CONTROL_COUNT 6

class MWaveOsc :
	public MOsc 
{
public:

	static MRtti		gvRtti;

	enum WAVEOSC_PARAM
	{
		OCTAVE,
		VOLUME,
		TUNE,
		LOOP,
		REVERSE,
		SAMPLENAME
	};

protected:

	MWave				*ivPtWave;

	bool				ivIsOn,
						ivIsReversed;

	MStringControl*		ivPtSampleNameControl;
	MBoolControl*		ivPtReverseControl;
	MBoolControl*		ivPtLoopControl;

	FP					ivPlayPos,
						ivPitch,
						ivSampleFrequence;

	MCriticalSection	ivCriticalSection;

public:

	MWaveOsc();
	
	~MWaveOsc();

	unsigned int		getControlCount();
	IControl*			getControl( unsigned int index );

	virtual void		valueChanged( MControlListenerEvent *anEvent );

	virtual void		setFreq( FP freq );
	virtual void		addPitch( FP delta );

	virtual void		reset();

	virtual void		goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt );

	void				setWave(MWave *wave);
	MWave*				getWave();

	void				setOn(bool value);
	bool				getOn();

	bool				getReversed();
	void				setReverse(bool value);

	void				setLooped( bool loop );
	bool				getLooped();

	void				saveFile( const char *fileName );

	bool				isValid();

	String				getSampleName();
	void				setSampleName( String sampelName );

	virtual String		getName();
	virtual String		getShortName();

	virtual int			getViewId();

	virtual MTreeNode*	save();
	virtual void		load( MTreeNode* node );

	virtual IRtti*		getRtti() const;
	static MObject*		createInstance();

	void				loadFile( const char *fileName );

protected:

	void				reverse();
};

#endif
