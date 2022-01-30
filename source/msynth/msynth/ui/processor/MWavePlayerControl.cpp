/**
 * (C)2000-2003 MRoc
 */

#include "MWavePlayerControl.h"
#include <framework/resource/text/MTextResource.h>
#include <gui/dialog/MFileDialog.h>
#include <gui/dialog/MMessageBox.h>
#include <gui/MTopWnd.h>
#include <gui/button/MFlatTextButtonRenderer.h>

#define MAX_SAMPLE_NAME_LENGTH 256

const int MWavePlayerControl::WPC_DEFAULT_WIDTH = 59;
const int MWavePlayerControl::WPC_DEFAULT_HEIGHT = 280;

INIT_RTTI( MWavePlayerControl::MControlListener, "MWavePlayerControl::MControlListener" );
INIT_RTTI( MWavePlayerControl::MActionListener, "MWavePlayerControl::MActionListener" );

MWavePlayerControl::MWavePlayerControl( MWavePlayer *ptWavePlayer, int id )
	: MWndCollection(),
	ivIndex( id ),
	ivPtControlListener( 0 ),
	ivPtActionListener( 0 ),
	ivPtButtonChannelSelect( 0 ),
	ivPtButtonLoad( 0 ),
	ivPtButtonSave( 0 ),
	ivPtNameGui( 0 ),
	ivPtReverseControl( 0 ),
	ivPtLoopControl( 0 ),
	ivPtMuteControl( 0 ),
	ivPtExclusiveControl( 0 ),
	ivPtPitchControl( 0 ),
	ivPtVolControl( 0 ),
	ivPtPanControl( 0 ),
	ivPtWavePlayer( 0 )
{
	ASSERT( ptWavePlayer );

	setBkColor( MColorMap::BK_COLOR3 );
	setWavePlayer( ptWavePlayer );

	ivPtVolControl = new MFloatGui(	((MFloatControl*)(ivPtWavePlayer->getControl( MWavePlayer::VOLUME ))),	getBkColor() );
	addChild( ivPtVolControl );

	ivPtPitchControl = new MFloatGui( ((MFloatControl*)(ivPtWavePlayer->getControl( MWavePlayer::PITCH ))), getBkColor() );
	addChild( ivPtPitchControl );

	ivPtPanControl = new MFloatGui( ((MFloatControl*)(ivPtWavePlayer->getControl( MWavePlayer::PAN ))), getBkColor() );
	addChild( ivPtPanControl );

	String buffer("");
	buffer.Format( "Sample %i", (ivIndex+1) );
	ivPtButtonChannelSelect	= new MToggleButton(
		new MFlatTextButtonRenderer( buffer, MColorMap::FG_COLOR3 ) );
	ivPtButtonChannelSelect->setBkColor( MColorMap::BK_COLOR3 );
	ivPtButtonChannelSelect->setSize( MSize( WPC_DEFAULT_WIDTH-2, 14 ) );
	ivPtButtonChannelSelect->setToolTipText(
		 MTextResource::getInstance()->getString("TT_BEATBOX_EDITOR_SELECT_BUTTON", ivIndex+1 ) );
	addChild( ivPtButtonChannelSelect );

	ivPtNameGui = new MStringGui( ((MStringControl*)(ivPtWavePlayer->getControl( MWavePlayer::SAMPLENAME ) ) ), getBkColor() );
	//ivPtNameGui->setSize( MSize( 57, 15 ) );
	addChild( ivPtNameGui );

	ivPtButtonLoad = new MButton(
		"load",
		MColorMap::FG_COLOR3,
		MColorMap::BK_COLOR5,
		MColorMap::BK_COLOR3 );
	addChild( ivPtButtonLoad );

	ivPtButtonSave = new MButton(
		"save",
		MColorMap::FG_COLOR3,
		MColorMap::BK_COLOR5,
		MColorMap::BK_COLOR3 );
	addChild( ivPtButtonSave );

	ivPtButtonLoad->setToolTipText(
		 MTextResource::getInstance()->getString("TT_BEATBOX_EDITOR_LOAD_BUTTON", ivIndex + 1 ) );
	ivPtButtonSave->setToolTipText(
		MTextResource::getInstance()->getString("TT_BEATBOX_EDITOR_SAVE_BUTTON", ivIndex + 1 ) );

	ivPtReverseControl = new MBoolGui( ((MBoolControl*)(ivPtWavePlayer->getControl( MWavePlayer::REV ))), getBkColor() );
	addChild( ivPtReverseControl );

	ivPtLoopControl = new MBoolGui( ((MBoolControl*)(ivPtWavePlayer->getControl( MWavePlayer::LOOP ))), getBkColor() );
	addChild( ivPtLoopControl );

	ivPtMuteControl = new MBoolGui( ((MBoolControl*)(ivPtWavePlayer->getControl( MWavePlayer::MUTE ))), getBkColor() );
	addChild( ivPtMuteControl );

	ivPtExclusiveControl = new MBoolGui( ((MBoolControl*)(ivPtWavePlayer->getControl( MWavePlayer::EXCLUSIVE ))), getBkColor() );
	addChild( ivPtExclusiveControl );

	ivPtActionListener = new MActionListener( this );

	setSize( MSize( WPC_DEFAULT_WIDTH, WPC_DEFAULT_HEIGHT ) );

	MBoolControl* validCon = (MBoolControl*) ivPtWavePlayer->getControl( MWavePlayer::ISVALID );
	enableControls( validCon->getValue() );

	setSize( MSize( WPC_DEFAULT_WIDTH, WPC_DEFAULT_HEIGHT ) );
}

