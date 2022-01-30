/**
 * (C) 2001-2003 MRoc
 */

#ifndef __MOrgan
#define __MOrgan

#include "MOsc.h"
#include "MOscSine.h"

#define SINECOUNT 7

class MOrgan : public MOsc
{
public:

	static MRtti gvRtti;

	enum Controls
	{
		OCTAVE,
		VOLUME,
		TUNE,
		VOLBASE
	};

protected:

	static const FP		gvMulAdder[];

	FP					ivArMultiply[SINECOUNT];
	MOscSine*			ivArPtOsc[SINECOUNT];
	MFloatControl*		ivPtVolControls[SINECOUNT];
						
public:

	MOrgan();
	virtual ~MOrgan();

	virtual unsigned int getControlCount();
	virtual IControl* getControl( unsigned int index );

	virtual void	goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt );
	virtual void	reset();

	virtual void	setFreq(FP freq);
	virtual void	addPitch(FP delta);

	virtual void	updateFromFineTune();

	virtual void	valueChanged( MControlListenerEvent *anEvent );

	virtual String	getName();
	virtual String	getShortName();

	virtual IRtti*	getRtti() const;
	static MObject*	createInstance();
};


#endif

