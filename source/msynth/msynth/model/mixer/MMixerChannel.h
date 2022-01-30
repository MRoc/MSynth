#ifndef __MMixerChannel
#define __MMixerChannel

#include <framework/MTypes.h>
#include <framework/listener/IFinalizeListener.h>

#include "../interfaces/IProcessor.h"
#include "../util/IEnvelopeFollower.h"

/**
 *
 * MMixerChannel (C)2001-2003 MRoc hifiShock
 *
 * Abstract base class of a mixerchannel.
 * with a name and a volume, pan and mute control.
 *
 * it is also a processor able to receive events
 * and to render audio.
 *
 * a mixer channel can also set in a offline
 * mode. if a channel is offline, it is not 
 * rendered.
 *
 */
class IMixerChannel :
	public IProcessor
{
public:
	/**
	 * returns the user readable name of the instrument
	 */
	virtual String getChannelName() = 0;

	/**
	 * sets the user readable name of the instrument
	 */
	virtual void setChannelName( String name ) = 0;

	/** 
	 * mutes/unmutes the channel
	 */
	virtual void setMute( bool isMute ) = 0;

	/** 
	 * returns the mute state
	 */
	virtual bool getMute() = 0; 

	/** 
	 * sets the volume of the channel
	 */
	virtual void setVolume( FP volume ) = 0;

	/** 
	 * returns the volume
	 */
	virtual FP getVolume() = 0;

	/**
	 * sets the panorama of the channel
	 */
	virtual void setPanorama( FP panorama ) = 0;

	/** 
	 * returns the panorama of the channel
	 */
	virtual FP getPanorama() = 0;

	/**
	 * returns the fx send for the given effect route
	 */
	virtual FP getFxSend( unsigned int index ) = 0;

	/**
	 * sets the offline state of the channel
	 * (system internal, not mute)
	 */
	virtual void setOffline( bool offline ) = 0;

	/**
	 * returns the offline state of the channel
	 * (system internal, not mute)
	 */
	virtual bool getOffline() = 0;

	/**
	 * returns the channels envelope follower
	 */
	virtual IEnvelopeFollower* getEnvelopeFollower() = 0;

	/** 
	 * adds a finalisation listener to the channel
	 */
	virtual void addFinalizeListener( IFinalizeListener* ptListener ) = 0;

	/** 
	 * removes a finalisation listener to the channel
	 */
	virtual void removeFinalizeListener( IFinalizeListener* ptListener ) = 0;

	/**
	 * returns the number of input channels, 1=mono 2=stereo
	 */
	virtual unsigned int getChannelCount() = 0;

	/**
	 * fire a event into the processor
	 * (inheritanced from IProcessor)
	 */
	virtual void processEvent( MEvent* ptEvent ) = 0;

	/**
	 * processes the given data
	 * (inheritanced from IProcessor)
	 */
	virtual void goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt) = 0;
};

#endif