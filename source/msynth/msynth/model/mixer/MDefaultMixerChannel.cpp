#include "MDefaultMixerChannel.h"
#include "../interfaces/MSoundBufferUtils.h"

unsigned int MDefaultMixerChannel::DEFAULT_FX_SEND_COUNT = 4;

/**
 * runtime support
 */
MRtti MDefaultMixerChannel::gvRtti = MRtti( "MDefaultMixerChannel", NULL );

//-----------------------------------------------------------------------------
// + CONSTRUCTION
//-----------------------------------------------------------------------------
MDefaultMixerChannel::MDefaultMixerChannel() :
	ivOffline( false ),
	ivPtEq( 0 ),
	ivMuteControl( 0 ),
	ivEqControl( 0 ),
	ivVolControl( 0 ),
	ivPanControl( 0 ),
	ivFxSendControls( 0 ),
	ivNameControl( 0 )
{
	ivMuteControl = new MBoolControl( String( "Mute" ), false );
	ivEqControl = new MBoolControl( String( "Eq" ), false );

	ivVolControl = new MFloatControl( String( "Volume" ), 0.0f, 1.5f, 1.0, false, MFloatControl::CURVE_QUAD );
	ivPanControl = new MFloatControl( String( "Pan" ), 0.0f, 1.0f, 0.5, false );

	ivFxSendControls = new MFloatControl*[ DEFAULT_FX_SEND_COUNT ];
	for( unsigned int i=0;i<DEFAULT_FX_SEND_COUNT;i++ )
		ivFxSendControls[ i ] = new MFloatControl(
			String( "Fx" ) + MUtil::toString( i + 1 ),
			0.0f,
			1.0f,
			0.0,
			false,
			MFloatControl::CURVE_QUAD );

	ivNameControl = new MStringControl(
		"channelName",
		"undefined" );
	
	ivPtEq = new MPeakEq( 2 );
	((MFloatControl*) ivPtEq->getControl( 0 ) )->setName( "Low" );
	((MFloatControl*) ivPtEq->getControl( 1 ) )->setName( "High" );
};

//-----------------------------------------------------------------------------
// + DESTRUCTION
//-----------------------------------------------------------------------------
MDefaultMixerChannel::~MDefaultMixerChannel()
{
	fireDestroy();

	SAFE_DELETE( ivMuteControl );
	SAFE_DELETE( ivEqControl );

	SAFE_DELETE( ivVolControl );
	SAFE_DELETE( ivPanControl );

	SAFE_DELETE( ivNameControl );

	for( unsigned int i=0;i<DEFAULT_FX_SEND_COUNT;i++ )
		SAFE_DELETE( ivFxSendControls[ i ] );
	SAFE_DELETE( ivFxSendControls );

	if( ivPtEq )
		delete ivPtEq;
};

//-----------------------------------------------------------------------------
// + GET CHANNEL NAME
// Returns the string to display
//-----------------------------------------------------------------------------
String MDefaultMixerChannel::getChannelName()
{
	return ivNameControl->getValue();
};

//-----------------------------------------------------------------------------
// + SET CHANNEL NAME
// To set the channelname
//-----------------------------------------------------------------------------
void MDefaultMixerChannel::setChannelName( String name )
{
	ivNameControl->setValue( name );
};

//-----------------------------------------------------------------------------
// + SET MUTE
// To mute a channel
//-----------------------------------------------------------------------------
void MDefaultMixerChannel::setMute( bool isMute )
{
	ivMuteControl->setValue( isMute );
};

//-----------------------------------------------------------------------------
// + GET MUTE
// Returns if this channel is muted
//-----------------------------------------------------------------------------
bool MDefaultMixerChannel::getMute()
{
	return ivMuteControl->getValue();
}; 

//-----------------------------------------------------------------------------
// + SET VOLUME (0.0 - 1.5)
// To sets this channels volume
//-----------------------------------------------------------------------------
void MDefaultMixerChannel::setVolume( FP volume )
{
	ivVolControl->setValue( volume );
};

//-----------------------------------------------------------------------------
// + GET VOLUME
// To sets this channels volume
//-----------------------------------------------------------------------------
FP MDefaultMixerChannel::getVolume()
{
	return ivVolControl->getValue();
};
	
//-----------------------------------------------------------------------------
// + SET PANORAMA (0.0-2.0)
// To set the channels panorama
//-----------------------------------------------------------------------------
void MDefaultMixerChannel::setPanorama( FP panorama )
{
	ivPanControl->setValue( panorama );
};

//-----------------------------------------------------------------------------
// + GET PANORAMA
// Returns this channels panorama
//-----------------------------------------------------------------------------
FP MDefaultMixerChannel::getPanorama()
{
	return ivPanControl->getValue();
};

//-----------------------------------------------------------------------------
// + GET EQ ON
//	returns true if the eq section is activated and will be rendered in goNext
//-----------------------------------------------------------------------------
bool MDefaultMixerChannel::getEqOn()
{
	return ivEqControl->getValue();
};

//-----------------------------------------------------------------------------
// + SET EQ ON
//	to activate the eq section to be rendered in goNext, true=active
//-----------------------------------------------------------------------------
void MDefaultMixerChannel::setEqOn( bool value )
{
	ivEqControl->setValue( value );
};

//-----------------------------------------------------------------------------
// + GET EQ
//	returns the eq section to set the eq values
//-----------------------------------------------------------------------------
MPeakEq* MDefaultMixerChannel::getEq()
{
	return ivPtEq;
};

