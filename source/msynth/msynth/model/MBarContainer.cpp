#include "MBarContainer.h"
#include <gui/dialog/MMessageBox.h>

#define DEFAULT_BAR_COUNT 640

INIT_RTTI( MBarContainer, "MBarContainer" );

/**
 * constructor
 */
MBarContainer::MBarContainer()
	: MList(),
	ivMaxBarCount( DEFAULT_BAR_COUNT ),
	ivPtInstrument( 0 ),
	ivRow( 0 )
{
	MTargetRegistry::getInstance()->addListener( this );
}

/**
 * destructor
 */
MBarContainer::~MBarContainer()
{
	MTargetRegistry::getInstance()->removeListener( this );

	if( ivPtInstrument )
	{
		ivPtInstrument->removeFinalizeListener( this );
		if( ivPtInstrument->noteListNeedsListener() )
			ivPtInstrument->removeInstrumentListener( this );
	}
	delete ivPtInstrument;
	clear();
}

/**
 * creates a default bar
 */
MBar* MBarContainer::createDefaultBar()
{
	ASSERT( ivPtInstrument != 0 );

	MDefaultNoteList* ptNoteList = MNoteFactory::getInstance()->getNoteList( ivPtInstrument->getNoteListId() );
	MBar* ptBar = new MBar( this );
	ptBar->setNoteList( ptNoteList );

	return ptBar;
}

/**
 * sets the used instrument. if instrument's notelists require
 * instrument notification, a IInstrumentListener is registered.
 */
void MBarContainer::setInstrument( MInstrument* ptInstrument )
{
	if( ivPtInstrument )
	{
		ivPtInstrument->removeFinalizeListener( this );
		if( ivPtInstrument->noteListNeedsListener() )
			ivPtInstrument->removeInstrumentListener( this );
	}

	ivPtInstrument = ptInstrument;

	if( ivPtInstrument )
	{
		ivPtInstrument->addFinalizeListener( this );
		if( ivPtInstrument->noteListNeedsListener() )
			ivPtInstrument->addInstrumentListener( this );
	}
}

/**
 * returns the used instrument
 */
MInstrument* MBarContainer::getInstrument()
{
	return ivPtInstrument;
}

/**
 * sets the highest usable pattern index
 */
void MBarContainer::setMaxBarCount( unsigned int maxBarCount )
{
	ivMaxBarCount = maxBarCount;
}

/**
 * returns the highest usable pattern index
 */
unsigned int MBarContainer::getMaxBarCount()
{
	return ivMaxBarCount;
}

//-----------------------------------------------------------------------------
// + GET MIN COUNT
// Calculated from the last bar
//-----------------------------------------------------------------------------
unsigned int MBarContainer::getMinBarCount()
{
	unsigned int minLength = 0;

	MBar* bar = (MBar*) getFirstItem();

	while(
		bar != 0 &&
		bar->getNext() != 0 )
	{
		bar = (MBar*) bar->getNext();
	}

	if( bar )
	{
		minLength = bar->getStopsAt() + 1;
	}

	return minLength;
}

//-----------------------------------------------------------------------------
// + GET BAR COUNT
// Returns the number of bars
//-----------------------------------------------------------------------------
unsigned int MBarContainer::getBarCount()
{
	MBar *ptBar = (MBar*) getFirstItem();
	unsigned int count = 0;

	while( ptBar != 0 )
	{
		count++;
		ptBar = (MBar*) ptBar->getNext();
	}

	return count;
}

//-----------------------------------------------------------------------------
// + getItemAt
// Returns the bar number 'index' 
// Handle with care!!!
//-----------------------------------------------------------------------------
MListItem* MBarContainer::getItemAt( int index )
{
	MBar *ptBar = (MBar*) getFirstItem();
	int count = 0;
	while( count < index )
	{
		count++;
		ptBar = (MBar*) ptBar->getNext();
	}

	return ptBar;
}