MWavePlayerControl::~MWavePlayerControl()
{
	ivChilds.clear();

	SAFE_DELETE( ivPtActionListener );
	SAFE_DELETE( ivPtControlListener );

	SAFE_DELETE( ivPtButtonChannelSelect );

	SAFE_DELETE( ivPtVolControl );
	SAFE_DELETE( ivPtPitchControl );
	SAFE_DELETE( ivPtPanControl );

	SAFE_DELETE( ivPtReverseControl );
	SAFE_DELETE( ivPtLoopControl );
	SAFE_DELETE( ivPtMuteControl );
	SAFE_DELETE( ivPtExclusiveControl );

	SAFE_DELETE( ivPtButtonLoad );
	SAFE_DELETE( ivPtButtonSave );

	SAFE_DELETE( ivPtNameGui );
}

void MWavePlayerControl::paint( IGraphics* ptGraphics, const MRect &rect )
{
	ptGraphics->drawRect( rect, MColorMap::FG_COLOR3 );
}

MWavePlayer *MWavePlayerControl::getWavePlayer()
{
	return ivPtWavePlayer;
}

void MWavePlayerControl::setWavePlayer(MWavePlayer *wavePlayer)
{
	if( ivPtWavePlayer )
		SAFE_DELETE( ivPtControlListener );
	ivPtWavePlayer = wavePlayer;
	if( ivPtWavePlayer )
		ivPtControlListener = new MControlListener( this, ivPtWavePlayer );
}

void MWavePlayerControl::setActiveChannel(bool value)
{
	ivPtButtonChannelSelect->setValue( value );
}

bool MWavePlayerControl::getActiveChannel()
{
	return ivPtButtonChannelSelect->getValue();
}

void MWavePlayerControl::doLayout()
{
	MWnd *wndArray[3] = { ivPtVolControl, ivPtPanControl, ivPtPitchControl };
	MWnd *wndArray2[4] = { ivPtMuteControl, ivPtLoopControl, ivPtReverseControl, ivPtExclusiveControl };
	int maxWidth = 0;

	for( unsigned int i=0;i<3;i++ )
	{
		if( wndArray[ i ]->getWidth() > maxWidth )
			maxWidth = wndArray[ i ]->getWidth();
	}
	for( i=0;i<3;i++ )
	{
		int offset = ( maxWidth - wndArray[ i ]->getWidth() ) / 2;
		wndArray[ i ]->setRect(
			MRect(
				5 + offset,
				55 + i*50,
				wndArray[ i ]->getWidth(),
				wndArray[ i ]->getHeight() ) );
	}

	for( i=0;i<4;i++ )
		wndArray2[ i ]->setRect(
			MRect(
				4,
				205 + i*18,
				getWidth()-8,
				17 ) );

	ivPtButtonChannelSelect->setRect( MRect( 1, 1, 57, 15 ) );
	ivPtNameGui->setRect( MRect( 1, 16, 57, 16 ) );
	ivPtButtonLoad->setRect( MRect( 1, 32, 28, 15 ) );
	ivPtButtonSave->setRect( MRect( 30, 32, 28, 15 ) );
}

void MWavePlayerControl::enableControls( bool valid )
{
	ivPtNameGui->setEnabled( valid );
	ivPtPitchControl->setEnabled( valid && !ivPtWavePlayer->getLoop() );
	ivPtPanControl->setEnabled( valid );
	ivPtVolControl->setEnabled( valid );
	ivPtMuteControl->setEnabled( valid );
	ivPtLoopControl->setEnabled( valid );
	ivPtReverseControl->setEnabled( valid );
	ivPtMuteControl->setEnabled( valid );
	ivPtButtonSave->setEnabled( valid );
	ivPtExclusiveControl->setEnabled( valid );

	repaint();
}

