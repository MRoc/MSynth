/*

  MSong (C)2001 MRoc hifiShock

  A MSong contains some MBarContainer

*/
#include "MSong.h"
#include <gui/dialog/MMessageBox.h>

#ifdef __LIGHTVERSION
	#define MAX_PATTERNS 64
#else
	#define MAX_PATTERNS 640
#endif

//-----------------------------------------------------------------------------
// + CONSTRUCTION
//-----------------------------------------------------------------------------
MSong::MSong( IBpmControl* ptControl ) : 
	ivPtBpmControl( ptControl ),
	ivMaxBarCount( MAX_PATTERNS )
{
	MLogger::logMessage( "<msong::msong/>\n" );
}

//-----------------------------------------------------------------------------
// + DESTRUCTION
//-----------------------------------------------------------------------------
MSong::~MSong()
{
	MLogger::logMessage( "<msong::~msong/>\n" );

	fireSongDestroy();
	ivSongListeners.clear();
	deleteContent( true );
}

//-----------------------------------------------------------------------------
// + NEW SONG
//-----------------------------------------------------------------------------
void MSong::newSong()
{
	MLogger::logMessage( "<msong::newsong/>\n" );

	MSynchronize cls( &ivCriticalSection );

	deleteContent( true );

#ifdef __LIGHTVERSION
	for( unsigned int i=0;i<3;i++ )
		createDefaultSynth( false );
	for( i=0;i<1;i++ )
		createDefaultBeatBox( false );
#else
	for( unsigned int i=0;i<4;i++ )
		createDefaultSynth( false );
	for( i=0;i<2;i++ )
		createDefaultBeatBox( false );
#endif
	//createDefaultAudioChannel( false );

	fireModelChanged();
}

//-----------------------------------------------------------------------------
// + createBar
//-----------------------------------------------------------------------------
MBar* MSong::createBar( unsigned int row, unsigned int col, unsigned int resolution )
{
	MBar* ptBar = 0;

	if( row >= 0 && row < getBarContainerCount() &&
		col >= 0 && col < getMaxBarCount() )
	{
		ptBar = getBarContainer( row )->createDefaultBar();
		ptBar->setNotesPerBar( resolution );
		ptBar->setIndex( col );
		if( getBarContainer( row )->canInsertBar( ptBar, col ) )
			getBarContainer( row )->insertAt( ptBar );
		else
		{
			delete ptBar;
			ptBar = 0;
		}
	}

	return ptBar;
}

//-----------------------------------------------------------------------------
// + moveBar
//-----------------------------------------------------------------------------
bool MSong::moveBar( unsigned int fromRow, unsigned int fromCol, int rowDiv, int colDiv )
{
	bool back = false;

	unsigned int newRow = unsigned int(fromRow + rowDiv);
	unsigned int newCol = unsigned int(fromCol + colDiv);
	if(	newRow < getBarContainerCount() &&
		newCol < getMaxBarCount() )
	{
		MBarContainer *sourceBC = getBarContainer( fromRow );  // Source barContainer 
		MBarContainer *targetBC = getBarContainer( newRow ); // Target barContainer
		MBar* ptBar = sourceBC->getBarOverlappingAt( fromCol );
		ASSERT( ptBar && sourceBC && targetBC );
			
		if( targetBC->canInsertBar( ptBar, newCol ) )
		{
			sourceBC->unlink( ptBar );
			ptBar->setIndex( newCol );
			targetBC->insertAt( ptBar );

			// update instrument listener
			// revision: not required because MBarContainer forwards all events if nescassary
			ptBar->getNoteList()->updateFromInstrument( targetBC->getInstrument() );

			if( rowDiv != 0 )
				ptBar->getEventListCollection()->clear();
			back = true;
		}
	}

	return back;
}

//-----------------------------------------------------------------------------
// + glow
//-----------------------------------------------------------------------------
bool MSong::glow( unsigned int row, MBar* ptBar )
{
	ASSERT( ptBar != 0 );
	ASSERT( row < getBarContainerCount() );

	bool back = false;

	MSynchronize cls( &ivCriticalSection );

	MBar *nextBar = (MBar*) ptBar->getNext();
	if( nextBar )
	{
		ASSERT( nextBar->getNoteList() != 0 );

		MDefaultNoteList *noteList = ptBar->getNoteList();
		MDefaultNoteList *nextList = nextBar->getNoteList();
		ptBar->appendBar( nextBar );

		back = true;
	}

	return back;
}