//-----------------------------------------------------------------------------
// + GET FX SEND COUNT
//	returns the number of fx sends of this channel
//-----------------------------------------------------------------------------
unsigned int MDefaultMixerChannel::getFxSendCount()
{
	return DEFAULT_FX_SEND_COUNT;
};

//-----------------------------------------------------------------------------
// + GET FX SEND
//	returns the amount of send of this channel at the given index
//-----------------------------------------------------------------------------
FP MDefaultMixerChannel::getFxSend( unsigned int index )
{
	ASSERT( index < DEFAULT_FX_SEND_COUNT ); 
	return ivFxSendControls[ index ]->getValue();
};

//-----------------------------------------------------------------------------
// + SET FX SEND
//	sets the amount of send of this channel at the given index
//-----------------------------------------------------------------------------
void MDefaultMixerChannel::setFxSend( unsigned int index, FP value )
{
	ASSERT( index < DEFAULT_FX_SEND_COUNT ); 
	ivFxSendControls[ index ]->setValue( value );
}

//-----------------------------------------------------------------------------
// + GOI NEXT
//	renders a mixerchannel, e.g. the eq if activated
//-----------------------------------------------------------------------------
void MDefaultMixerChannel::goNext( MSoundBuffer* buffer, unsigned int startFrom, unsigned int stopAt )
{
	if( getMute()  )
	{
		for( unsigned int i=0;i<buffer->getChannelCount();i++ )
		{
			FP* ptEnd = buffer->getData(i) + stopAt;
			for( FP* pt = buffer->getData(i) + startFrom; pt<ptEnd; pt++ )
				(*pt) = 0.0f;
		}
	}
};

//-----------------------------------------------------------------------------
// getControlCount
//-----------------------------------------------------------------------------
unsigned int MDefaultMixerChannel::getControlCount()
{
	return FXSEND + 4;
}

//-----------------------------------------------------------------------------
// getControl
//-----------------------------------------------------------------------------
IControl* MDefaultMixerChannel::getControl( unsigned int index )
{
	switch( index )
	{
		case NAME: return ivNameControl; break;
		case VOLUME: return ivVolControl; break;
		case PANORAMA: return ivPanControl; break;
		case MUTE: return ivMuteControl; break;
		case EQON: return ivEqControl; break;
		case LOW: return ivPtEq->getControl( 0 ); break;
		case HIGH: return ivPtEq->getControl( 1 ); break;
		default: return ivFxSendControls[ index - FXSEND ];
	};
}

//-----------------------------------------------------------------------------
// getControlByName
//-----------------------------------------------------------------------------
IControl* MDefaultMixerChannel::getControlByName( String name )
{
	IControl* back = 0;
	unsigned int count = getControlCount();
	for( unsigned int i=0;i<count;i++ )
		if( getControl( i ) != 0 &&
			getControl( i )->getName() == name )
			back = getControl( i );
	return back;
}

//-----------------------------------------------------------------------------
// + ADD CHANNEL LISTENER
//-----------------------------------------------------------------------------
void MDefaultMixerChannel::addFinalizeListener( IFinalizeListener* ptListener )
{
	ivFinalizeListeners.addListener( ptListener );
}

//-----------------------------------------------------------------------------
// + REMOVE CHANNEL LISTENER
//-----------------------------------------------------------------------------
void MDefaultMixerChannel::removeFinalizeListener( IFinalizeListener* ptListener )
{
	ivFinalizeListeners.removeListener( ptListener );
}

//-----------------------------------------------------------------------------
// # FIRE DESTROY
//-----------------------------------------------------------------------------
void MDefaultMixerChannel::fireDestroy()
{
	while( ivFinalizeListeners.getListenerCount() > 0 )
		( (IFinalizeListener*) ivFinalizeListeners.getListener( 0 ) )->objectTerminated( (void*) this );
}

/**
 * returns string representation
 */
String MDefaultMixerChannel::toString()
{
	return this->getChannelName();
}

/**
 * returns the runtime type information
 */
IRtti* MDefaultMixerChannel::getRtti() const
{
	return &gvRtti;
}

/**
 * querries for a specified interface
 */
void* MDefaultMixerChannel::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MDefaultMixerChannel*)this);
	else if( className == "IMixerChannel" )
		return (void*) ((IMixerChannel*)this);
	else
		return MObject::getInterface( className );
}

/**
 * sets the offline state of the channel
 * (system internal, not mute)
 */
void MDefaultMixerChannel::setOffline( bool offline )
{
	//TRACE( "setOffline %s\n", offline ? "true" : "false" );
	ivOffline = offline;
}

/**
 * returns the offline state of the channel
 * (system internal, not mute)
 */
bool MDefaultMixerChannel::getOffline()
{
	return ivOffline;
}

/**
 * returns the channels envelope follower
 */
IEnvelopeFollower* MDefaultMixerChannel::getEnvelopeFollower()
{
	return &ivEnvelopeFollower;
}

IControlCollection* MDefaultMixerChannel::getChild( unsigned int index )
{
	return 0;
}

unsigned int MDefaultMixerChannel::getChildCount()
{
	return 0;
}

String MDefaultMixerChannel::getName()
{
	return this->getChannelName();
}

String MDefaultMixerChannel::getType()
{
	return "MDefaultMixerChannel";
}

void MDefaultMixerChannel::registerTargets( IAutomationTracker* tracker )
{
	unsigned int count = getControlCount();
	for( unsigned int i=0;i<count;i++ )
	{
		IEventTarget* pTarget = (IEventTarget*) getControl( i )->getInterface( "IEventTarget" );
		if( pTarget )
			MTargetRegistry::getInstance()->registerContext(
				new MAutomationContext( pTarget, tracker ) );
	}
}