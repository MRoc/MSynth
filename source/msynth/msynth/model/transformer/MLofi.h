/*(C)2000-2003 MRoc hifiShock*/

#ifndef __MLofi
#define __MLofi

#include "MTransformer.h"

class MLofi :
	public MTransformer
{
public:

	static MRtti gvRtti;

	enum LofiControls
	{
		MIX
	};

protected:

	MFloatControl* ivPtMixControl;

public:

	MLofi();

	virtual ~MLofi();

	virtual unsigned int getControlCount();

	virtual IControl* getControl( unsigned int index );

	virtual void goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt );

	virtual String getName();

	virtual String getShortName();

	virtual IRtti* getRtti() const;

	static MObject *createInstance();
};

#endif