//-----------------------------------------------------------------------------
// + cutBar
//-----------------------------------------------------------------------------
MBar* MSong::cutBar( unsigned int row, unsigned int index, MBar* ptBar )
{
	MBar* nextBar = 0;

	MSynchronize cls( &ivCriticalSection );

	nextBar = ptBar->cutBar( index );
	if( nextBar && nextBar->getNoteList() )
	{
		getBarContainer( row )->insertAt( nextBar );
	}

	return nextBar;
}

//-----------------------------------------------------------------------------
// + DELETE BAR CONTAINER
// Deletes the barcontainer at the given index
//-----------------------------------------------------------------------------
void MSong::deleteBarContainer( int index, bool sendNotify )
{
	MSynchronize cls( &ivCriticalSection );

	if( index != -1 )
	{
		ASSERT( ((unsigned int) index) < getBarContainerCount() );

		MBarContainer* ptContainer = removeBarContainer( index, sendNotify );
		delete ptContainer;
	}
}

//-----------------------------------------------------------------------------
// + swapBarContainer
// swaps two containes
//-----------------------------------------------------------------------------
void MSong::swapBarContainer( unsigned int index1, unsigned int index2 )
{
	MSynchronize cls( &ivCriticalSection );

	ASSERT( index1 < ivArBarContainers.size() );
	ASSERT( index2 < ivArBarContainers.size() );

	MBarContainer* temp = ivArBarContainers[ index1 ];
	ivArBarContainers[ index1 ] = ivArBarContainers[ index2 ];
	ivArBarContainers[ index2 ] = temp;

	unsigned int count = getBarContainerCount();
	for( unsigned int i=0;i<count;i++ )
		getBarContainer( i )->setRow( i );

	fireInstrumentsSwapped( index1, index2 );
}

//-----------------------------------------------------------------------------
// + resetAllInstruments
//-----------------------------------------------------------------------------
void MSong::resetAllInstruments()
{
	MSynchronize cls( &ivCriticalSection );

	MEvent event( MEvent::RESET );
	unsigned int count = getBarContainerCount();
	for( unsigned int i=0;i<count;i++ )
		getBarContainer( i )->getInstrument()->processEvent( &event );
}

//-----------------------------------------------------------------------------
// + setMaxBarCount
//-----------------------------------------------------------------------------
void MSong::setMaxBarCount( unsigned int maxBars )
{
	ivMaxBarCount = maxBars;
	for( BarContainerIter i=ivArBarContainers.begin();i!=ivArBarContainers.end();i++ )
		(*i)->setMaxBarCount( ivMaxBarCount );
}

//-----------------------------------------------------------------------------
// + getMaxBarCount
//-----------------------------------------------------------------------------
unsigned int MSong::getMaxBarCount()
{
	return ivMaxBarCount;
}

//-----------------------------------------------------------------------------
// + GET MIN BAR COUNT
// Calculated from a barContainers, returns the minimal possible barCount
//-----------------------------------------------------------------------------
unsigned int MSong::getMinBarCount()
{
	unsigned int minimalBarCount = 0;

	for( BarContainerIter i=ivArBarContainers.begin();i!=ivArBarContainers.end();i++ )
		if( minimalBarCount < (*i)->getMinBarCount() )
			minimalBarCount = (*i)->getMinBarCount();

	return minimalBarCount;
}

//-----------------------------------------------------------------------------
// # DD BAR CONTAINER (push back)
// To add a barcontainer to the song (an new channel)
//-----------------------------------------------------------------------------
void MSong::addBarContainer( MBarContainer *barContainer, bool sendNotify )
{
	ASSERT( barContainer );
	MSynchronize cls( &ivCriticalSection );

	barContainer->setRow( ivArBarContainers.size() );
	ivArBarContainers.push_back( barContainer );
	barContainer->setMaxBarCount( getMaxBarCount() );

	if( sendNotify )
		fireInstrumentAdded( ivArBarContainers.size() - 1 );
}