//-----------------------------------------------------------------------------
// + getBarStartingAt
// returns the bar that starts at the given index
//-----------------------------------------------------------------------------
MBar* MBarContainer::getBarStartingAt( int barNo )
{
	MBar *ptBar = (MBar*) getFirstItem();

	while(
		ptBar != 0 &&
		ptBar->getIndex() != barNo )
	{
		ptBar = (MBar*) ptBar->getNext();
	}

	return ptBar;
}

//-----------------------------------------------------------------------------
// + CAN INSERT AT
// Returns true if a bar can be pasted at the specified index.
// Checks that the new bar does not overlapp existing or the end of the container.
//-----------------------------------------------------------------------------
bool MBarContainer::canInsertBar( MBar *bar, int pasteIndex )
{
	ASSERT( ivPtInstrument != 0 );
	ASSERT( bar != 0 );
	ASSERT( bar->getNoteList() != 0 );

	if( bar->getNoteList()->getTypeId() != ivPtInstrument->getNoteListId() )
	{
		return false;
	}

	bool canDrop = true;

	int lengthOfNewBar = bar->getLength();
	unsigned int pasteIndexEnd = pasteIndex + lengthOfNewBar - 1;

	if( pasteIndexEnd < ivMaxBarCount )
	{
		MBar *next = (MBar*) getFirstItem();
		while( next != 0 && canDrop )
		{
			if( next != bar ) // dont compare new bar is next( dragging )
			{
				if( isOverlapping( pasteIndex, pasteIndexEnd,
						next->getIndex(), next->getStopsAt() ) )
				{ // new bar would overlapp an existing
					canDrop = false;
				}
			}
			next = (MBar*) next->getNext();
		}
	}
	else
	{ // new bar would overlapp the end of the barContainer
		canDrop = false;
	}

	return canDrop;
}

//-----------------------------------------------------------------------------
// # IS OVERLAPPING
// Tests wether the indexes1 overlapping the indexes2
//-----------------------------------------------------------------------------
bool MBarContainer::isOverlapping( int start1, int end1, int start2, int end2 )
{
	bool back = true;

	TRACE( "[start1(%i) end1(%i) start2(%i) end2(%i)]\n",
		start1,
		end1,
		start2,
		end2 );
	if( (start1 < start2 && end1 < start2 ) ||
		(start1 > end2 ) )
	{
		back = false;
	}

	TRACE( "[is overlapping? %s]\n", ( back ? "true" : "false" ) );

	return back;
}

//-----------------------------------------------------------------------------
// + IS FREE
// Returns true if a bar exists at the specified index
// means checks for overlapping bars
//-----------------------------------------------------------------------------
bool MBarContainer::isFree( unsigned int index )
{
	bool free = true;

	MBar *next = (MBar*) getFirstItem();
	while( next != 0 && free )
	{
		if( ( next->getIndex() <= (int)index ) &&
			( next->getStopsAt() >= (int)index ) )
		{
			free = false;
		}
		next = (MBar*) next->getNext();
	}

	return free;
}

//-----------------------------------------------------------------------------
// + insertAt
// overrides insertAt of MList to set the row index
//-----------------------------------------------------------------------------
void MBarContainer::insertAt( MListItem *item )
{
	MBar* ptBar = ((MBar*)item);
	ptBar->setRow( ivRow );
	ptBar->setTracker( this );
	MList::insertItem( item );
}

//-----------------------------------------------------------------------------
// + GET MAX POSSIBLE LENGTH
// Returns the maximal barLength the given bar can have
//-----------------------------------------------------------------------------
int MBarContainer::getMaxPossibleLength( MBar* ptBar )
{
	MBar* ptNextBar = (MBar*) ptBar->getNext();
	int lastPossibleStopAt;
	if( ptNextBar )
	{
		lastPossibleStopAt = ptNextBar->getIndex();
	}
	else
	{
		lastPossibleStopAt = ivMaxBarCount;
	}

	return lastPossibleStopAt - ptBar->getIndex();
}

/**
 * stores as tree node
 */
