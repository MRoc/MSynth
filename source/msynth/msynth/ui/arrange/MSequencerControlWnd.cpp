/**
 * (C)2001-2003 MRoc
 */

#include "MSequencerControlWnd.h"

#include <framework/resource/text/MTextResource.h>
#include <gui/dialog/MFileDialog.h>
#include <gui/dialog/MMessageBox.h>
#include <gui/MGUIUtils.h>

INIT_RTTI( MSequencerControlWnd, "MSequencerControlWnd" );
INIT_RTTI( MSequencerControlWnd::MPlayStateListener, "MSequencerControlWnd::MPlayStateListener" );
INIT_RTTI( MSequencerControlWnd::MButtonListener, "MSequencerControlWnd::MButtonListener" );

MSequencerControlWnd::MSequencerControlWnd() :
	ivPtSequencerControl( 0 ),
	ivPtCountControl( 0 ),
	ivPtShuffleControl( 0 ),
	ivPtButtonPlay( 0 ),
	ivPtButtonStreaming( 0 ),
	ivPtButtonRecord( 0 ),
	ivPtButtonStop( 0 ),
	ivPtPosLabel( 0 ),
	ivPtPlaystateListener( 0 ),
	ivPtButtonListener( 0 )
{
	ivPtNamespace = new MWndNamespace( this );

	ivBkColor = MColorMap::BK_COLOR1;
	ivLayout.load( "resource/layout/MSequencerControlWnd.xml" );
	ivLayout.getRoot()->setNamespace( ivPtNamespace );

	// PLAY
	ivPtButtonPlay = new MToggleButton( 
		IDB_ICON_PLAY_OFF,
		IDB_ICON_PLAY_ON,
		MColorMap::create(255,0,0),
		MColorMap::create(255,0,0),
		ivBkColor );
	ivPtButtonPlay->setRect( MRect( 5, 5, 55, 40 ) );
	ivPtButtonPlay->setToolTipText( MTextResource::getInstance()->getString("TT_ARRANGE_PLAY_BUTTON") );
	addChild( ivPtButtonPlay );

	// STOP
	ivPtButtonStop = new MButton( 
		IDB_ICON_STOP_OFF,
		IDB_ICON_STOP_ON,
		MColorMap::create(255,0,0),
		MColorMap::create(255,0,0),
		ivBkColor );
	ivPtButtonStop->setRect( MRect( 5, 5, 55, 40 ) );
	ivPtButtonStop->setToolTipText( MTextResource::getInstance()->getString("TT_ARRANGE_STOPY_BUTTON") );
	addChild( ivPtButtonStop );

	// EXPORT
	ivPtButtonStreaming	= new MToggleButton( 
		IDB_ICON_RECORD_OFF,
		IDB_ICON_RECORD_ON,
		MColorMap::create(0,255,0),
		MColorMap::create(0,255,0),
		ivBkColor );
	ivPtButtonStreaming->setRect( MRect( 5, 5, 55, 40 ) );
	ivPtButtonStreaming->setToolTipText( MTextResource::getInstance()->getString("TT_ARRANGE_EXPORT_BUTTON") );
	addChild( ivPtButtonStreaming );

	// RECORD
	ivPtButtonRecord = new MToggleButton( 
		IDB_ICON_AUTO_OFF,
		IDB_ICON_AUTO_ON,
		MColorMap::create(0,255,0),
		MColorMap::create(0,255,0),
		ivBkColor );
	ivPtButtonRecord->setRect( MRect( 5, 5, 55, 40 ) );
	ivPtButtonRecord->setToolTipText( MTextResource::getInstance()->getString("TT_ARRANGE_RECORD_BUTTON" ) );
	addChild( ivPtButtonRecord );
	
	// shuffle control
	ivPtShuffleControl = new MCountControl(
		3,
		0,
		100,
		ivBkColor,
		MColorMap::FG_COLOR3,
		"Shuffle factor",
		90 );
	ivPtShuffleControl->setRect( MRect( 460, 20, 150, 14 ) );
	ivPtShuffleControl->getModel()->addListener( this );
	addChild( ivPtShuffleControl );

	// bpm control
	ivPtCountControl = new MCountControl(
		3,
		60,
		260,
		ivBkColor,
		MColorMap::FG_COLOR3,
		"Beats per minute",
		90 );
	ivPtCountControl->setRect( MRect( 460, 5, 150, 14 ) );
	ivPtCountControl->getModel()->addListener( this );
	addChild( ivPtCountControl );

	// play position label
	ivPtPosLabel = new MPlayPositionLabel(
		ivBkColor,
		MColorMap::FG_COLOR3,
		"Playposition" );
	ivPtPosLabel->setRect( MRect( 460, 30, 150, 16 ) );
	addChild( ivPtPosLabel );

	ivPtButtonListener = new MButtonListener( this );

	MWndNamespace::regObjNS( ivPtPosLabel, ivPtPosLabel, "Playposition" );
	MWndNamespace::regObjNS( ivPtShuffleControl, ivPtShuffleControl, "Shuffle" );
	MWndNamespace::regObjNS( ivPtButtonPlay, ivPtButtonPlay, "Play" );
	MWndNamespace::regObjNS( ivPtButtonStop, ivPtButtonStop, "Stop" );
	MWndNamespace::regObjNS( ivPtButtonRecord, ivPtButtonRecord, "Record" );
	MWndNamespace::regObjNS( ivPtButtonStreaming, ivPtButtonStreaming, "Export" );
	MWndNamespace::regObjNS( ivPtCountControl, ivPtCountControl, "Bpm" );
};

