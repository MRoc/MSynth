/*

	MBeatBoxChannelControlContainer (C)2001 MRoc hifiShock
  
	This class contains the MBeatBoxChannelControls on the
	left side of the BeatBoxNoteFrame

*/

#include "MBeatBoxChannelControlContainer.h"

INIT_RTTI( MBeatBoxChannelControlContainer::MBeatBoxListener, "MBeatBoxChannelControlContainer::MBeatBoxListener" );

//-----------------------------------------------------------------------------
// + CONSTRUCTION
//-----------------------------------------------------------------------------
MBeatBoxChannelControlContainer::MBeatBoxChannelControlContainer(
			MBeatBox* ptBeatBox,
			MSize* ptPartSize ) :
	MyInteractiveWnd(),
	ivPtSize( 0 ),
	ivPtBeatBox( 0 ),
	ivSelectedChannel( 0 ),
	ivPtListener( 0 )
{
	ivBkColor = MColorMap::BK_COLOR2;

	setBeatBox( ptBeatBox );
	setPartSize( ptPartSize );
};

//-----------------------------------------------------------------------------
// + DESTRUCTION
//-----------------------------------------------------------------------------
MBeatBoxChannelControlContainer::~MBeatBoxChannelControlContainer()
{
	UnregisterListeners();
	setBeatBox( 0 );
	deleteControls();
}

void MBeatBoxChannelControlContainer::paint( IGraphics* pG, const MRect &rect )
{
	pG->fillRect( rect, ivBkColor );

	MPoint target = getScrollPos();
	pG->setOffset( pG->getOffset() - target );
	unsigned int controlCount = ivArControls.size();
	for( unsigned int i=0;i<controlCount;i++ )
		((MBeatBoxChannelControl*)ivArControls[i])->paintControl( pG );
	pG->setOffset( pG->getOffset() + target );
};

//-----------------------------------------------------------------------------
// + LAYOUT CONTROLS
// Layouts all controls, but does no repaint!!!
//-----------------------------------------------------------------------------
void MBeatBoxChannelControlContainer::layoutControls()
{
	MSize size = getSize();
	int width = size.getWidth();
	int height = ivPtSize->getHeight();
	int yOffset = 0;

	unsigned int controlCount = ivArControls.size();
 	for( unsigned int i=0;i<controlCount;i++ )
	{
		((MBeatBoxChannelControl*)ivArControls[i])->setPosition( MRect( 0, yOffset, width, height ) );
		yOffset += height;
	}
};

//-----------------------------------------------------------------------------
// + UPDATE FROM MODEL
//-----------------------------------------------------------------------------
void MBeatBoxChannelControlContainer::updateFromModel()
{
	unsigned int controlCount = this->ivArControls.size();
	for( unsigned int j=0;j<controlCount;j++ )
	{
		MBeatBoxChannelControl* ptControl = ivArControls[ j ];
		ptControl->getWavePlayer()->getControl( MWavePlayer::SAMPLENAME )->removeControlListener( ivPtListener );
	}

	deleteControls();

	unsigned int wavePlayerCount = (unsigned int) ivPtBeatBox->getWavePlayerCount();
	for( unsigned int i=0;i<wavePlayerCount;i++ )
	{
		MWavePlayer* ptPlayer = ivPtBeatBox->getWavePlayerAt( i );
		MBeatBoxChannelControl* ptControl = new MBeatBoxChannelControl();
		ptControl->setWavePlayer( ptPlayer );
		ptControl->setBkColor( MColorMap::BK_COLOR3 );
		ptControl->setBkColorSelected( MColorMap::FG_COLOR3 );
		ivArControls.push_back( ptControl );
	}

	RegisterListeners();

	ivSelectedChannel = ivPtBeatBox->getWavePlayerIndex( ivPtBeatBox->getSelectedChannel() );

	ASSERT(
		ivSelectedChannel >= 0 &&
		(unsigned int)ivSelectedChannel < ivArControls.size() );

	((MBeatBoxChannelControl*)ivArControls[ ivSelectedChannel ])->setSelected( true );

	layoutControls();
	repaint();
};

//-----------------------------------------------------------------------------
// # SET BEAT BOX
//-----------------------------------------------------------------------------
void MBeatBoxChannelControlContainer::setBeatBox( MBeatBox* ptBeatBox )
{
	if( ivPtBeatBox )
		SAFE_DELETE( ivPtListener );

	ivPtBeatBox = ptBeatBox;

	if( ivPtBeatBox )
		ivPtListener = new MBeatBoxChannelControlContainer::MBeatBoxListener( ivPtBeatBox, this );
};

//-----------------------------------------------------------------------------
// # GET BEAT BOX
//-----------------------------------------------------------------------------
MBeatBox* MBeatBoxChannelControlContainer::getBeatBox()
{
	return ivPtBeatBox;
};