void MWavePlayerControl::setId( int num )
{
	ivIndex = num;
	if( ivPtButtonChannelSelect )
	{
		String numAsStr = MInt::toString( ivIndex + 1 );
		((MFlatTextButtonRenderer*)ivPtButtonChannelSelect->getRenderer())->setText( "Sample " + numAsStr );
		ivPtButtonChannelSelect->setToolTipText(
			MTextResource::getInstance()->getString("TT_BEATBOX_EDITOR_SELECT_BUTTON", ivIndex+1 ) );
	}
}

MWavePlayerControl::MControlListener::MControlListener( MWavePlayerControl* ptGui, MWavePlayer* ptModel ) :
	ivPtGui( ptGui ),
	ivPtModel( ptModel )
{
	ivPtModel->getControl( MWavePlayer::LOOP )->addControlListener( this );
	ivPtModel->getValidControl()->addControlListener( this );
}

MWavePlayerControl::MControlListener::~MControlListener()
{
	ivPtModel->getControl( MWavePlayer::LOOP )->removeControlListener( this );
	ivPtModel->getValidControl()->removeControlListener( this );
}

void MWavePlayerControl::MControlListener::valueChanged( MControlListenerEvent *anEvent )
{
	if( anEvent->getSource() == ivPtModel->getControl( MWavePlayer::LOOP ) )
	{
		if( ivPtModel->isValid() )
		{
			ivPtGui->ivPtPitchControl->setEnabled( ! ivPtModel->getLoop() );
			ivPtGui->ivPtPitchControl->repaint();
		}
	}
	else if( anEvent->getSource() == ivPtModel->getControl( MWavePlayer::ISVALID ) )
		ivPtGui->enableControls( ivPtModel->isValid() );
}

void MWavePlayerControl::addActionListener( IActionListener* ptListener )
{
	ASSERT( ptListener );
	this->ivActionListener.addListener( ptListener );
}

void MWavePlayerControl::removeActionListener( IActionListener* ptListener )
{
	ASSERT( ptListener );
	ASSERT( ivActionListener.getListenerCount() > 0 );
	this->ivActionListener.removeListener( ptListener );
}

void MWavePlayerControl::fireActionPerformed( void* ptSrc )
{
	unsigned int count = ivActionListener.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IActionListener*)ivActionListener.getListener( i ))->onActionPerformed( (void*) this );
}

MWavePlayerControl::MActionListener::MActionListener( MWavePlayerControl* ptControl ) :
	ivPtControl( ptControl )
{
	ivPtControl->ivPtButtonLoad->addActionListener( this );
	ivPtControl->ivPtButtonSave->addActionListener( this );
	ivPtControl->ivPtButtonChannelSelect->addActionListener( this );
}

MWavePlayerControl::MActionListener::~MActionListener()
{
	ivPtControl->ivPtButtonLoad->removeActionListener( this );
	ivPtControl->ivPtButtonSave->removeActionListener( this );
	ivPtControl->ivPtButtonChannelSelect->removeActionListener( this );
}

void MWavePlayerControl::MActionListener::onActionPerformed( void* ptSrc )
{
	MButton* ptButton = (MButton*) ptSrc;
	if( ptButton == ivPtControl->ivPtButtonChannelSelect )
		ivPtControl->fireActionPerformed( ivPtControl );
	else if( ptButton == ivPtControl->ivPtButtonLoad )
	{
		MFileDialog dlg;
		if(
			dlg.showOpenDialog(
				((MTopWnd*)ivPtControl->getTopParent()),
				"Open wave",
				"Windows PCM Wave (*.wav)\0*.WAV\0All files (*.*)\0*.*\00" ) )
		{
			try
			{
				ivPtControl->ivPtWavePlayer->loadFile( dlg.getSelectedFile() );
			}
			catch(MException e)
			{	
				MMessageBox::showError(
					"ERROR_LOADING",
					dlg.getSelectedFile(),
					e.getExceptionDescripton() );
			}
		}
	}
	else if( ptButton == ivPtControl->ivPtButtonSave )
	{
		if( ! ivPtControl->ivPtWavePlayer->isValid() )
			return;

		MFileDialog dlg;
		if(
			dlg.showSaveDialog(
				((MTopWnd*)ivPtControl->getTopParent()),
				"Save wave",
				"Windows PCM Wave (*.wav)\0*.WAV\0All files (*.*)\0*.*\00" ) )
		{
			try
			{
				ivPtControl->ivPtWavePlayer->saveFile( dlg.getSelectedFile() );
			}
			catch( MException e )
			{	
				MMessageBox::showError(
					"ERROR_SAVING",
					dlg.getSelectedFile(),
					e.getExceptionDescripton() );
			}
		}
	}
}