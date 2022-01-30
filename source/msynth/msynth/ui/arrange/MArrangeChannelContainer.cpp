#include "MArrangeChannelContainer.h"

INIT_RTTI( MArrangeChannelContainer::MButtonListener, "MArrangeChannelContainer::MButtonListener" );

MArrangeChannelContainer::MArrangeChannelContainer( MSong* ptSong ) :
	MWndCollection(),
	ivPtSong( 0 ),
	ivPtSelectedControl( 0 ),
	ivMouseDragged( false ),
	ivPartSize( MSize( 30, 20 ) )
{
	ivPtButtonListener = new MButtonListener( this );
	setSong( ptSong );
}

MArrangeChannelContainer::~MArrangeChannelContainer()
{
	while( ivPtChannelControls.size() > 0 )
		deleteChannelAt( 0 );

	SAFE_DELETE( ivPtButtonListener );
}

void MArrangeChannelContainer::paint( IGraphics* ptGraphics,const  MRect &rect )
{
	ptGraphics->clipRect(
		MRect(
			0,
			0,
			rect.getWidth(),
			ivPtChannelControls.size() * ivPartSize.getHeight() ) );
	ptGraphics->fillRect( rect, ivBkColor );
}

void MArrangeChannelContainer::builtGui()
{
	setVisible( false );

	ivPtSelectedControl = 0;

	while( ivPtChannelControls.size() > 0 )
		deleteChannelAt( 0 );
	
	unsigned int count = (unsigned int) ivPtSong->getBarContainerCount();
	for( unsigned int i=0;i<count;i++ )
	{
		ivPtChannelControls.push_back( 0 );
		createChannelAt( i ); 
	}

	setVisible( true );
};

void MArrangeChannelContainer::closeChildFrames()
{
	unsigned int count = (unsigned int) ivPtChannelControls.size();
	for( unsigned int i=0;i<count;i++ )
		ivPtChannelControls[ i ]->CloseChildFrame();
}

void MArrangeChannelContainer::updateFromModel()
{
	MLogger::logMessage( "<marrangechannelcontainer::updatefrommodel/>\n" );
	unsigned int count = (unsigned int) ivPtChannelControls.size();
	for( unsigned int i=0;i<count;i++ )
		((MArrangeChannelControl*)ivPtChannelControls[ i ])->updateFromModel();
}

void MArrangeChannelContainer::doLayout()
{
	//setVisible( false );
	int oldY = 0;
	MRect rect = getRect();
	unsigned int count = (unsigned int) ivPtChannelControls.size();
	for( unsigned int i=0;i<count;i++ )
	{
		MArrangeChannelControl *control = ivPtChannelControls[ i ];
		if( control )
			control->setRect(
				MRect(
					0,
					i * ivPartSize.getHeight(),
					rect.getWidth(),
					ivPartSize.getHeight() ) );
	}
	//setVisible( true );
}

void MArrangeChannelContainer::setSong( MSong* ptSong )
{
	ivPtSong = ptSong;
}

MSong* MArrangeChannelContainer::getSong()
{
	return ivPtSong;
}

void MArrangeChannelContainer::setPartSize( MSize size )
{
	ivPartSize = size;
}

MSize MArrangeChannelContainer::getPartSize()
{
	return ivPartSize;
}

void MArrangeChannelContainer::SetSelectedChannel( int selectedIndex )
{
	//TRACE( "<MArrangeChannelContainer::SetSelectedChannel %i>\n", selectedIndex );

	if( ivPtSelectedControl )
		ivPtSelectedControl->setSelected( false );
	ivPtSelectedControl = ivPtChannelControls[ selectedIndex ];
	ivPtSelectedControl->setSelected( true );
}

int MArrangeChannelContainer::GetSelectedChannel()
{
	int index = -1;

	unsigned int count = (unsigned int) ivPtChannelControls.size();
	for( unsigned int i=0;i<count;i++ )
		if( ivPtChannelControls[i]->getSelected() )
		{
			index = i;
			break;
		}

	return index;
}

void MArrangeChannelContainer::onMouseUp( MMouseEvent* anEvent )
{
	if( ivMouseDragged )
	{
		ReleaseCapture();
		ivMouseDragged = false;
	}
}

