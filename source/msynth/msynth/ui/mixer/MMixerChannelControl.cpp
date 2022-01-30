/**
 * (C)2001-2003 MRoc
 */
 
#include "MMixerChannelControl.h"

#define VOLUME_SLIDER_CHANNEL_ID		1200
#define CHANNELNAME_CEDIT_CHANNEL_ID	2500
#define MUTE_BUTTON_CHANNEL_ID			5200
#define PANORAMA_SLIDER_CHANNEL_ID		1250
#define PANORAMA_LABEL_CHANNEL_ID		5300
#define EQ_BUTTON_ID					1300
#define HIGH_SLIDER_CHANNEL_ID			1350
#define HIGH_LABEL_CHANNEL_ID			1351
#define LOW_SLIDER_CHANNEL_ID			1352
#define LOW_LABEL_CHANNEL_ID			1353

#define FXSEND_SLIDER_CHANNEL_ID		1400
#define FXSEND_LABEL_CHANNEL_ID			1410

int	MMixerChannelControl::PREF_WIDTH = 60;//56;
int	MMixerChannelControl::PREF_HEIGHT = 278;

INIT_RTTI( MMixerChannelControl::MMixerChannelListener, "MMixerChannelControl::MMixerChannelListener" );

MMixerChannelControl::MMixerChannelControl()
	:
	MWndCollection(),
	ivNameGui( 0 ),
	ivPanGui( 0 ),
	ivHighGui( 0 ),
	ivLowGui( 0 ),
	ivMuteGui( 0 ),
	ivEqOnGui( 0 ),
	ivVolGui( 0 ),
	ivPtMixerChannel( 0 ),
	ivIsMasterChannel( false ),
	ivPtListener( 0 ),
	ivPtPeakControl( 0 )
{
	for( int i=0;i<4;i++ )
		ivFxGuis[ i ] = 0;

	ivBkColor = MColorMap::BK_COLOR2;
}

MMixerChannelControl::MMixerChannelControl( MDefaultMixerChannel *ptMixerChannel, bool isMaster )
	:
	MWndCollection(),
	ivNameGui( 0 ),
	ivPanGui( 0 ),
	ivHighGui( 0 ),
	ivLowGui( 0 ),
	ivMuteGui( 0 ),
	ivEqOnGui( 0 ),
	ivVolGui( 0 ),
	ivPtMixerChannel( 0 ),
	ivIsMasterChannel( isMaster ),
	ivPtListener( 0 ),
	ivPtPeakControl( 0 )
{
	for( int i=0;i<4;i++ )
		ivFxGuis[ i ] = 0;

	setMixerChannel( ptMixerChannel );
	ivBkColor = MColorMap::BK_COLOR2;

	((MFloatControl*)(ivPtMixerChannel->getControl( MDefaultMixerChannel::LOW )))->setName( String( "Low" ) );
	((MFloatControl*)(ivPtMixerChannel->getControl( MDefaultMixerChannel::HIGH )))->setName( String( "High" ) );

	ivNameGui = new MStringGui(
		((MStringControl*)(ivPtMixerChannel->getControl( MDefaultMixerChannel::NAME ))),
		getBkColor() );
	ivNameGui->setRect( MRect( 0, 0, 55, 16 ) );
	addChild( ivNameGui );

	if( ivIsMasterChannel )
		for( int i=0;i<4;i++ )
			ivFxGuis[ i ] = 0;
	else
	{
		int index;
		int i1 = 2;
		int j1;
		for( int i=0;i<2;i++ )
		{
			j1 = 17;
			for( int j=0;j<2;j++ )
			{
				index = j*2 + i;
				ivFxGuis[ index ] = new MFloatGui(
					IDB_EQSLIDER_ON,
					IDB_EQSLIDER_OFF,
					((MFloatControl*)(ivPtMixerChannel->getControl( MDefaultMixerChannel::FXSEND + index ))),
					MColorMap::BK_COLOR3 );
				ivFxGuis[ index ]->setRect( MRect( i1, j1, ivFxGuis[ index ]->getWidth(), ivFxGuis[ index ]->getHeight() ) );
				addChild( ivFxGuis[ index ] );
				j1 += 40;
			}
			i1 += 26;
		}
	}

	ivVolGui = new MVFloatGui(
		((MFloatControl*)(ivPtMixerChannel->getControl( MDefaultMixerChannel::VOLUME ))),
		MColorMap::BK_COLOR3 );
	ivVolGui->setRect( MRect( 2, 98, 20, 200 ) );
	addChild( ivVolGui );

	ivEqOnGui = new MBoolGui(
		((MBoolControl*)(ivPtMixerChannel->getControl( MDefaultMixerChannel::EQON ))),
		MColorMap::BK_COLOR3 );
	ivEqOnGui->setSize( MSize( 30, 20 ) );
	ivEqOnGui->setRect( MRect( ( 34 - ivEqOnGui->getWidth() ) / 2 + 20, 98, ivEqOnGui->getWidth(), ivEqOnGui->getHeight() ) ); 
	addChild( ivEqOnGui );

	ivHighGui = new MFloatGui(
		IDB_EQSLIDER_ON,
		IDB_EQSLIDER_OFF,
		((MFloatControl*)(ivPtMixerChannel->getControl( MDefaultMixerChannel::HIGH ))),
		MColorMap::BK_COLOR3 );
	ivHighGui->setRect( MRect( ( 34 - ivHighGui->getWidth() ) / 2 + 20, 126, ivHighGui->getWidth(), ivHighGui->getHeight() ) );
	addChild( ivHighGui );

	ivLowGui = new MFloatGui(
		IDB_EQSLIDER_ON,
		IDB_EQSLIDER_OFF,
		((MFloatControl*)(ivPtMixerChannel->getControl( MDefaultMixerChannel::LOW ))),
		MColorMap::BK_COLOR3 );
	ivLowGui->setRect( MRect( ( 34 - ivLowGui->getWidth() ) / 2 + 20, 166, ivLowGui->getWidth(), ivLowGui->getHeight() ) );
	addChild( ivLowGui );

	ivPanGui = new MFloatGui(
		IDB_EQSLIDER_ON, IDB_EQSLIDER_OFF,
		((MFloatControl*)(ivPtMixerChannel->getControl( MDefaultMixerChannel::PANORAMA ))),
		MColorMap::BK_COLOR3 );
	ivPanGui->setRect( MRect( ( 34 - ivPanGui->getWidth() ) / 2 + 20, 214, ivPanGui->getWidth(), ivPanGui->getHeight() ) );
	addChild( ivPanGui );

	ivMuteGui = new MBoolGui(
		((MBoolControl*)(ivPtMixerChannel->getControl( MDefaultMixerChannel::MUTE ))),
		MColorMap::BK_COLOR3 );
	ivMuteGui->setRect( MRect( 2, 259, 52, 17 ) );
	addChild( ivMuteGui );

	ivPtPeakControl = new MPeakControl();
	ivPtPeakControl->setRect( MRect( 56, 1, 3, 275 ) );
	addChild( ivPtPeakControl );

	ivPtPeakControl->setEnvelopeFollower( ivPtMixerChannel->getEnvelopeFollower() );
}

