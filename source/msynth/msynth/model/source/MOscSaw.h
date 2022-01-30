/**
 * (C) 2000-2003 MRoc
 */

#ifndef __MOSCSAW
#define __MOSCSAW

#include "MOsc.h"

class MOscSaw : public MOsc
{
public:

	static MRtti		gvRtti;

protected:

	FP					d_osc,
						d_delta;
						
public:
	MOscSaw();

	virtual				~MOscSaw();

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
