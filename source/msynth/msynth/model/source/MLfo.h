#ifndef __LFO
#define __LFO

#include <framework/MTypes.h>

#include "ILfo.h"
#include "../../../MSynthConstants.h"
#include "../control/IControl.h"
#include "../control/MBoolControl.h"
#include "../control/MFloatControl.h"
#include "../control/IControlCollection.h"

class LFO :
	public MObject,
	public ILfo,
	public IControlCollection,
	public IControlListener
{
public:

	enum LfoControls
	{
		LFOON,
		SYNC,
		FREQUENCY,
		BANDWIDTH,
		PHASE
	};

protected:

	MBoolControl*		ivPtOnControl;
	MBoolControl*		ivPtSyncControl;
	MFloatControl*		ivPtFreqControl;
	MFloatControl*		ivPtWidthControl;
	MFloatControl*		ivPtPhaseControl;

	double				ivSin, ivCos, ivCurrentSin, ivCurrentCos;

public:

	FULL_RTTI( LFO, "LFO" );

	LFO();
	virtual ~LFO();

	virtual String toString();

	virtual IControlCollection* getChild( unsigned int index );

	virtual unsigned int getChildCount();
	virtual unsigned int getControlCount();
	virtual IControl* getControl( unsigned int index );

	virtual bool getLfoState();
	virtual void setLfoState( bool value );

	virtual void setLfoBandWidth( FP bandWidth );
	virtual FP getLfoBandWidth();

	virtual void setLfoFrequence( FP frequence );
	virtual FP getLfoFrequence();

	virtual void goNext( FP* ptData, unsigned int startAt, unsigned int stopAt );
	virtual void reset();

	virtual void valueChanged( MControlListenerEvent *anEvent );

	virtual String getType();
	virtual String getName();

	virtual void registerTargets( IAutomationTracker* tracker );
	virtual IControl* getControlByName( String name );
};

#endif