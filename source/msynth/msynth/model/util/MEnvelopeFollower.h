#ifndef __MEnvelopeFollower
#define __MEnvelopeFollower


#include <framework/listener/MListenerList.h>
#include "../interfaces/MSoundBuffer.h"
#include "IEnvelopeFollower.h"

/**
 * default implementation of a envelope follower
 */
class MEnvelopeFollower :
	public IEnvelopeFollower
{
protected:

	FP ivBUp;
	FP ivBDown;
	FP ivPeak;

	/**
	 * list of listeners
	 */
	MListenerList ivListeners;

public:

	/** 
	 * constructor
	 */
	MEnvelopeFollower();

	/**
	 * destructor
	 */
	virtual ~MEnvelopeFollower();

	/** 
	 * does a envelope follow
	 */
	virtual void goNext( MSoundBuffer* pBuffer, unsigned int start, unsigned int stop );

	/** 
	 * adds a listener to the enveloper
	 */
	virtual void addListener( IEnvelopeFollowerListener* pListener );

	/** 
	 * removes a listener from the enveloper
	 */
	virtual void removeListener( IEnvelopeFollowerListener* pListener );

	/**
	 * when invoked, the envelope follower fires
	 * a change notification.
	 */
	virtual void fireUpdate();

private:

	/**
	 * fires a envelope changed event
	 */
	void fireEnvelopeChanged( FP value );
};

#endif