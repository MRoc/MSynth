/*

  MMixer (C)2001 MRoc hifiShock

  This interface is abstract base class of a mixer.
  A mixer is a collection of channels.
  A channel can be added, removed and the number of
  channels is given.

*/

#ifndef __IMixer
#define __IMixer

#include "MMixerChannel.h"

class IMixer
{
public:
	// Current number of added Channels
	virtual unsigned int getChannelCount() = 0;

	// Pointer to a Channel
	virtual IMixerChannel *getMixerChannel( unsigned int index ) = 0;

	// Add a channel
	virtual void addMixerChannel( IMixerChannel *ptMixerChannel ) = 0;

	// remove a channel
	virtual void removeMixerChannel( unsigned int index ) = 0;
	virtual void removeMixerChannel( IMixerChannel *ptMixerChannel ) = 0;

	// remove all channels
	virtual void removeAllMixerChannels() = 0;
};

#endif