//-----------------------------------------------------------------------------
// + GET BAR CONTAINER
// Returns the barContainer at the specified index
//-----------------------------------------------------------------------------
MBarContainer *MSong::getBarContainer( unsigned int index )
{
	ASSERT( index < ivArBarContainers.size() );
	return ivArBarContainers[ index ];
}

//-----------------------------------------------------------------------------
// # EMOVE BAR CONTAINER
// Removes the barcontainer at the given index and returns it
//-----------------------------------------------------------------------------
MBarContainer *MSong::removeBarContainer( unsigned int index, bool sendNotify )
{
	MBarContainer *back = 0;
	MSynchronize cls( &ivCriticalSection );

	back = getBarContainer( index );
	ivArBarContainers.erase( ivArBarContainers.begin() + index );

	unsigned int count = getBarContainerCount();
	for( unsigned int i=0;i<count;i++ )
		getBarContainer( i )->setRow( i );

	if( sendNotify )
		fireInstrumentRemoved( index );

	return back;
}

//-----------------------------------------------------------------------------
// + GET BAR CONTAINER COUNT
// Returns the number of barContainers this song contains
//-----------------------------------------------------------------------------
unsigned int MSong::getBarContainerCount()
{
	return ivArBarContainers.size();
}

//-----------------------------------------------------------------------------
// + DELETE CONTENT
// Deletes all barContainers and their content
//-----------------------------------------------------------------------------
void MSong::deleteContent( bool sendNotify )
{
	MSynchronize cls( &ivCriticalSection );

	unsigned int barContainerCount = ivArBarContainers.size();
	for( unsigned int i=0;i<barContainerCount;i++ )
		deleteBarContainer( 0, sendNotify );

	// after removing -> fire model changed
	if( sendNotify )
		fireModelChanged();
}

/**
 * stores as tree node
 */
MTreeNode* MSong::save()
{
	MTreeNode* ptBack = new MTreeNode( "MSong" );
	unsigned int count = getBarContainerCount();
	for( unsigned int i=0;i<count;i++ )
		ptBack->addChild( getBarContainer( i )->save() );
	return ptBack;
}

/**
 * loads from the given tree node
 */
void MSong::load( MTreeNode* ptNode )
{
	MSynchronize cls( &ivCriticalSection );

	deleteContent( true );

	MTreeNodeIterator iter = ptNode->getIterator();
	while( iter.hasNext() )
	{
		MTreeNode* barContainerNode = iter.nextTreeNode();
		String nodeName = barContainerNode->getName();
		if( strcmp( nodeName, "bc" ) == 0 )
		{
			MBarContainer *newContainer = new MBarContainer();
			newContainer->load( barContainerNode, ivPtBpmControl );
			if( newContainer->getInstrument() != 0 )
			{
#ifdef __LIGHTVERSION
				if( getBarContainerCount() < 4 )
					addBarContainer( newContainer, false );
				else 
					delete newContainer;
#else
				addBarContainer( newContainer, false );
#endif
			}
			else
				MMessageBox::showError( "ERROR_LOADING_INSTRUMENT", MUtil::toString( getBarContainerCount() + 1 ) );
		}
	}

	MTargetRegistry::getInstance()->getIdRegistry()->clear();
	MIdMap::getInstance()->clear();

	fireModelChanged();
}

//-----------------------------------------------------------------------------
// + addSongListener (ISongListeners*)
//-----------------------------------------------------------------------------
void MSong::addSongListener( ISongListener* ptSongListener )
{
	ivSongListeners.addListener( ptSongListener );
};

//-----------------------------------------------------------------------------
// + REMOVE SONG LISTENER (ISongListeners)
//-----------------------------------------------------------------------------
void MSong::removeSongListener( ISongListener* ptSongListener )
{
	ivSongListeners.removeListener( ptSongListener );
};

//-----------------------------------------------------------------------------
// # FIRE INSTRUMENT ADDED
//-----------------------------------------------------------------------------
void MSong::fireInstrumentAdded( unsigned int index )
{
	unsigned int count = ivSongListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((ISongListener*)ivSongListeners.getListener(i))->instrumentAdded( index );
};

//-----------------------------------------------------------------------------
// # FIRE INSTRUMENT REMOVED
//-----------------------------------------------------------------------------
void MSong::fireInstrumentRemoved( unsigned int index )
{
	unsigned int count = ivSongListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((ISongListener*)ivSongListeners.getListener(i))->instrumentRemoved( index );
};

