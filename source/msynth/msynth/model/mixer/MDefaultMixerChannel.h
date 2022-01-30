#ifndef __MDefaultMixerChannel
#define __MDefaultMixerChannel

#pragma warning(disable:4786)

#include <framework/MObject.h>
#include <framework/MUtil.h>
#include <framework/xml/MXmlUtil.h>
#include <framework/listener/MListenerList.h>

#include "MMixerChannel.h"

#include "../transformer/MPeakEq.h"
#include "../control/MFloatControl.h"
#include "../control/MBoolControl.h"
#include "../control/MStringControl.h"

#include "../util/MEnvelopeFollower.h"

/**
 * implementation of a IMixerChannel with some base features
 */
class MDefaultMixerChannel :
	public MObject,
	public IMixerChannel,
	public IControlCollection
{
public:

	/**
	 * runtime support
	 */
	static MRtti gvRtti;

	/**
	 * the number of send effects a channel can have
	 */
	static unsigned int	DEFAULT_FX_SEND_COUNT;

	/**
	 * enumeration defining all controls a MDefaultMixerChannel has
	 */
	enum MixerChannelControls
	{
		NAME,
		VOLUME,
		PANORAMA,
		MUTE,
		EQON,
		LOW,
		HIGH,
		FXSEND
	};

protected:

	/** 
	 * system internal offline flag,
	 * used for unglupping streams
	 * if no events can be received.
	 */
	bool				ivOffline;

	/**
	 * instance of a equalizer fort eqing
	 */
	MPeakEq*			ivPtEq;

	/**
	 * control defining the mute state, if muted,
	 * the soundbuffer in goNext is not changed.
	 */
	MBoolControl*		ivMuteControl;

	/**
	 * defining the state of the equalizer,
	 * if on, the eq is used, else not.
	 */
	MBoolControl*		ivEqControl;

	/**
	 * control defining the volume of the channel
	 */
	MFloatControl*		ivVolControl;

	/**
	 * control defininig the stereo panning factor
	 */
	MFloatControl*		ivPanControl;

	/**
	 * some control defining the amount of send effect
	 * routed to the effect rack.
	 */
	MFloatControl**		ivFxSendControls;

	/**
	 * name control for a channel's name
	 */
	MStringControl*		ivNameControl;

	/**
	 * list of finalize listeners to be informed
	 * about termination.
	 */
	MListenerList		ivFinalizeListeners;

	/**
	 * the channels envelope follower
	 */
	MEnvelopeFollower ivEnvelopeFollower;

public:

	/**
	 * constructor
	 */
	MDefaultMixerChannel();

	/**
	 * destructor
	 */
	virtual ~MDefaultMixerChannel();

	virtual unsigned int getFxSendCount();
	virtual FP			getFxSend( unsigned int index );
	virtual void		setFxSend( unsigned int index, FP value );

	// string to display
	virtual String		getChannelName();
	virtual void		setChannelName( String name );

	// to mute a channel
	virtual void		setMute( bool isMute );
	virtual bool		getMute(); 

	// to set the volume of a channel
	virtual void		setVolume( FP volume );
	virtual FP			getVolume();

	// to set the panorama of a channel
	virtual void		setPanorama( FP panorama );
	virtual FP			getPanorama();

	virtual bool		getEqOn();
	virtual void		setEqOn( bool value );

	virtual MPeakEq*	getEq();

	/**
	 * sets the offline state of the channel
	 * (system internal, not mute)
	 * ( inheritanced from IMixerChannel)
	 */
	virtual void setOffline( bool offline );

	/**
	 * returns the offline state of the channel
	 * (system internal, not mute)
	 * ( inheritanced from IMixerChannel)
	 */
	virtual bool getOffline();

	/**
	 * returns the channels envelope follower
	 */
	virtual IEnvelopeFollower* getEnvelopeFollower();

	virtual void		goNext( MSoundBuffer* buffer, unsigned int startFrom, unsigned int stopAt );

	virtual void		addFinalizeListener( IFinalizeListener* ptListener );
	virtual void		removeFinalizeListener( IFinalizeListener* ptListener );

	virtual unsigned int getControlCount();
	virtual IControl* getControl( unsigned int );
	virtual IControl* getControlByName( String name );

    virtual String getName();
	virtual String getType();
	virtual void registerTargets( IAutomationTracker* tracker );
	virtual IControlCollection* getChild( unsigned int index );
	virtual unsigned int getChildCount();

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * querries for a specified interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * returns a string representation
	 */
	virtual String toString();

protected:

	virtual void		fireDestroy();
};


#endif