MTreeNode* MBarContainer::save()
{
	MTreeNode* ptBack = new MTreeNode( "bc" );

	ptBack->addChild( ivPtInstrument->save() );

	MBar *nextBar = (MBar*) getFirstItem();
	while( nextBar != 0 )
	{
		ptBack->addChild( nextBar->save() );
		nextBar = (MBar*) nextBar->getNext();
	}

	return ptBack;
}


/**
 * loads from the given tree node
 */
void MBarContainer::load( MTreeNode* ptNode, IBpmControl* ptControl )
{
	clear();

	MTreeNodeIterator iter = ptNode->getIterator();
	while( iter.hasNext() )
	{
		MTreeNode* barNode = iter.nextTreeNode();
		String nodeName = barNode->getName();

		if( strcmp( nodeName, "bar") == 0)
		{
			MBar *newBar = new MBar( this );
			newBar->load( barNode );
			insertAt( newBar );
		}
		else if( strcmp( nodeName, "MSoundSource" ) == 0 || 
			strcmp( nodeName, "source" ) == 0 )
		{
			String valueString = barNode->getAttribute( "value" );

			MInstrument *ptInstrument = 0;
			if( strcmp(valueString, "MSynth") == 0 )
				ptInstrument = new Synth();
			else if( strcmp(valueString, "MDrumBox") == 0 )
				ptInstrument = new MBeatBox( ptControl );
			else if( strcmp(valueString, "MAudioChannel") == 0 )
				ptInstrument = new MAudioChannel();

			if( ptInstrument )
			{
				ptInstrument->setAutomationTracker( this );
				ptInstrument->load( barNode );
				setInstrument( ptInstrument );
			}
			else
			{
				MMessageBox::showError( "ERROR_CREATING_INSTRUMENT", valueString );
			}
		}
	}
}

//-----------------------------------------------------------------------------
// + GET BAR OVERLAPPING AT
// Returns the bar that overlapps the given index
//-----------------------------------------------------------------------------
MBar* MBarContainer::getBarOverlappingAt( int index )
{
	MBar *ptBar = (MBar*) getFirstItem();

/*	while(	ptBar != 0 )
		if( ((ptBar->getIndex() <= index ) && (ptBar->getStopsAt() >= index )) ||
			ptBar->getIndex() > index )
			break;
		else
			ptBar = (MBar*) ptBar->getNext();*/

	while(	ptBar != 0 )
		if( (ptBar->getIndex() <= index ) && (ptBar->getStopsAt() >= index ) )
			break;
		else
			ptBar = (MBar*) ptBar->getNext();


	return ptBar;
}

//-----------------------------------------------------------------------------
// + GET NOTE STARTS AT
// Returns the note lieing on this systemstep, null if no one was found
//-----------------------------------------------------------------------------
MDefaultNote* MBarContainer::getNoteStartsAt( int barAt, int noteStartsAt )
{
	MDefaultNote* ptNote = 0;

	// Search the bar overlapping the given location
	MBar *ptBar = getBarOverlappingAt( barAt );

	if( ptBar != 0 )
	{
		int notesPerBar = ptBar->getNotesPerBar();
		int div = 32 / notesPerBar;
		if( noteStartsAt % div == 0 )
		{
			int barOffset = barAt - ptBar->getIndex();
			int noteOffset = barOffset * notesPerBar;
			ptNote = (MDefaultNote*) ptBar->getNoteList()->getItemAt( noteOffset + (noteStartsAt/div) );
		}
	}

	return ptNote;
}

void MBarContainer::triggerAutomationEvents( unsigned int pattern, unsigned int step )
{
	MSynchronize cls( &ivSync );

	// Search the bar overlapping the given location
	MBar *ptBar = getBarOverlappingAt( pattern );
	if( ptBar != 0 )
	{
		unsigned int stepsPerBar = ptBar->getNotesPerBar();
		unsigned int div = 32 / stepsPerBar;
		if( step % div == 0 )
		{
			unsigned int barOffset = pattern - ptBar->getIndex();
			unsigned int stepOffset = barOffset * stepsPerBar;

			IEventListCollection* ptListCollection =
				ptBar->getEventListCollection();
			ASSERT( ptListCollection );
			ptListCollection->triggerEvents( stepOffset + (step/div) );
		}
	}
}

