/*

  MDefaultMixer (C)2001 MRoc hifiShock

  A mixer is a collection of channels.
  A channel can be added, removed and the number of
  channels is given.
  Because a Mixer has a Master-Section, a Mixer inheritances
  from MMixerChannel and so its a MixerChannel too.

  Dont delete MMixerChannels, only Controls!!!

*/

#include "../mixer/MDefaultMixer.h"
#include "../interfaces/MSoundBufferUtils.h"

#include <framework/performance/MClock.h>

INIT_RTTI( MDefaultMixer, "MDefaultMixer" );

/**
 * constructor
 */
MDefaultMixer::MDefaultMixer()
	: MDefaultMixerChannel(),
	ivPtSong( 0 ),
	ivPtFxRack( 0 )
{
	setChannelName( String( "Master" ) );
}

/**
 * destructor
 */
MDefaultMixer::~MDefaultMixer()
{
	if( ivPtSong )
	{
		ivPtSong->removeSongListener( this );
		ivPtSong = 0;
	}
	ivMixerChannels.clear();
}

/**
 * Builds the model from the song
 */
void MDefaultMixer::buildUpModel( bool sendNotify )
{
	ASSERT( ivPtSong != 0 );

	removeAllMixerChannels();

	unsigned int count = ivPtSong->getBarContainerCount(); 
	for( unsigned int i=0;i<count;i++ )
		addMixerChannel( ivPtSong->getBarContainer( i )->getInstrument() );

	if( sendNotify )
		fireMixerModelChanged();
}

/** 
 * sets the song and adds this mixer as songlistener
 */
void MDefaultMixer::setSong( MSong* ptSong )
{
	if( ivPtSong )
		ivPtSong->removeSongListener( this );

	ivPtSong = ptSong;

	if( ivPtSong )
		ivPtSong->addSongListener( this );

	buildUpModel( true );
}

/**
 * returns the song and adds this mixer as songlistener
 */
MSong* MDefaultMixer::getSong()
{
	return ivPtSong;
}

/**
 * Current number of added Channels
 */
unsigned int MDefaultMixer::getChannelCount()
{
	return ivMixerChannels.size();
}

/**
 * Pointer to a channel
 */
IMixerChannel *MDefaultMixer::getMixerChannel( unsigned int index )
{
	ASSERT( index >= 0 && index < ivMixerChannels.size() );
	return ivMixerChannels[ index ];
}

/**
 * Add a channel
 */
void MDefaultMixer::addMixerChannel( IMixerChannel *ptMixerChannel )
{
	ivMixerChannels.push_back( ptMixerChannel );
}

/**
 * remove a channel by index
 */
void MDefaultMixer::removeMixerChannel( unsigned int index )
{
	ASSERT( index >= 0 && index < ivMixerChannels.size() );
	ivMixerChannels.erase( ivMixerChannels.begin() + index );
}

/**
 * remove the given channel
 */
void MDefaultMixer::removeMixerChannel( IMixerChannel *ptMixerChannel )
{
	unsigned int count = ivMixerChannels.size();
	for( unsigned int i=0; i<count; i++ )
		if( ivMixerChannels[i] = ptMixerChannel )
		{
			removeMixerChannel( i );
			break;
		}
}

/** 
 * removes all channels
 */
void MDefaultMixer::removeAllMixerChannels()
{
	ivMixerChannels.clear();
}

/**
 * INSTRUMENT ADDED (Inheritanced from ISongListeners)
 * Called if a instrument was added to the song
 */
void MDefaultMixer::instrumentAdded( unsigned int index )
{
	IMixerChannel* ptChannel = ivPtSong->getBarContainer( index )->getInstrument();
	ivMixerChannels.insert( ivMixerChannels.begin() + index, ptChannel );
	fireChannelAdded( (unsigned int) index );
}

/**
 * INSTRUMENT REMOVED (Inheritanced from ISongListeners)
 * Called if a instrument was removed from the song
 */
void MDefaultMixer::instrumentRemoved( unsigned int index )
{
	ASSERT( index >= 0 && index < ivMixerChannels.size() );
	ivMixerChannels.erase( ivMixerChannels.begin() + index );
	fireChannelRemoved( (unsigned int) index );
}

/**
 * instrumentsSwapped
 */
void MDefaultMixer::instrumentsSwapped( unsigned int index1, unsigned int index2 )
{
	ASSERT( index1 >= 0 && index1 < ivMixerChannels.size() );
	ASSERT( index2 >= 0 && index2 < ivMixerChannels.size() );
	ASSERT( index1 != index2 );

	IMixerChannel* ptTemp = ivMixerChannels[ index1 ];
	ivMixerChannels[ index1 ] = ivMixerChannels[ index2 ];
	ivMixerChannels[ index2 ] = ptTemp;
	fireChannelsSwapped( (unsigned int) index1, (unsigned int) index2 );
}

