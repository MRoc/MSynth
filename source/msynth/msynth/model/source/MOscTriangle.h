/**
 * (C) 2000-2003 MRoc
 */

#ifndef __MOSCTRIANGLE
#define __MOSCTRIANGLE

#include "MOsc.h"
#include "../interfaces/MSoundBuffer.h"

class MOscTriangle : public MOsc
{
public:

	static MRtti		gvRtti;

protected:

	FP					d_osc,
						d_delta;
						
public:

	MOscTriangle();

	virtual ~MOscTriangle();

	virtual void goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt );
	virtual void reset();

	virtual void setFreq(FP freq);
	virtual void addPitch(FP delta);

	virtual String	getName();
	virtual String	getShortName();

	virtual IRtti* getRtti() const;
	static MObject *createInstance();
};

#endif
