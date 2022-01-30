#ifndef __MDefaultMixer
#define __MDefaultMixer

#include <framework/MTypes.h>
#include <framework/map/MProperties.h>
#include <framework/listener/MListenerList.h>

#include "IMixer.h"
#include "MDefaultMixerChannel.h"

#include "../MSong.h"
#include "../MFxRack.h"

/**
 * interface defining a mixer listener
 */
class IMixerListener :
	public IListener
{
public:

	virtual void onMixerModelChanged() = 0;
	virtual void onMixerChannelAdded( unsigned int index ) = 0;
	virtual void onMixerChannelRemoved( unsigned int index ) = 0;
	virtual void onMixerChannelSwapped( unsigned int index1, unsigned int index2 ) = 0;
};

typedef std::vector<IMixerChannel*> MixerChannelList;

/**
 *
 * MDefaultMixer (c)2001-2003 MRoc
 *
 * default implementation of a multitrack
 * mixer console with a optional fx rack.
 * 
 */
class MDefaultMixer :
	public MDefaultMixerChannel,
	public IMixer,
	public ISongListener
{
protected:

	/**
	 * song required to register as song listener
	 */
	MSong* ivPtSong;

	/**
	 * list of mixer channels
	 */
	MixerChannelList ivMixerChannels;

	/**
	 * listener list
	 */
	MListenerList ivMixerListeners;

	/** 
	 * a optional fx rack
	 */
	MFxRack* ivPtFxRack;

public:

	FULL_RTTI_SUPER( MDefaultMixer,"MDefaultMixer", MDefaultMixerChannel );

	/**
	 * constructor
	 */
	MDefaultMixer();

	/** 
	 * destructor
	 */
	virtual ~MDefaultMixer();

	/**
	 * fires notification, e.g. the peak bar
	 */
	virtual void fireNotifications();

	/**
	 * returns the number of connected channels
	 */
	virtual unsigned int getChannelCount();

	/**
	 * returns the indexed channen
	 */
	virtual IMixerChannel* getMixerChannel( unsigned int index );

	/**
	 * prcess a event
	 * (inheritanced from MDefaultMixerChannel)
	 */
	virtual void processEvent( MEvent* pEvent );

	/**
	 * renders and mixes all connected and
	 * nor mute and offlined channels.
	 */
	virtual void goNext( MSoundBuffer* pBuffer, unsigned int startFrom, unsigned int stopAt );

	/**
	 * builts up the mixer form the songmodel
	 */
	virtual void buildUpModel( bool sendNotify );

	/**
	 * sets the song
	 */
	virtual void setSong( MSong* ptSong );

	/**
	 * returns the song
	 */
	virtual MSong* getSong();

	/**
	 * sets a optional fx rack
	 */
	virtual void setFxRack( MFxRack* pFxRack );

	/**
	 * returns the optional fx rack,
	 * can be null.
	 */
	virtual MFxRack* getFxRack();

	static void* createInstance();

protected:

	/**
	 * adda a channel
	 */
	virtual void			addMixerChannel( IMixerChannel *ptMixerChannel );

	/**
	 * removes a channel by index
	 */
	virtual void			removeMixerChannel( unsigned int index );

	/**
	 * removes a channel
	 */
	virtual void			removeMixerChannel( IMixerChannel *ptMixerChannel );

	/**
	 * removes all channels
	 */
	virtual void			removeAllMixerChannels();

	void					fireMixerModelChanged();
	void					fireChannelAdded( unsigned int index );
	void					fireChannelRemoved( unsigned int index );
	void					fireChannelsSwapped( unsigned int index1, unsigned int index2 );

public:

	// INTERFACE ISongListeners
	virtual void			instrumentAdded( unsigned int index );
	virtual void			instrumentRemoved( unsigned int index );
	virtual void			instrumentsSwapped( unsigned int index1, unsigned int index2 );
	virtual void			songModelChanged();
	virtual void			songDestroy();

	void					addMixerListener( IMixerListener* ptListener );
	void					removeMixerListener( IMixerListener* ptListener );
};

#endif