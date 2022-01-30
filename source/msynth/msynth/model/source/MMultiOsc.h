/*

	MMultiOsc (C)2001 MRoc hifiShock

	a synchronized oscilator containing other oscilators.

*/
#ifndef __MMULTIOSC
#define __MMULTIOSC

#include <framework/MDefaultFactory.h>
#include <framework/MException.h>
#include <framework/sync/MSynchronize.h>

#include "MOsc.h"
#include "MOscSaw.h"

/**
 * multioscilator holding 1-3 sub oscilators
 */
class MMultiOsc :
	public MOsc
{
public:

	/**
	 * the runtime type info
	 */
	static MRtti			gvRtti;

protected:

	unsigned int			ivOscCount;
	MOsc**					ivOsc;

	IAutomationTracker*		ivPtTracker;

	MCriticalSection		ivCriticalSection;
		
public:

	/**
	 * constructor
	 */
	MMultiOsc();

	/**
	 * destructor
	 */
	virtual ~MMultiOsc();

	virtual void			deleteContent();

	virtual void			goNext( MSoundBuffer *buffer,
									unsigned int startFrom,
									unsigned int stopAt );					

	virtual void			setFreq( FP freq );

	virtual void			addPitch( FP delta );

	virtual void			triggerPortamento( FP targetFrequence, unsigned int portamentoLength );
	virtual void			stopPortamento();

	virtual void			reset();

	void					setOscCount( int oscCount );
	unsigned int			getOscCount();

	void					setOscAt( MOsc* ptOsc, int index );
	MOsc*					getOscAt( int index );

	virtual String			getName(); // MMultiOsc
	virtual String			getShortName(); // MultiOsc

	virtual MTreeNode*		save();
	virtual void			load( MTreeNode* node );

	virtual void			registerTargets( IAutomationTracker* ptTracker );

	virtual int				getViewId();

	/**
	 * returns the runtime type info
	 */
	virtual IRtti*			getRtti() const;

	/**
	 * returns a instance of this class
	 */
	static MObject*			createInstance();
};

#endif
