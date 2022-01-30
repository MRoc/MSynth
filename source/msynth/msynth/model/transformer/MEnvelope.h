/*

	MEnvelope (C)2000-2003 MRoc hifiShock

	Linear ADSR-Envelope-Generator,
	attack, decay, release > 0 !!!

	v. 0.1	-> design
	v. 0.2	-> working
	v. 0.3	-> trigger on / off

*/

#ifndef __MENVELOPE
#define __MENVELOPE

#include "MTransformer.h"

class MEnvelope :
	public MTransformer
{
public:

	static MRtti gvRtti;

	enum ENVELOPE_PARAM
	{
		ATTACK,
		DECAY,
		SUSTAIN,
		RELEASE
	};

protected:

	// controls...
	MFloatControl*	ivPtAttackControl;
	MFloatControl*	ivPtDecayControl;
	MFloatControl*	ivPtReleaseControl;
	MFloatControl*	ivPtSustainControl;

	// definition of state...
	enum EEnvelopeState
	{
		E_ATTACK,	// noteOn
		E_DECAY,	//    "
		E_SUSTAIN,	//    "
		E_RELEASE,  // noteOff
		E_OFF,		//    "
		E_FALLDOWN	// fastFadeOut
	};

	// runtime values...
	EEnvelopeState	ivState;
	unsigned int	ivStep;
	double			ivValue;
	double			ivValueBeginState; // value when beginning new state

public:

	MEnvelope();
	virtual ~MEnvelope();

	virtual void processEvent( MEvent* ptEvent );
	virtual void goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt );

	virtual unsigned int getControlCount();
	virtual IControl* getControl( unsigned int index );

	virtual String getName();
	virtual String getShortName();

	virtual IRtti* getRtti() const;
	static MObject *createInstance();

protected:

	void validateState( unsigned int attack, unsigned int decay, unsigned int release, unsigned int fallDown );
	int	getAttack()	{ return int(ivPtAttackControl->getValue());	};
	int	getDecay()	{ return int(ivPtDecayControl->getValue()); };
	int	getRelease(){ return int(ivPtReleaseControl->getValue()); };
	FP	getSustain(){ return ivPtSustainControl->getValue(); };
};

#endif