MSequencerControlWnd::~MSequencerControlWnd()
{
	ivPtShuffleControl->getModel()->removeListener( this );
	ivPtCountControl->getModel()->removeListener( this );

	SAFE_DELETE( ivPtButtonListener );
	SAFE_DELETE( ivPtPlaystateListener );

	if( ivPtSequencerControl->getPlayState() == IPlayState::SESSION_NOT_PLAYING || 
		ivPtSequencerControl->getPlayState() == IPlayState::SESSION_PLAYING ||
		ivPtSequencerControl->getPlayState() == IPlayState::PLAYING )
	{
		ivPtSequencerControl->stop();
	}

	ivChilds.clear();
	SAFE_DELETE( ivPtCountControl );
	SAFE_DELETE( ivPtShuffleControl );
	SAFE_DELETE( ivPtButtonPlay );
	SAFE_DELETE( ivPtButtonStop );
	SAFE_DELETE( ivPtButtonRecord );
	SAFE_DELETE( ivPtButtonStreaming );
	SAFE_DELETE( ivPtPosLabel );
}

void MSequencerControlWnd::paint( IGraphics* ptGraphics, const MRect &rect )
{
	MGuiUtils::paintRaisedBorder(
		ptGraphics,
		MRect( rect.getPoint(), MSize(rect.getWidth()-1, rect.getHeight()-1) ),
		ivBkColor );
}

void MSequencerControlWnd::updateFromModel()
{
	//ShuffleSlider
	ivPtShuffleControl->setPos( int( ivPtSequencerControl->getSequencer()->getShuffle() * 100 ) );

	//SpinButton
	ivPtCountControl->setPos( (int) ivPtSequencerControl->getSequencer()->getBpm() );
}

void MSequencerControlWnd::Play()
{
	ASSERT( ivPtSequencerControl );
	if( ! ivPtSequencerControl->isPlaying() )
		ivPtSequencerControl->start();
	else
		ivPtSequencerControl->stop();
}

void MSequencerControlWnd::SetBPM()
{
	ivPtSequencerControl->getSequencer()->setBpm( (unsigned int) ivPtCountControl->getPos() );
}

void MSequencerControlWnd::Export()
{
#ifndef __TESTVERSION

	if(	ivPtSequencerControl->getPlayState() == IPlayState::NEUTRAL ||
		ivPtSequencerControl->getPlayState() == IPlayState::PLAYING )
	{
		MRecordDialog modeDialog;
		modeDialog.create( MRect( 0, 0, 300, 150 ), ((MTopWnd*)getTopParent() ) );
		MDialog::DlgResult res = modeDialog.doModal();
		if( res == MDialog::DlgResult::OK )
		{
			MFileDialog fileDialog;
			if(
				fileDialog.showSaveDialog(
					(MTopWnd*)getTopParent(),
					"Export wave...",
					"Windows PCM Wave\0*.WAV\0All Files\0*.*\0\0"
				) )
			{
				repaint(); 
				switch( modeDialog.getSelectedMode() )
				{
					case MRecordDialog::RENDER:
						ivPtSequencerControl->stop();
						ivPtSequencerControl->render( fileDialog.getSelectedFile() );
						break;
					case MRecordDialog::STREAMING:
						if( ! ivPtSequencerControl->record( fileDialog.getSelectedFile() ) )
							MMessageBox::showError( "ERROR_OPENING_OUTPUTSTREAM" );
						break;
				}
			}
			else
				ivPtButtonStreaming->setValue( false );
		}
		else
			ivPtButtonStreaming->setValue( false );
	}
	else if( ivPtSequencerControl->getPlayState() )
	{
		ivPtSequencerControl->stopStreamToDisk();
	}
#else
	MMessageBox::showError( "DISABLED_IN_DEMO" );
	ivPtButtonStreaming->setValue( false );
#endif
}

