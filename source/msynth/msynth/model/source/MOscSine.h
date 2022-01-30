/**
 * (C) 2002-2003 MRoc
 */

#ifndef __MOSCSINE
#define __MOSCSINE

#include "MOsc.h"

class MOscSine : public MOsc
{
public:

	static MRtti		gvRtti;

protected:

	double				ivSin, ivCos, ivX, ivY;
						
public:

	MOscSine();

	virtual ~MOscSine();

	virtual void		goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt );
	virtual void		reset();

	virtual void		setFreq( FP freq );
	virtual void		addPitch( FP delta);
	
	virtual String		getName();
	virtual String		getShortName();

	virtual IRtti*		getRtti() const;
	static MObject*		createInstance();
};

#endif
