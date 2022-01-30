/**
 * (C) 2001-2003 MRoc
 */

#ifndef __MOSCSTRING
#define __MOSCSTRING

#include "MOsc.h"

#define SAWCOUNT 7

class MOscString : public MOsc
{
public:

	static MRtti		gvRtti;

	enum Controls
	{
		VOLBASE = 4
	};

protected:

	static const	FP	gvDetune[ SAWCOUNT ];

	FP				d_osc,
					d_delta;

	MFloatControl*	ivPtVolControls[ SAWCOUNT ];

	FP				saw[ SAWCOUNT ];
		
public:

	MOscString();

	~MOscString();

	virtual unsigned int getControlCount();
	virtual IControl* getControl( unsigned int index );

	virtual void		goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt );
	virtual void		reset();

	virtual void		setFreq(FP freq);
	virtual void		addPitch(FP delta);

	virtual String		getName();
	virtual String		getShortName();

	virtual IRtti*		getRtti() const;
	static MObject*		createInstance();
};

#endif