MMixerChannelControl::~MMixerChannelControl()
{
	SAFE_DELETE( ivPtListener );

	setMixerChannel( 0 );

	removeChild( ivNameGui );
	removeChild( ivPanGui );
	removeChild( ivEqOnGui );
	removeChild( ivLowGui );
	removeChild( ivHighGui );
	removeChild( ivVolGui );
	removeChild( ivMuteGui );
	removeChild( ivPtPeakControl );

	SAFE_DELETE( ivNameGui );
	SAFE_DELETE( ivPanGui );
	SAFE_DELETE( ivEqOnGui );
	SAFE_DELETE( ivLowGui );
	SAFE_DELETE( ivHighGui );
	SAFE_DELETE( ivVolGui );
	SAFE_DELETE( ivMuteGui );
	SAFE_DELETE( ivPtPeakControl );
	
	for( int i=0;i<4;i++ )
	{
		if( ivFxGuis[ i ] )
		{
			removeChild( ivFxGuis[ i ] );
			delete( ivFxGuis[ i ] );
			ivFxGuis[ i ] = 0;
		}
	}
}

void MMixerChannelControl::paint( IGraphics* ptGraphics, const MRect &rect )
{
	ptGraphics->fillRect( rect, ivBkColor );

	MRect eqBack( 20, 98, 34, 110 );
	ptGraphics->fillRect( eqBack, MColorMap::BK_COLOR3 );

	MRect panBack( 20, 209, 34, 49 );
	ptGraphics->fillRect( panBack, MColorMap::BK_COLOR3 );
}

IMixerChannel *MMixerChannelControl::getMixerChannel()
{
	return ivPtMixerChannel;
}

void MMixerChannelControl::setMixerChannel( MDefaultMixerChannel *ptMixerChannel )
{
	if( ivPtMixerChannel )
		SAFE_DELETE( ivPtListener )

	ivPtMixerChannel = ptMixerChannel;

	if( ivPtMixerChannel )
	{
		ivPtListener = new MMixerChannelListener( this, ivPtMixerChannel );
	}
}

void MMixerChannelControl::updateFromModel()
{
	ASSERT( ivPtMixerChannel != 0 );

	bool eqOn = ivPtMixerChannel->getEqOn();
	ivHighGui->setEnabled( eqOn );
	ivLowGui->setEnabled( eqOn );
}

bool MMixerChannelControl::getIsMasterChannel()
{
	return ivIsMasterChannel;
}

String MMixerChannelControl::toString()
{
	String back("");
	back.Format( "<MMixerChannelControl name=\"%s\"/>", getMixerChannel()->getChannelName().getData() );
	return back;
}

MMixerChannelControl::MMixerChannelListener::MMixerChannelListener( MMixerChannelControl* ptControl, MDefaultMixerChannel* ptChannel ) :
	ivPtControl( ptControl ),
	ivPtChannel( ptChannel )
{
	ivPtChannel->addFinalizeListener( this );
	((MBoolControl*)(ivPtChannel->getControl( MDefaultMixerChannel::EQON )))->addControlListener( this );
}

MMixerChannelControl::MMixerChannelListener::~MMixerChannelListener()
{
	if( ivPtChannel )
	{
		((MBoolControl*)(ivPtChannel->getControl( MDefaultMixerChannel::EQON )))->removeControlListener( this );
		ivPtChannel->removeFinalizeListener( this );
		ivPtChannel = 0;
	}
}

/**
 * called if the object is deleted, after recievieing this message,
 * the object MUST deregister itself as listener because the observed
 * object will be deleted, if not there it will result in an endless loop!!!
 */
void MMixerChannelControl::MMixerChannelListener::objectTerminated( void* obj )
{
	ivPtControl->setMixerChannel( 0 );
}

void MMixerChannelControl::MMixerChannelListener::valueChanged( MControlListenerEvent *anEvent )
{
	IControl* pSrc = anEvent->getSource();
	if( pSrc == ivPtChannel->getControl( MDefaultMixerChannel::EQON ) )
	{
		bool eqOn = ivPtChannel->getEqOn();
		ivPtControl->ivHighGui->setEnabled( eqOn );
		ivPtControl->ivHighGui->repaint();
		ivPtControl->ivLowGui->setEnabled( eqOn );
		ivPtControl->ivLowGui->repaint();
	}
}