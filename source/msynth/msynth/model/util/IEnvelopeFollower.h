#ifndef __IEnvelopeFollower
#define __IEnvelopeFollower

#include <framework/listener/IListener.h>
#include "../interfaces/MSoundBuffer.h"

/**
 * interface defining a listener to
 * be informed about envelope changes
 * from the IEnvelopeFollower.
 */
class IEnvelopeFollowerListener :
	public IListener
{
public:

	/**
	 * invoked when the envelope changed
	 */
	virtual void onEnvelopeChanged( FP curEnvelope ) = 0; 
};

/**
 * interface defining a envelope follower
 */
class IEnvelopeFollower
{
public:

	/**
	 * calculates the next peak
	 */
	virtual void goNext( MSoundBuffer* pBuffer, unsigned int start, unsigned int stop ) = 0;

	/** 
	 * adds a listener to the enveloper
	 */
	virtual void addListener( IEnvelopeFollowerListener* pListener ) = 0;

	/** 
	 * removes a listener from the enveloper
	 */
	virtual void removeListener( IEnvelopeFollowerListener* pListener ) = 0;

	/**
	 * when invoked, the envelope follower fires
	 * a change notification.
	 */
	virtual void fireUpdate() = 0;
};

#endif