//-----------------------------------------------------------------------------
// + GET NOTE STOPS AT
// Returns the note stoping on this systemstep, null if no one was found
//-----------------------------------------------------------------------------
MDefaultNote* MBarContainer::getNoteStopsAt( int barAt, int noteStopsAt )
{
	MDefaultNote* ptNote = 0;

	// Search the bar overlapping the given location
	MBar *ptBar = getBarOverlappingAt( barAt );

	if( ptBar != 0 )
	{
		int notesPerBar = ptBar->getNotesPerBar();
		int div = 32 / notesPerBar;
		if( noteStopsAt % div == 0 )
		{
			int barOffset = barAt - ptBar->getIndex();
			int noteOffset = barOffset * notesPerBar;
			ptNote = (MDefaultNote*) ptBar->getNoteList()->getItemStopsAt( noteOffset + (noteStopsAt/div) );
		}
	}

	return ptNote;
}

//-----------------------------------------------------------------------------
// + stateChanged (inheritanced from IInstrumentListener)
//	loops the messages from an instrument to the notelists
//-----------------------------------------------------------------------------
void MBarContainer::stateChanged( MInstrumentEvent anEvent )
{
	TRACE( "<mbarcontainer::statechanged event=\"%i\"/>\n", anEvent.getMessageType() );

	MBar* ptBar = (MBar*)getFirstItem();

	while( ptBar != 0 )
	{
		MDefaultNoteList* noteList = ptBar->getNoteList();
		if( noteList->getInstrumentListener() )
		{
			noteList->getInstrumentListener()->stateChanged( anEvent );
		}
		ptBar = (MBar*) ptBar->getNext();
	}
}

//-----------------------------------------------------------------------------
// + getInstrumentListenerName (inheritanced from IInstrumentListener)
//-----------------------------------------------------------------------------
String	MBarContainer::getInstrumentListenerName()
{
	return String( "MBarContainer" );
}

//-----------------------------------------------------------------------------
// + setRow
// calls set row in all contained bars
//-----------------------------------------------------------------------------
void MBarContainer::setRow( unsigned int index )
{
	ivRow = index;

	MBar* ptBar = (MBar*)getFirstItem();
	while( ptBar != 0 )
	{
		ptBar->setRow( index );
		ptBar = (MBar*) ptBar->getNext();
	}
}

//-----------------------------------------------------------------------------
// + getRow
//-----------------------------------------------------------------------------
unsigned int MBarContainer::getRow()
{
	return ivRow;
}

//-----------------------------------------------------------------------------
// + addAutomationEvent
// inserts the automation event in the event list at the given position
//-----------------------------------------------------------------------------
void MBarContainer::addAutomationEvent( IAutomationEvent* ptEvent,
	IEventTarget* ptTarget, unsigned int pattern, unsigned int systemStep )
{
	ASSERT( ptEvent );

	MSynchronize cls( &ivSync );

	// TO DO
	MBar* ptBar;
	if( (ptBar=this->getBarOverlappingAt( pattern )) != 0 )
	{
		unsigned int offset = pattern - ptBar->getIndex();

		IEventListCollection* ptListCollection =
			ptBar->getEventListCollection();
		ASSERT( ptListCollection );

		if( ptBar->getNotesPerBar() != 32 && systemStep % 2 != 0 )
		{
			TRACE("[releasing event, no space to insert event! pattern(%i) step(%i)]\n"
				,pattern, systemStep );
			ptEvent->release();
		}
		else
		{
			if( ptBar->getNotesPerBar() == 16 )
				systemStep /= 2;
			TRACE("[recording event pattern(%i) step(%i)]\n", pattern, systemStep);

			ptListCollection->addAutomationEvent(
				ptEvent,
				ptTarget,
				offset * ptBar->getNotesPerBar() + systemStep );
		}
	}
	else
	{ // could not record
		TRACE("[releasing event, no overlapping bar! pattern(%i) step(%i)]\n"
			,pattern, systemStep);
		ptEvent->release();
	}
}