void MSequencerControlWnd::SetSequencerControl( MSequencerControl* ptControl )
{
	if( ivPtSequencerControl )
	{
		SAFE_DELETE( ivPtPlaystateListener );
		ivPtPosLabel->setUpdateThread( 0 );
	}
	ivPtSequencerControl = ptControl;
	if( ivPtSequencerControl )
	{
		ivPtPosLabel->setUpdateThread( ivPtSequencerControl->getUpdateThread() );
		ivPtPlaystateListener = new MPlayStateListener( this, ivPtSequencerControl );
	}
}

void MSequencerControlWnd::doLayout()
{
	if( ivChilds.size() > 0 )
		ivLayout.doLayout( this );
}

void MSequencerControlWnd::onCommand( unsigned int id )
{
	if( id == ID_PLAY )
		Play();
	else if( id == ID_EXPORT )
		Export();
	else
		MSequencerControlWnd::onCommand( id );
}

void MSequencerControlWnd::valueChanged( MSpinButtonModel* pModel )
{
	if( ivPtShuffleControl->getModel() == pModel )
		ivPtSequencerControl->getSequencer()->setShuffle( FP( ivPtShuffleControl->getPos() ) / 100.0f );
	else if( ivPtCountControl->getModel() == pModel )
		ivPtSequencerControl->getSequencer()->setBpm( (unsigned int) ivPtCountControl->getPos() );
}

/**
 * constructor
 */
MSequencerControlWnd::MPlayStateListener::MPlayStateListener(
	MSequencerControlWnd* ptControl,
	MSequencerControl* ptModel  ) :
	ivPtControl( ptControl ),
	ivPtSControl( ptModel )
{
	ivPtSControl->addPlayStateListener( this );
}

/**
 * destructor
 */
MSequencerControlWnd::MPlayStateListener::~MPlayStateListener()
{
	ivPtSControl->removePlayStateListener( this );
}

/**
 * invoked when play state changed
 */
void MSequencerControlWnd::MPlayStateListener::onPlayStateChanged( IPlayState::PlayState newState )
{
	bool play, stream;
	switch( newState )
	{
		case IPlayState::NEUTRAL:
			play = false;
			stream = false;
			break;
		case IPlayState::PLAYING:
			play = true;
			stream = false;
			break;
		case IPlayState::RENDERING:
			play = false;
			stream = true;
			break;
		case IPlayState::SESSION_NOT_PLAYING:
			play = false;
			stream = true;
			break;
		case IPlayState::SESSION_PLAYING:
			play = true;
			stream = true;
			break;
		default:
			ASSERT( false );
	}
	if( ivPtControl->ivPtButtonPlay->getValue() != play )
		ivPtControl->ivPtButtonPlay->setValue( play );
	if( ivPtControl->ivPtButtonStreaming->getValue() != stream )
		ivPtControl->ivPtButtonStreaming->setValue( stream );

}

MSequencerControlWnd::MButtonListener::MButtonListener( MSequencerControlWnd* ptwnd ) :
	ivPtWnd( ptwnd )
{
	ivPtWnd->ivPtButtonPlay->addActionListener( this );
	ivPtWnd->ivPtButtonStreaming->addActionListener( this );
	ivPtWnd->ivPtButtonRecord->addActionListener( this );
	ivPtWnd->ivPtButtonStop->addActionListener( this );
}
MSequencerControlWnd::MButtonListener::~MButtonListener()
{
	ivPtWnd->ivPtButtonPlay->removeActionListener( this );
	ivPtWnd->ivPtButtonStreaming->removeActionListener( this );
	ivPtWnd->ivPtButtonRecord->removeActionListener( this );
	ivPtWnd->ivPtButtonStop->removeActionListener( this );
}
void MSequencerControlWnd::MButtonListener::onActionPerformed( void* ptSrc )
{
	MButton* ptButton = (MButton*) ptSrc;
	if( ptButton == ivPtWnd->ivPtButtonPlay )
		ivPtWnd->Play();
	else if( ptButton == ivPtWnd->ivPtButtonStop )
		ivPtWnd->ivPtSequencerControl->stop();
	else if( ptButton== ivPtWnd->ivPtButtonStreaming )
		ivPtWnd->Export();
	else if( ptButton == ivPtWnd->ivPtButtonRecord )
	{
		MTargetRegistry::getInstance()->setRecordMode(
			ivPtWnd->ivPtButtonRecord->getValue() ?
			MTargetRegistry::RECORD_EVENTS : MTargetRegistry::DISCARD_EVENTS );
	}
}