/**
 * SONG MODEL CHANGED (Inheritanced from ISongListeners)
 * Called if the song-model completely changed
 */
void MDefaultMixer::songModelChanged()
{
	buildUpModel( true );
}

/**
 * SONG MODEL CHANGED (Inheritanced from ISongListeners)
 * Called if the song-model is deleted
 */
void MDefaultMixer::songDestroy()
{
	if( ivPtSong )
		ivPtSong->removeSongListener( this );
	ivPtSong = 0;
}

/**
 * addMixerListener
 */
void MDefaultMixer::addMixerListener( IMixerListener* ptListener )
{
	ivMixerListeners.addListener( ptListener );
}

/**
 * removeMixerListener
 */
void MDefaultMixer::removeMixerListener( IMixerListener* ptListener )
{
	ivMixerListeners.removeListener( ptListener );
}

/**
 * fireMixerModelChanged
 */

void MDefaultMixer::fireMixerModelChanged()
{
	unsigned int count = this->ivMixerListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IMixerListener*)ivMixerListeners.getListener( i ))->onMixerModelChanged();
}

/**
 * fireChannelAdded
 */
void MDefaultMixer::fireChannelAdded( unsigned int index )
{
	unsigned int count = this->ivMixerListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IMixerListener*)ivMixerListeners.getListener( i ))->onMixerChannelAdded( index );
}

/**
 * fireChannelRemoved
 */
void MDefaultMixer::fireChannelRemoved( unsigned int index )
{
	unsigned int count = this->ivMixerListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IMixerListener*)ivMixerListeners.getListener( i ))->onMixerChannelRemoved( index );
}

/**
 * fireChannelsSwapped
 */
void MDefaultMixer::fireChannelsSwapped( unsigned int index1, unsigned int index2 )
{
	unsigned int count = this->ivMixerListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IMixerListener*)ivMixerListeners.getListener( i ))->onMixerChannelSwapped( index1, index2 );
}

/**
 * processes events
 */
void MDefaultMixer::processEvent( MEvent* pEvent )
{
}

/**
 * renders the effect rack
 */