//-----------------------------------------------------------------------------
// + removeAutomationEvent
// removes the given automation event
//-----------------------------------------------------------------------------
void MBarContainer::removeAutomationEvent( IAutomationEvent* ptEvent )
{
	MSynchronize cls( &ivSync );

	MListItem* ptItem = getFirstItem();
	while( ptItem != 0 )
	{
		MBar* ptBar = ((MBar*)ptItem);
		ptBar->getEventListCollection()->removeAutomationEvent( ptEvent );
		ptItem = ptItem->getNext();
	}
}

//-----------------------------------------------------------------------------
// + removeAutomationEvents
// removes all automation events for this control
//-----------------------------------------------------------------------------
void MBarContainer::removeAutomationEvents( IEventTarget* ptTarget )
{
	MSynchronize cls( &ivSync );

	MListItem* ptItem = getFirstItem();
	while( ptItem != 0 )
	{
		MBar* ptBar = ((MBar*)ptItem);
		ptBar->getEventListCollection()->removeAutomationEventList( ptTarget );
		ptItem = ptItem->getNext();
	}
}

void MBarContainer::automationAdded( IEventTarget* ptTarget )
{
	if( hasTarget( ptTarget ) )
	{
		MListItem* ptItem = getFirstItem();
		while( ptItem != 0 )
		{
			MBar* ptBar = (MBar*) ptItem;
			//ptBar->getEventListCollection()
			ptItem = ptItem->getNext();
		}
	}
}

void MBarContainer::automationRemoved( IEventTarget* ptTarget )
{
}

bool MBarContainer::hasTarget( IEventTarget* ptTarget )
{
	return hasTarget( ivPtInstrument, ptTarget );
}

bool MBarContainer::hasTarget( IControlCollection* ptCollection, IEventTarget* ptTarget )
{
	bool back = false;

	unsigned int count = ptCollection->getControlCount();
	for( unsigned int i=0;i<count;i++ )
	{
		if( ptCollection->getControl( i ) )
		{
			IEventTarget* pTarget2 = (IEventTarget*) ptCollection->getControl( i )->getInterface( "IEventTarget" );
			if( pTarget2 == ptTarget )
			{
				back = true;
				break;
			}
		}
		else
			MLogger::logWarning(
				"<mbarcontainer::hastarget collection=\"%s\" index=\"%i\"/>\n",
				ptCollection->getName().getData(),
				i );
	}

	count = ptCollection->getChildCount();
	for( i=0;(i<count) && (back != true);i++ )
		back = hasTarget( ptCollection->getChild( i ), ptTarget );

	return back;
}

void MBarContainer::collectTargets( std::vector<IEventTarget*> &collector )
{
	collectTargets( ivPtInstrument, collector );
}

void MBarContainer::collectTargets( IControlCollection* ptCollection, std::vector<IEventTarget*> &collector )
{
	unsigned int count = ptCollection->getControlCount();
	for( unsigned int i=0;i<count;i++ )
	{
		IEventTarget* ptTarget = (IEventTarget*) ptCollection->getControl( i )->getInterface("IEventTarget");
		if( ptTarget &&
			MTargetRegistry::getInstance()->isInAutomation( ptTarget ) )
		{
			collector.push_back( ptTarget );
		}
	}
	count = ptCollection->getChildCount();
	for( i=0;i<count;i++ )
		collectTargets( ptCollection->getChild( i ), collector );
}

/**
 * called if the object is deleted, after recievieing this message,
 * the object MUST deregister itself as listener because the observed
 * object will be deleted, if not there it will result in an endless loop!!!
 */
void MBarContainer::objectTerminated( void* obj )
{
	this->setInstrument( 0 );
}