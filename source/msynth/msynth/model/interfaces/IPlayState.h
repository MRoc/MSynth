#ifndef __IPlayState
#define __IPlayState

#include <framework/listener/IListener.h>

// possible matrix:

// playing | recording | state
// --------------------------------------------
//    0    |    0      | -> neutral
//    0    |    1      | -> rendering song
//    1    |    0      | -> playing song
//    1    |    1      | -> live session

/**
 * defines an instance storing MSynths play state handling
 */
class IPlayState
{
public:

	/**
	 * state definition
	 */
	enum PlayState
	{
		NEUTRAL,
		PLAYING,
		SESSION_NOT_PLAYING,
		SESSION_PLAYING,
		RENDERING
	};

	/**
	 * returns the current state
	 */
	virtual PlayState getPlayState() = 0;
};

/**
 * defines a play state listener with which objects can
 * be informed about changes in play state
 */
class IPlayStateListener :
	public IListener
{
public:

	/**
	 * invoked when play state changed
	 */
	virtual void onPlayStateChanged( IPlayState::PlayState newState ) = 0;
};


#endif