void MArrangeChannelContainer::onMouseMove( MMouseEvent* anEvent )
{
	MPoint point = anEvent->getPoint();
	int currentIndex =  point.getY() / ivPartSize.getHeight();
	if( ivMouseDragged &&
		currentIndex >= 0 &&
		currentIndex != ivLastIndex &&
		currentIndex < int(ivPtSong->getBarContainerCount()) )
	{
		TRACE( "<MArrangeChannelContainer::OnMouseMove( unsigned int nHitTest, CPoint point )->SWAP>\n" );
		ivPtSong->swapBarContainer( ivLastIndex, currentIndex );
		ivLastIndex = currentIndex;
		SetSelectedChannel( ivLastIndex );
	}
}

void MArrangeChannelContainer::onChannelAdded()
{
	ivPtChannelControls.push_back( 0 );
	createChannelAt( ivPtChannelControls.size() - 1 );
	doLayout();
}

void MArrangeChannelContainer::onChannelRemoved( unsigned int index )
{
	ASSERT( index >= 0 && index < ivPtChannelControls.size() );

	MArrangeChannelControl* ptTemp = ivPtChannelControls[ index ];
	if( ivPtSelectedControl == ptTemp )
		ivPtSelectedControl = 0;

	deleteChannelAt( index );

	doLayout();
}

void MArrangeChannelContainer::onChannelsSwapped( unsigned int index1, unsigned int index2 )
{
	MArrangeChannelControl* ptTemp = ivPtChannelControls[ index1 ];
	ivPtChannelControls[ index1 ] = ivPtChannelControls[ index2 ];
	ivPtChannelControls[ index2 ] = ptTemp;

	doLayout();
}

void MArrangeChannelContainer::createChannelAt( unsigned int index )
{
	ASSERT( index >= 0 && index < ivPtChannelControls.size() );
	ASSERT( ivPtSong->getBarContainer( index )->getInstrument() != 0 );

	if(	ivPtChannelControls[ index ] )
	{
		MArrangeChannelControl* ptTemp = ivPtChannelControls[ index ];
		removeChild( ptTemp );
		ptTemp->removeActionListener( this->ivPtButtonListener );
		ivPtChannelControls[ index ] = 0;
		delete ptTemp;
	}

	MArrangeChannelControl *newChannel = new MArrangeChannelControl(ivPtSong->getBarContainer( index )->getInstrument());
	newChannel->setBkColor( MColorMap::BK_COLOR3 );
	newChannel->setBkColorSelected( MColorMap::FG_COLOR3 );
	newChannel->setRect(
		MRect(
			0,
			index * ivPartSize.getHeight(),
			getRect().getWidth(), 
			ivPartSize.getHeight() ) );
	ivPtChannelControls[ index ] = newChannel;
	newChannel->addActionListener( this->ivPtButtonListener );
	addChild( newChannel );
}

void MArrangeChannelContainer::deleteChannelAt( unsigned int index )
{
	ASSERT( index < this->ivPtChannelControls.size() );

	MArrangeChannelControl* ptControl = ivPtChannelControls[ index ];
	removeChild( ptControl );
	ivPtChannelControls.erase( ivPtChannelControls.begin() + index );
	ptControl->removeActionListener( this->ivPtButtonListener );
	delete ptControl;
}

void MArrangeChannelContainer::storeChildFramePosition()
{
	unsigned int count = ivPtChannelControls.size();
	for( unsigned int i=0;i<count;i++ )
	{
		if( ivPtChannelControls[ i ] )
			ivPtChannelControls[ i ]->StoreEditorPosition();
	}
}

int MArrangeChannelContainer::getIndex( MArrangeChannelControl* ptControl )
{
	int back = -1;
	unsigned int count = this->ivPtChannelControls.size();
	for( unsigned int i=0;i<count;i++ )
		if( ivPtChannelControls[ i ] == ptControl )
		{
			back = i;
			break;
		}
	return back;
}

MArrangeChannelContainer::MButtonListener::MButtonListener( MArrangeChannelContainer* ptControl ) :
	ivPtControl( ptControl )
{
}
MArrangeChannelContainer::MButtonListener::~MButtonListener()
{
}
void MArrangeChannelContainer::MButtonListener::onActionPerformed( void* ptSrc )
{
	MArrangeChannelControl* ptcontrol = (MArrangeChannelControl*) ptSrc;

	int index = ivPtControl->getIndex( ptcontrol );
	if( index >= 0 && index <= int( ivPtControl->ivPtSong->getBarContainerCount() ) )
	{
		ivPtControl->SetSelectedChannel( index );
		if( ! ivPtControl->ivMouseDragged )
		{
			ivPtControl->ivLastIndex = index;
			ivPtControl->ivMouseDragged = true;
		}
		else
			ReleaseCapture();
	}
}