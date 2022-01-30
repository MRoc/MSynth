/*(C)2000-2003 MRoc hifiShock*/

#ifndef __MFuzz
#define __MFuzz

#include "MTransformer.h"

class MFuzz :
	public MTransformer,
	public IControlListener
{
public:

	static MRtti gvRtti;

	enum FUZZ_PARAM
	{
		MIX,
		BOOST
	};

protected:

	MFloatControl* ivPtMixControl;
	MFloatControl* ivPtBoostControl;

	FP ivTreshold;
	FP ivBoost;

public:

	MFuzz();
	virtual ~MFuzz();

	virtual unsigned int getControlCount();
	virtual IControl* getControl( unsigned int index );

	virtual void goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt );

	virtual void valueChanged( MControlListenerEvent *anEvent );

	virtual String getName();
	virtual String getShortName();

	virtual IRtti* getRtti() const;
	virtual void* getInterface( const String &className ) const;
	static MObject *createInstance();

private:

	void updateBoostCoeffs();
};

#endif
