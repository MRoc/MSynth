/*

	MDelay (C)2000/2001 MRoc hifiShock

*/

#ifndef __MDelay
#define __MDelay

#include "MTransformer.h"

class MDelay :
	public MTransformer
{
public:

	static MRtti gvRtti;

	enum DELAY_PARAM
	{
		MIX,
		DELAYTIME,
		FEEDBACK
	};

protected:

	MFloatControl*	ivPtMix;
	MFloatControl*	ivPtFeedback;
	MFloatControl*	ivPtDelayTime;

	FP**			ivPtDelayBuffer;

	unsigned int	ivStep;
	unsigned int	ivBufferLength;

public:

	MDelay();
	virtual ~MDelay();

	virtual bool	create( unsigned int channelCount );

	virtual unsigned int getControlCount();
	virtual IControl* getControl( unsigned int index );

	virtual void	processEvent( MEvent* ptEvent );
	virtual void	goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt );

	void			setDelay( FP delay );
	FP				getDelay();

	void			setFeedback( FP feedback );
	FP				getFeedback();

	virtual String	getName();
	virtual String	getShortName();

	virtual IRtti* getRtti() const;
	static MObject* createInstance();
};

#endif

