/*
  (C)2000-2003 MRoc hifiShock

  v. 0.1	design
  v. 0.2	performanceTune(loop, accent out of goNext),
			better resampling
*/

#ifndef __MWAVEPLAYER
#define __MWAVEPLAYER

#include <framework/sync/MSynchronize.h>
#include "MOsc.h"
#include "../MWave.h"

/**
 * implementation of a wave player for a beatbox
 */
class MWavePlayer :
	public MObject,
	public ISerializeable,
	public IControlListener,
	public IControlCollection
{
public:

	/**
	 * runtime support
	 */
	static MRtti gvRtti;

	/**
	 * enumeration of all used controls
	 */
	enum WAVE_PLAYER_CONTROLS
	{
		SAMPLENAME,
		VOLUME,
		PITCH,
		PAN,
		LOOP,
		REV,
		MUTE,
		EXCLUSIVE,
		ID,
		ISVALID
	};

protected:

	int					ivId;

	MWave*				ivPtWave;
	bool				ivReversed;

	unsigned int		ivBpm;

	MStringControl*		ivPtNameControl;
	MBoolControl*		ivPtLoopControl;
	MBoolControl*		ivPtMuteControl;
	MBoolControl*		ivPtRevControl;
	MBoolControl*		ivPtValidControl;
	MBoolControl*		ivPtExclusiveControl;
	MFloatControl*		ivPtVolControl;
	MRangeControl*		ivPtPitchControl;
	MFloatControl*		ivPtPanControl;

	bool				ivIsOn,
						ivIsFallDown;

	FP					ivNoteVolume,
						ivFallDown,
						ivPlayPos;

	MCriticalSection	ivCriticalSection;

public:

	MWavePlayer();
	MWavePlayer(int id);
	MWavePlayer(int id, unsigned int bpm, IAutomationTracker* pTracker );
	virtual ~MWavePlayer();

	void init();

	void setWave(MWave *wave);
	MWave *getWave();

	void goNext( MSoundBuffer *buffer, unsigned int renderFrom, unsigned int renderTo );

	void resetPlayState();
	void resetSample();

	void setOn( bool value );
	bool getOn();

	void setFallDown(bool value);
	bool getFallDown();

	bool getExclusive();
	bool getMute();
	bool getLoop();

	void setNoteVolume( FP noteVolume );
	FP getNoteVolume();

	void setBpm(unsigned int bpm);
	unsigned int getBpm();

	void loadFile(const char *fileName);
	void saveFile(const char *fileName);

	bool isValid();

	String getSampleName();
	void setSampleName( String name );

	virtual void valueChanged( MControlListenerEvent *anEvent );

	virtual unsigned int getControlCount();
	virtual IControl* getControl( unsigned int index );
	virtual IControl* getControlByName( String name );

	virtual unsigned int getChildCount();
	virtual IControlCollection* getChild( unsigned int index );

	virtual void registerTargets( IAutomationTracker* tracker );

	MBoolControl* getValidControl();

	virtual String getType();
	virtual String getName();
	virtual String getShortName();
	
	virtual int getViewId();
	
	virtual MTreeNode* save();
	virtual void load( MTreeNode* node );

public: // MObject

	/**
	 * returns a string representation
	 */
	virtual String toString();

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * querries for a specified interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * returns a instance of the waveplayer
	 */
	static MObject* createInstance();
};

#endif
