/**
 * (C)2000-2003 MRoc hifiShock
 */

#ifndef __MOSCSQUARE
#define __MOSCSQUARE

#include "MOsc.h"

#define MSQUARE_CONTROL_COUNT 1

class MOscSquare : public MOsc
{
public:

	static MRtti		gvRtti;

	enum
	{
		WIDTH = 4
	};

protected:

	FP				d_osc,
					d_step,
					d_sqtune;

	MFloatControl*	ivPtWidthControl;
						
public:

	MOscSquare();

	virtual ~MOscSquare();

	virtual unsigned int getControlCount();
	virtual IControl* getControl( unsigned int index );

	virtual void		goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt );
	virtual void		reset();

	virtual void		setFreq(FP freq);
	virtual void		addPitch(FP delta);
	virtual void		setWidth(FP width);
	virtual FP			getWidth();
	
	virtual String		getName();
	virtual	String		getShortName();

	virtual IRtti*		getRtti() const;
	static MObject*		createInstance();

protected:

	void				fillBuffer( FP* dataTemp, unsigned int startAt, unsigned int stopAt, FP value );
};

#endif