//-----------------------------------------------------------------------------
// # fireInstrumentsSwapped
//-----------------------------------------------------------------------------
void MSong::fireInstrumentsSwapped( unsigned int index1, unsigned int index2 )
{
	unsigned int count = ivSongListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((ISongListener*)ivSongListeners.getListener(i))->instrumentsSwapped( index1, index2 );
};

//-----------------------------------------------------------------------------
// # FIRE MODEL CHANGED
//-----------------------------------------------------------------------------
void MSong::fireModelChanged()
{
	unsigned int count = ivSongListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((ISongListener*)ivSongListeners.getListener(i))->songModelChanged();
};

//-----------------------------------------------------------------------------
// # FIRE SONG DESTROX
//-----------------------------------------------------------------------------
void MSong::fireSongDestroy()
{
	unsigned int count = ivSongListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((ISongListener*)ivSongListeners.getListener(i))->songDestroy();
};

//-----------------------------------------------------------------------------
// + CREATE DEFAULT SYNTH
//-----------------------------------------------------------------------------
void MSong::createDefaultSynth( bool sendNotify )
{
	MLogger::logMessage( "<msong::createdefaultsynth notify=\"%s\"/>\n",
		sendNotify ? "true" : "false" );

	MSynchronize cls( &ivCriticalSection );

#ifdef __LIGHTVERSION
	if( getBarContainerCount() >= 4 ) 
	{
		MMessageBox::showError( "LIMITED_CHANNELCOUNT_LE" );
		return;
	}
#endif
	Synth *ptSynth = new Synth();

	MBarContainer* ptContainer = new MBarContainer();
	ptSynth->setAutomationTracker( ptContainer );
	ptContainer->setInstrument( ptSynth );
	addBarContainer( ptContainer, sendNotify );

	MOsc* ptOsc = (MOsc*) MDefaultFactory::getInstance()->createObject( "MOscSaw" );
	MTransformer* ptTransformer1 = MTransformerRegistry::getInstance()->createTransformerByClass( "MIIR", 1 );
	MTransformer* ptTransformer2 = MTransformerRegistry::getInstance()->createTransformerByClass( "MEnvelope", 1 );

	ptSynth->setSoundSource( ptOsc );
	ptSynth->addTransformer( ptTransformer1 );
	ptSynth->addTransformer( ptTransformer2 );
};

//-----------------------------------------------------------------------------
// + CREATE DEFAULT BEATBOX
//-----------------------------------------------------------------------------
void MSong::createDefaultBeatBox( bool sendNotify )
{
	MLogger::logMessage( "<sequencer::createdefaultbeatbox notify=\"%s\"/>\n",
		sendNotify ? "true" : "false" );

	MSynchronize cls( &ivCriticalSection );

#ifdef __LIGHTVERSION
	if( getBarContainerCount() >= 4 ) 
	{
		MMessageBox::showError( "LIMITED_CHANNELCOUNT_LE" );
		return;
	}
#endif

	MBeatBox* ptDrumBox = new MBeatBox( ivPtBpmControl );
	//ptDrumBox->setId( getBarContainerCount() );
	MBarContainer* ptContainer = new MBarContainer();
	ptDrumBox->setAutomationTracker( ptContainer );
	ptDrumBox->makeDefaultPlayer();
	ptContainer->setInstrument( ptDrumBox );
	addBarContainer( ptContainer, sendNotify );
}

void MSong::createDefaultAudioChannel( bool shouldNotify )
{
	TRACE( "<Sequencer::createDefaultAudioChannel()>\n" );

	MSynchronize cls( &ivCriticalSection );

#ifdef __LIGHTVERSION
	if( getBarContainerCount() >= 4 ) 
	{
		MMessageBox::showError( "LIMITED_CHANNELCOUNT_LE" );
		return;
	}
#endif

	MAudioChannel* ptChannel = new MAudioChannel();
	//ptChannel->setId( getBarContainerCount() );
	MBarContainer* ptContainer = new MBarContainer();
	ptChannel->setAutomationTracker( ptContainer );
	ptContainer->setInstrument( ptChannel );
	addBarContainer( ptContainer, shouldNotify );
}