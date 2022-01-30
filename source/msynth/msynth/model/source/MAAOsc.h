#ifndef __MAAOsc
#define __MAAOsc

#include "MOsc.h"
#include <framework/primitive/MArray.h>

/**
 * simple wave sample
 */
class MWavetable
{
protected:

	/**
	 * the embedded sample data
	 */
	MFloatArray ivWaveData;

	/**
	 * the play position in sample
	 */
	FP ivPlayPos;

	/** 
	 * the delta per step
	 */
	FP ivPitch;

	/**
	 * the current frequency
	 */
	FP ivSampleFrequence;

	/**
	 * to sync
	 */
	MCriticalSection ivSync;

public:

	/**
	 * constructor
	 */
	MWavetable();

	/**
	 * destructor
	 */
	virtual ~MWavetable();

	/**
	 * string filename
	 */
	virtual void load( String fileName );

	/**
	 * returns the sample data
	 */
	virtual FP* getData();

	/**
	 * returns the sample data length
	 */
	virtual unsigned int getDataLength();

	/**
	 * resets state
	 */
	virtual void reset();

	/**
	 * sets the frequency
	 */
	virtual void setFreq(FP freq);

	/**
	 * adds pitch
	 */
	virtual void addPitch(FP delta);

	/**
	 * sets the phase (0.0-1.0)
	 */
	virtual void setPhase( FP phase );

	/**
	 * renders the osc
	 */
	virtual void goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt );

};

/**
 * anti aliased wave player osc
 */
class MAAOsc :
	public MOsc
{
public:

	/** 
	 * the runtime type info
	 */
	static MRtti gvRtti;

	enum
	{
		MODE = 4
	};

protected:

	/**
	 * string for mode selection
	 */
	static const char *MODE_STRINGS[];

	/**
	 * values for mode selection
	 */
	static const int MODE_VALUES[];

	/** 
	 * mode select control, *, -, original
	 */
	MDiscreteComboControl*	ivPtModeControl;

	/**
	 * control to select osc number one
	 */
	MDiscreteComboControl*	ivPtWave1Control;

	/**
	 * control to select osc number two
	 */
	MDiscreteComboControl*	ivPtWave2Control;

	/**
	 * the embedded wavetable
	 */
	MWavetable ivWaveTable1;

	/**
	 * the embedded wavetable
	 */
	MWavetable ivWaveTable2;

	/**
	 * list of available wave file names
	 */
	StrList ivOscFileNames;

	/**
	 * to sync
	 */
	MCriticalSection ivSync;

public:

	/** 
	 * constructor
	 */
	MAAOsc();

	/**
	 * destructor
	 */
	virtual ~MAAOsc();

	/**
	 * returns the number of embedded controls
	 */
	virtual unsigned int getControlCount();

	/**
	 * returns the indexed control
	 */
	virtual IControl* getControl( unsigned int index );

	/**
	 * renders the osc
	 */
	virtual void goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt );

	/**
	 * resets state
	 */
	virtual void reset();

	/**
	 * sets the frequency
	 */
	virtual void setFreq(FP freq);

	/**
	 * adds pitch
	 */
	virtual void addPitch(FP delta);

	/**
	 * returns the osc's name
	 */
	virtual String getName();

	/**
	 * returns the osc's short name
	 */
	virtual	String getShortName();

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * creates an instance of this class
	 */
	static MObject* createInstance();

	/**
	 * inheritanced from IControlListener
	 */
	virtual void valueChanged( MControlListenerEvent *anEvent );
};

#endif