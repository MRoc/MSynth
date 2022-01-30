/*(C)2000 MRoc hifiShock*/

#ifndef __MCLIPPER
#define __MCLIPPER

#include "MTransformer.h"

#define CLIP_MAX 2.0f 

class MClipper :
	public MTransformer
{
public:

	static MRtti gvRtti;

protected:

	enum ClipperControls
	{
		CUT
	};

	MFloatControl* ivPtTresholdControl;

public:

	MClipper();

	virtual ~MClipper();

	virtual unsigned int getControlCount();

	virtual IControl* getControl( unsigned int index );

	virtual void goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt );

	virtual String getType();

	virtual String	getName();

	virtual String	getShortName();

	virtual IRtti* getRtti() const;

	static MObject *createInstance();
};

#endif