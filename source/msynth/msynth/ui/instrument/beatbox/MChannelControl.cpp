/*
	(C)2000-2002 MRoc hifiShock
*/
#include "MChannelControl.h"

INIT_RTTI( MChannelControl::MBeatBoxListener, "MChannelControl::MBeatBoxListener" );
INIT_RTTI( MChannelControl::MActionListener, "MChannelControl::MActionListener" );

MChannelControl::MChannelControl() :
	MWndCollection(),
	ivPtListener( 0 )
{
	ivBkColor = MColorMap::BK_COLOR1;
	ivPtBeatBox = 0;
	ivPtActionListener = new MActionListener( this );
}

MChannelControl::~MChannelControl()
{
	deleteControls();
	SAFE_DELETE( ivPtActionListener );
	SAFE_DELETE( ivPtListener );
}

void MChannelControl::deleteControls()
{
	while( ivArControls.size() > 0 )
	{
		MWnd* ptWnd = ivArControls[ 0 ];
		MWavePlayerControl* ptControl = ivArControls[0];
		ptControl->removeActionListener( ivPtActionListener );
		ivArControls.erase( ivArControls.begin() );
		removeChild( ptWnd );
		delete ptWnd;
	}
}

void MChannelControl::updateGui()
{
	ASSERT( ivPtBeatBox != 0 );
	setVisible( false );
	deleteControls();
	unsigned int channelCount = ivPtBeatBox->getWavePlayerCount();
	if( channelCount )
	{
		for( unsigned int i=0;i<channelCount;i++ )
			createControl( i );
		((MScrollPane*)getParent())->setScrollSize( getPreferredSize() );
	}
	setVisible( true );
}

MSize MChannelControl::getPreferredSize()
{
	return
		MSize(
			ivPtBeatBox->getWavePlayerCount() * (MWavePlayerControl::WPC_DEFAULT_WIDTH + 1) + 4,
			MWavePlayerControl::WPC_DEFAULT_HEIGHT + 4 );
}

void MChannelControl::createControl( unsigned int index )
{
	ASSERT( ivPtBeatBox->getWavePlayerAt( index ) );
	MWavePlayerControl* ptWnd = new MWavePlayerControl(
		ivPtBeatBox->getWavePlayerAt( index ),
		index );
	ptWnd->addActionListener( this->ivPtActionListener );
	ptWnd->setRect(
		MRect(
			2 + index * 60,
			2,
			ptWnd->getWidth(),
			ptWnd->getHeight() ) );
	ivArControls.push_back( ptWnd );
	addChild( ptWnd );
}

void MChannelControl::updateFromModel()
{
	setActiveChannel( ivPtBeatBox->getSelectedChannel() );
}

void MChannelControl::setActiveChannel( MObject* selectedObject )
{
	unsigned int count = ivPtBeatBox->getWavePlayerCount();
	for( unsigned int i=0;i<count;i++ )
        ivArControls[i]->setActiveChannel( ivArControls[i]->getWavePlayer() == selectedObject );
}

void MChannelControl::setBeatBox(MBeatBox *ptDrumBox)
{
	if( ivPtListener )
		SAFE_DELETE( ivPtListener );

	ivPtBeatBox = ptDrumBox;

	if( ivPtBeatBox )
		ivPtListener = new MBeatBoxListener( ivPtBeatBox, this );
}

MBeatBox *MChannelControl::getBeatBox()
{
	return ivPtBeatBox;
}

void MChannelControl::onUpdateFromModel()
{
	updateGui();
	updateFromModel();
}

void MChannelControl::onWaveplayerAdded( unsigned int index )
{
	createControl( this->ivArControls.size() );
	((MScrollPane*)getParent())->setScrollSize( getPreferredSize() );
}

void MChannelControl::onWaveplayerRemoved( unsigned int index )
{
	MWavePlayerControl* ptControl = ivArControls[ index ];
	ASSERT( index < ivArControls.size() );
	ivArControls.erase( ivArControls.begin() + index );
	removeChild( ptControl );
	delete ptControl;
	unsigned int count = ivArControls.size();
	for( unsigned int i = 0;i<count;i++ )
	{
		ivArControls[ i ]->setId( i );
		layoutControl( ivArControls[ i ], i );
	}
	((MScrollPane*)getParent())->setScrollSize( getPreferredSize() );
	repaint();
}

void MChannelControl::onSelectedChannelChanged( unsigned int index )
{
	setActiveChannel( ivPtBeatBox->getSelectedChannel() );
}

void MChannelControl::doLayout()
{
	unsigned int count = ivArControls.size();
	for( unsigned int i=0;i<count;i++ )
		layoutControl( ivArControls[ i ], i );

	repaint();
}

void MChannelControl::layoutControl( MWavePlayerControl* ptControl, unsigned int index )
{
	MSize d = ptControl->getSize();
	ptControl->setRect(
		MRect(
			2 + index*60,
			2,
			d.getWidth(),
			d.getHeight() ) );
}

MChannelControl::MBeatBoxListener::MBeatBoxListener( MBeatBox* ptBeatBox, MChannelControl* ptChannelControl ) :
	ivPtBeatBox( ptBeatBox ),
	ivPtControl( ptChannelControl )
{
	ivPtBeatBox->addInstrumentListener( this );
}

MChannelControl::MBeatBoxListener::~MBeatBoxListener()
{
	ivPtBeatBox->removeInstrumentListener( this );
}

void MChannelControl::MBeatBoxListener::stateChanged( MInstrumentEvent anEvent )
{
	int type = anEvent.getMessageType();
	if( type == MBeatBox::MESSAGE_CHANNEL_ADDED )
		ivPtControl->onWaveplayerAdded( ivPtBeatBox->getChannelCount() - 1 );
	else if( type == MBeatBox::MESSAGE_CHANNEL_REMOVED )
		ivPtControl->onWaveplayerRemoved( (unsigned int) anEvent.getValue() );
	else if( type == MBeatBox::MESSAGE_CHANNEL_SELECTED )
		ivPtControl->onSelectedChannelChanged( (unsigned int) anEvent.getValue() );
	else if( type == MBeatBox::MESSAGE_UPDATE_FROM_MODEL )
		ivPtControl->onUpdateFromModel();
}

String MChannelControl::MBeatBoxListener::getInstrumentListenerName()
{
	return "MChannelControl::MBeatBoxListener";
}

String MChannelControl::MBeatBoxListener::getClassName()
{
	return "MChannelControl::MBeatBoxListener";
}


MChannelControl::MActionListener::MActionListener( MChannelControl* ptControl ) :
	ivPtControl( ptControl )
{
}

MChannelControl::MActionListener::~MActionListener()
{
}

void MChannelControl::MActionListener::onActionPerformed( void* ptSrc )
{
	ivPtControl->ivPtBeatBox->setSelectedChannel( ((MWavePlayerControl*)ptSrc)->getWavePlayer() );
}
