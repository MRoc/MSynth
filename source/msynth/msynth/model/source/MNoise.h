/*

	MNoise (C)2000/2001 MRoc hifiShock

*/

#ifndef __MNoise
#define __MNoise

#include <framework/util/MRandomGen.h>

#include "MOsc.h"
#include "../transformer/MIIROrder1.h"

/** 
 * a noise generator with frequency independent noise.
 */
class MNoise : public MOsc
{
public:

	enum NOISE_PARAM
	{
		VOLUME
	};

	static MRtti gvRtti;

protected:

	MRandomGen ivRandomizer;
	MIIROrder1 ivFilter;

public:

	MNoise();
	virtual ~MNoise();

	virtual void			goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt );
	virtual void			reset();

	virtual unsigned int	getControlCount();
	virtual IControl*		getControl( unsigned int index );

	virtual void			setFreq( FP freq );
	virtual void			addPitch( FP delta );
	virtual void			setFineTune( FP tune );
	virtual FP				getFineTune();

	virtual String			getName();
	virtual String			getShortName();

	virtual IRtti*			getRtti() const;
	static MObject*			createInstance();		
};

#endif