void MDefaultMixer::goNext( MSoundBuffer* pBuffer, unsigned int startFrom, unsigned int stopAt )
{
	unsigned int k;

	ASSERT( pBuffer );
	ASSERT( startFrom <= stopAt );

	if( this->getMute() == false )
	{
		MSynchronize cls2( &ivPtFxRack->ivCriticalSection );

		MClock cTotal;
		cTotal.start();

		// calculate how many samples to render
		unsigned int samplesToRun = stopAt - startFrom;

		// create output tmp...
		MSoundBuffer masterBuffer( 2, samplesToRun );

		// create channel sound buffers...
		MSoundBuffer **channelBuffer = new MSoundBuffer*[ getChannelCount() ];
		for( k=0;k<getChannelCount();k++ )
			channelBuffer[ k ] = 0;

		// create effect sound buffers...
		MSoundBuffer** fxBuffer = 0;
		if( ivPtFxRack )
		{
			unsigned int fxCount = ivPtFxRack->getSlotCount();
			fxBuffer = new MSoundBuffer*[ fxCount ];
			for( unsigned int j=0;j<fxCount;j++ )
				fxBuffer[ j ] = 0;
		}
	
		MClock c0;
		c0.start();
		// render instruments and route into effect buffer if nescessary...
		for( k=0;k<getChannelCount();k++ )
		{
			// render channel
			IMixerChannel* pChannel = getMixerChannel( k );
			if( pChannel->getMute() == false && 
				pChannel->getOffline() == false )
			{
				//MClock singleChannelClock;
				//singleChannelClock.start();

				// allocating mem...
				//MClock memClock;
				//memClock.start();
				channelBuffer[ k ] = new MSoundBuffer( pChannel->getChannelCount(), samplesToRun );
				//memClock.stop();

				// render instrument...
				//MClock goNextClock;
				//goNextClock.start();
				pChannel->goNext( channelBuffer[ k ], 0, samplesToRun );
				//goNextClock.stop();

				// equing...
				//MClock eqClock;
				//eqClock.start();
				if( ((MDefaultMixerChannel*)pChannel)->getEqOn() )
				{
					((MDefaultMixerChannel*)pChannel)->getEq()->goNext(
						channelBuffer[ k ],
						0,
						channelBuffer[ k ]->getDataLength() );
				}
				//eqClock.stop();

				// mix into effect buffer if nescessary (pre fade listening)...
				//MClock fxSendClock;
				//fxSendClock.start();
				if( ivPtFxRack )
				{
					unsigned int fxCount = ivPtFxRack->getSlotCount();
					for( unsigned int j=0;j<fxCount;j++ )
						if( ivPtFxRack->getSlot( j )->getTransformer() && 
							pChannel->getFxSend( j ) != 0.0 )
						{
							if( ! fxBuffer[ j ] )
								fxBuffer[ j ] = new MSoundBuffer( 2, samplesToRun );
							MSoundBufferUtils::add( fxBuffer[ j ], channelBuffer[ k ], pChannel->getFxSend( j ) );
						}
				}
				//fxSendClock.stop();

				// fading...
				//MClock fadeClock;
				//fadeClock.start();
				MSoundBufferUtils::multiply(
					channelBuffer[ k ],
					0,
					channelBuffer[k]->getDataLength(),
					pChannel->getVolume() );
				//fadeClock.stop();

				// envelope follower...
				//MClock envClock;
				//envClock.start();
				pChannel->getEnvelopeFollower()->goNext(
					channelBuffer[ k ],
					0,
					channelBuffer[ k ]->getDataLength() );
				//envClock.stop();

				// trace...
				//singleChannelClock.stop();
				/*TRACE(
					"\tinstrument complete(%fms) alloc(%fms) gonext(%fms) eq(%fms) fxSend(%fms) fade(%fms) env(%fms)\n",
					singleChannelClock.getLastTimeSpan(),
					memClock.getLastTimeSpan(),
					goNextClock.getLastTimeSpan(),
					eqClock.getLastTimeSpan(),
					fxSendClock.getLastTimeSpan(),
					fadeClock.getLastTimeSpan(),
					envClock.getLastTimeSpan() );*/
			}
		}
		c0.stop();

		MClock c1;
		c1.start();
		// master mix...
		for( k=0;k<getChannelCount();k++ )
			if( channelBuffer[ k ] )
			{
				IMixerChannel* pChannel = getMixerChannel( k );
				MSoundBufferUtils::addStereo(
					&masterBuffer,
					channelBuffer[ k ],
					1.0f,
					pChannel->getPanorama() );
			}
		c1.stop();

		MClock c2;
		c2.start();
		// render effects and mix to master buffer...
		if( ivPtFxRack )
		{
			unsigned int fxCount = ivPtFxRack->getSlotCount();
			for( unsigned int j=0;j<fxCount;j++ )
				if( fxBuffer[ j ] &&
					ivPtFxRack->getSlot( j )->getTransformer() )
				{
					ivPtFxRack->getSlot( j )->getTransformer()->goNext( fxBuffer[ j ], 0, fxBuffer[ j ]->getDataLength() );
					MSoundBufferUtils::add( &masterBuffer, fxBuffer[ j ], 0.5 );
					delete fxBuffer[ j ];
				}
			delete fxBuffer;
		}
		c2.stop();

		// master equing...
		if( this->getEqOn() )
			this->ivPtEq->goNext( &masterBuffer, 0, samplesToRun );

		//TRACE( "MDefaultMixer::goNext max=%f\n", MSoundBufferUtils::findMax( &masterBuffer ) );

		// master eqing...
		MDefaultMixerChannel::goNext( &masterBuffer, 0, samplesToRun );

		// master fading & panning...
		MSoundBufferUtils::steroVolume( &masterBuffer, getVolume(), getPanorama() );

		// output...
		MSoundBufferUtils::pasteBuffer( pBuffer, &masterBuffer, startFrom, samplesToRun );
		//pBuffer->pasteBuffer( &masterBuffer, startFrom, samplesToRun );

		MClock envClock;
		envClock.start();
		// follower...
		ivEnvelopeFollower.goNext( &masterBuffer, 0, masterBuffer.getDataLength() );
		envClock.stop();

		// delete channel sound buffers...
		for( k=0;k<getChannelCount();k++ )
			if( channelBuffer[ k ] )
				delete channelBuffer[ k ];
		delete channelBuffer;

		cTotal.stop();

		/*TRACE( "tps(%f) mixer total: %f rendering: %f msec mixing: %f msec effects: %f envfollower: %f msec samplesToRun: %u\n",
			cTotal.getLastTimeSpan() / (float)samplesToRun,
			cTotal.getLastTimeSpan(),
			c0.getLastTimeSpan(),
			c1.getLastTimeSpan(),
			c2.getLastTimeSpan(),
			envClock.getLastTimeSpan(),
			samplesToRun );*/
	}
}

/**
 * sets a optional fx rack
 */
void MDefaultMixer::setFxRack( MFxRack* pFxRack )
{
	ivPtFxRack = pFxRack;
}

/**
 * returns the optional fx rack,
 * can be null.
 */
MFxRack* MDefaultMixer::getFxRack()
{
	return ivPtFxRack;
}

/**
 * fires notification, e.g. the peak bar
 */
void MDefaultMixer::fireNotifications()
{
	unsigned int count = this->getChannelCount();
	for( unsigned int i=0;i<count;i++ )
	{
		this->ivMixerChannels[ i ]->getEnvelopeFollower()->fireUpdate();
	}
	ivEnvelopeFollower.fireUpdate();
}

void* MDefaultMixer::createInstance()
{
	return new MDefaultMixer();
}