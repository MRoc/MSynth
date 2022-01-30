#ifndef __MAnalogSquare
#define __MAnalogSquare

#include "MOsc.h"
#include "MOscSine.h"

class MAnalogSquare :
	public MOsc
{
public:

	static MRtti gvRtti;

protected:

	unsigned int ivSineCount;
	MOscSine** ivPtSines;

public:


	MAnalogSquare();

	virtual ~MAnalogSquare();

	virtual void		goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt );
	virtual void		reset();

	virtual void		setFreq( FP freq );
	virtual void		addPitch( FP delta);
	
	virtual String		getName();
	virtual String		getShortName();

	virtual IRtti*		getRtti() const;
	static MObject*		createInstance();

	virtual unsigned int	getControlCount();
	virtual IControl*		getControl( unsigned int index );

};

#endif