//-----------------------------------------------------------------------------
// # SET PART SIZE
// Sets the pointer to the partsize
//-----------------------------------------------------------------------------
void MBeatBoxChannelControlContainer::setPartSize( MSize* ptSize )
{
	ivPtSize = ptSize;
};

//-----------------------------------------------------------------------------
// # GET PART SIZE
// Returns the pointer to the partsize
//-----------------------------------------------------------------------------
MSize* MBeatBoxChannelControlContainer::getPartSize()
{
	return ivPtSize;
};

//-----------------------------------------------------------------------------
// + SET SELECTED CHANNEL
//-----------------------------------------------------------------------------
void MBeatBoxChannelControlContainer::SetSelectedChannel( int channelNo )
{
	if( channelNo >= 0 && channelNo < int( ivArControls.size() ) )
	{
		unsigned int count = ivArControls.size();
		for( unsigned int i=0;i<count;i++ )
			((MDCControl*)ivArControls[ i ])->setSelected( (i == (unsigned int)channelNo) );
	}
	if( getVisible() )
	{
		repaint();
	}
};

//-----------------------------------------------------------------------------
// + GET SELECTED CHANNEL
//-----------------------------------------------------------------------------
int MBeatBoxChannelControlContainer::GetSelectedChannel()
{
	return ivSelectedChannel;
};

//-----------------------------------------------------------------------------
// # DELETE CONTROLS
//-----------------------------------------------------------------------------
void MBeatBoxChannelControlContainer::deleteControls()
{
	unsigned int count = ivArControls.size();
	for( unsigned int i=0;i<count;i++ )
		delete ivArControls[ i ];
	ivArControls.clear();
};

//-----------------------------------------------------------------------------
// # REGISTER LISTENERs
//-----------------------------------------------------------------------------
void MBeatBoxChannelControlContainer::RegisterListeners()
{
	if( ivPtBeatBox )
	{
		unsigned int wavePlayerCount = (unsigned int) ivPtBeatBox->getWavePlayerCount();
		for( unsigned int i=0;i<wavePlayerCount;i++ )
			ivPtBeatBox->getWavePlayerAt( i )->getControl( MWavePlayer::SAMPLENAME )->addControlListener( ivPtListener );
	}
};

//-----------------------------------------------------------------------------
// # UNREGISTER LISTENERs
//-----------------------------------------------------------------------------
void MBeatBoxChannelControlContainer::UnregisterListeners()
{
	if( ivPtBeatBox )
	{
		unsigned int wavePlayerCount = (unsigned int) ivPtBeatBox->getWavePlayerCount();
		for( unsigned int i=0;i<wavePlayerCount;i++ )
			ivPtBeatBox->getWavePlayerAt( i )->getControl( MWavePlayer::SAMPLENAME )->removeControlListener( ivPtListener );
	}
};

//-----------------------------------------------------------------------------
// + OnLButtonDown
//-----------------------------------------------------------------------------
void MBeatBoxChannelControlContainer::onMouseDown( MMouseEvent* anEvent )
{
	int channel = anEvent->getY() / ivPtSize->getHeight();

	if( channel >= 0 && channel < int( ivPtBeatBox->getWavePlayerCount() ) )
		ivPtBeatBox->setSelectedChannel( channel );
};

//-----------------------------------------------------------------------------
// + OnBeatBoxDestroy
//-----------------------------------------------------------------------------
/*LRESULT	MBeatBoxChannelControlContainer::OnBeatBoxDestroy( WPARAM wParam, LPARAM lParam )
{
	UnregisterListeners();
	setBeatBox( 0 );

	return IDOK;
};*/

MBeatBoxChannelControlContainer::MBeatBoxListener::MBeatBoxListener( MBeatBox* ptBeatBox, MBeatBoxChannelControlContainer* ptContainer ) :
	ivPtBeatBox( ptBeatBox ),
	ivPtContainer( ptContainer )
{
	ivPtBeatBox->addInstrumentListener( this );
}

MBeatBoxChannelControlContainer::MBeatBoxListener::~MBeatBoxListener()
{
	ivPtBeatBox->removeInstrumentListener( this );
}

void MBeatBoxChannelControlContainer::MBeatBoxListener::stateChanged( MInstrumentEvent anEvent )
{
	if( anEvent.getMessageType() == MBeatBox::MESSAGE_CHANNEL_SELECTED )
	{
		ivPtContainer->SetSelectedChannel( ivPtBeatBox->getSelection() );
	}
}

String MBeatBoxChannelControlContainer::MBeatBoxListener::getInstrumentListenerName()
{
	return "MBeatBoxChannelControlContainer::MBeatBoxListener";
}

String MBeatBoxChannelControlContainer::MBeatBoxListener::getClassName()
{
	return "MBeatBoxChannelControlContainer::MBeatBoxListener";
}

void MBeatBoxChannelControlContainer::MBeatBoxListener::valueChanged( MControlListenerEvent *anEvent )
{
	ivPtContainer->repaint();
}
