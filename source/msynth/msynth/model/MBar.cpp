/**
 * (C) 2001-2003 MRoc
 */

#include "MBar.h"
#include <gui/dialog/MMessageBox.h>

#define DEFAULT_BAR_LENGTH 1
#define DEFAULT_NOTES_PER_BAR 16
#define MAX_NOTES_PER_BAR 64
#define MBAR_TAG_NAME String("bar")
#define NOTES_PER_BAR_TAG String("notesPerBar")
#define STARTS_AT_TAG String("startsAt")
#define STOPS_AT_TAG String("stopsAt")
#define ROW_TAG String("row")

int	MBar::TYPE_ID = 0;
char* MBar::TYPE_USER_NAME = "Bar";

MBar::MBar( IAutomationTracker* ptTracker ) :
	MListItem(),
	ivRow( 0 ),
	ivBarLength( DEFAULT_BAR_LENGTH ),
	ivNotesPerBar( DEFAULT_NOTES_PER_BAR ),
	ivPtNoteList( 0 ),
	ivPtAutomationEventList( 0 ),
	ivPtTracker( ptTracker ),
	ivPtProperties( 0 )
{
	ivPtProperties = new MProperties();
	setUp();
}

MBar::MBar(  IAutomationTracker* ptTracker, int noteCount ) :
	MListItem(),
	ivRow( 0 ),
	ivBarLength( DEFAULT_BAR_LENGTH ),
	ivNotesPerBar( DEFAULT_NOTES_PER_BAR ),
	ivPtNoteList( 0 ),
	ivPtAutomationEventList( 0 ),
	ivPtTracker( ptTracker ),
	ivPtProperties( 0 )
{
	ivPtProperties = new MProperties();
	setLength( int(noteCount/DEFAULT_NOTES_PER_BAR) );
}

MBar::MBar( MBar *bar ) :
	MListItem( bar->getIndex() ),
	ivRow( bar->getRow() ),
	ivBarLength( bar->getLength() ),
	ivNotesPerBar( bar->getNotesPerBar() ),
	ivPtNoteList( 0 ),
	ivPtAutomationEventList( 0 ),
	ivPtTracker( bar->getTracker() ),
	ivPtProperties( 0 )
{
	ivPtProperties = new MProperties();
	ivPtNoteList = (MDefaultNoteList*) bar->getNoteList()->getCopy();
	setEventListCollection( bar->getEventListCollection()->clone() );
}

MBar::~MBar()
{
	fireBarReleasing();

 	SAFE_DELETE( ivPtNoteList );
	SAFE_DELETE( ivPtProperties );
	ivPtAutomationEventList->release();
}

void MBar::setNoteList( MDefaultNoteList *noteList )
{
	ivPtNoteList = noteList;
	setUp();
}

MDefaultNoteList *MBar::getNoteList()
{
	return ivPtNoteList;
}

void MBar::setIndex( int index )
{
	ivIndex = index;
	fireColChanged();
}

void MBar::setRow( int row )
{
	ivRow = row;
	fireRowChanged();
}

int MBar::getRow()
{
	return ivRow;
}

//-----------------------------------------------------------------------------
// + setNotesPerBar
//	Set Notes per bar (resolution), 1, 2, 4, 8, 16, 32, 64
//-----------------------------------------------------------------------------
void MBar::setNotesPerBar( int notesPerBar )
{
	if( notesPerBar != ivNotesPerBar )
	{
		ASSERT( ivPtNoteList );
		ASSERT( ivPtAutomationEventList );

		ivNotesPerBar = notesPerBar;
		unsigned int newNoteCount = getLength() * ivNotesPerBar;
		// new resolution means new noteListLength
		ivPtNoteList->changeResolution( newNoteCount );
		ivPtAutomationEventList->scale( newNoteCount );

		ASSERT( ivPtNoteList->getMaxLength() == ivPtAutomationEventList->getLength() );

		fireResolutionChanged();
	}
}

//-----------------------------------------------------------------------------
// + getNotesPerBar
//	Returns Notes per bar (resolution), 1, 2, 4, 8, 16, 32, 64
//-----------------------------------------------------------------------------
int MBar::getNotesPerBar()
{
	return ivNotesPerBar;
}

//-----------------------------------------------------------------------------
// + getLength
//-----------------------------------------------------------------------------
int MBar::getLength()
{
	return ivBarLength;
}

//-----------------------------------------------------------------------------
// + setLength
//-----------------------------------------------------------------------------
void MBar::setLength( int newBarLength )
{
	if( newBarLength != ivBarLength )
	{
		ivBarLength = newBarLength;
		if( ivPtNoteList )
		{
			unsigned int newNoteCount = ivBarLength * getNotesPerBar();
			ivPtNoteList->changeMaxLength( newNoteCount );
			ivPtAutomationEventList->scale( newNoteCount );
			this->fireLengthChanged();
		}

		if( ivPtNoteList && ivPtAutomationEventList )
			ASSERT( ivPtNoteList->getMaxLength() == ivPtAutomationEventList->getLength() );

		fireLengthChanged();
	}
}

//-----------------------------------------------------------------------------
// + getStopsAt
//	Returns the last index, this bar overlapps
//-----------------------------------------------------------------------------
int MBar::getStopsAt()
{
	return ivIndex + getLength() - 1;
}

//-----------------------------------------------------------------------------
// + setStopsAt
//-----------------------------------------------------------------------------
void MBar::setStopsAt( int stop )
{
	int newLength = stop - ivIndex + 1;
	setLength( newLength );
}

//-----------------------------------------------------------------------------
// + abbendBar
//	To append another container
//-----------------------------------------------------------------------------
void MBar::appendBar( MBar *bar )
{
	if( getNotesPerBar() != bar->getNotesPerBar() )
	{
		if( getNotesPerBar() > bar->getNotesPerBar() )
			bar->setNotesPerBar( getNotesPerBar() );
		else if( getNotesPerBar() < bar->getNotesPerBar() )
			setNotesPerBar( bar->getNotesPerBar() );
	}

	unsigned int newBarLength;
	if( (newBarLength=(bar->getIndex() - getStopsAt())) != getLength() )
	{
		unsigned int newBarLength =	(bar->getIndex() - ivIndex );
		TRACE( "[newBarLength=%i]\n", newBarLength );
		this->setLength( newBarLength ); 
	}

	setNext( bar->getNext() );
	if( bar->getNext() )
		bar->getNext()->setPrev( this );

	unsigned int newMaxLength = (getLength() + bar->getLength()) * getNotesPerBar();
	TRACE( "[newMaxLength=%i]\n", newMaxLength );
	
	unsigned int insertIndex = getLength() * getNotesPerBar();
	ivPtNoteList->insertAt(	bar->getNoteList(),
							insertIndex,
							newMaxLength );

	ivPtAutomationEventList->glow( bar->getEventListCollection() );

	setStopsAt( bar->getStopsAt() );

	TRACE( "[noteCount=%i eventCount=%i]\n",
		ivPtNoteList->getMaxLength(), 
		ivPtAutomationEventList->getLength() );
	ASSERT( ivPtNoteList->getMaxLength() == ivPtAutomationEventList->getLength() );

	delete bar;
}

//-----------------------------------------------------------------------------
// + cutBar
//	To cut the bar in two parts
//-----------------------------------------------------------------------------
MBar* MBar::cutBar( int at )
{
	TRACE( "<MBar::cutBar( at:%i )> startsAt: %i stopsAt: %i\n", at, ivIndex, getStopsAt() );
	MBar* back = 0;

	if( ivIndex != getStopsAt() &&
		getLength() > 1 && 
		at >= ivIndex &&
		at < getStopsAt() )
	{
		back = new MBar( ivPtTracker );
		back->setIndex( at + 1 );
		back->setRow( getRow() );
		back->setStopsAt( getStopsAt() );

		unsigned int cutAt = (at-ivIndex+1) * getNotesPerBar();
		MDefaultNoteList* newNoteList = ivPtNoteList->cutAt( cutAt );
		IEventListCollection* ptEventList = ivPtAutomationEventList->cut( cutAt );
		setStopsAt( at );
		back->setNoteList( newNoteList );	
		back->setEventListCollection( ptEventList );
		back->setNotesPerBar( getNotesPerBar() );
	}

	ASSERT( ivPtNoteList->getMaxLength() == ivPtAutomationEventList->getLength() );

	return back;
}

//-----------------------------------------------------------------------------
// + clear
//	Deletes all notes
//-----------------------------------------------------------------------------
void MBar::clear()
{
	SAFE_DELETE( ivPtNoteList );
}

//-----------------------------------------------------------------------------
// + save
//-----------------------------------------------------------------------------
MTreeNode* MBar::save()
{
	MTreeNode* ptBack = new MTreeNode( MBAR_TAG_NAME );

	ptBack->setAttribute( NOTES_PER_BAR_TAG, MInt::toString( getNotesPerBar() ) );
	ptBack->setAttribute( STARTS_AT_TAG, MInt::toString( ivIndex ) );
	ptBack->setAttribute( STOPS_AT_TAG, MInt::toString( getStopsAt() ) );
	ptBack->setAttribute( ROW_TAG, MInt::toString( getRow() ) );

	ptBack->addChild( getProperties()->MStr2ObjMap::save() );
	ptBack->addChild( getNoteList()->save() );
	ptBack->addChild( ivPtAutomationEventList->save() );

	return ptBack;
}

//-----------------------------------------------------------------------------
// + load
//-----------------------------------------------------------------------------
void MBar::load( MTreeNode* parentElement )
{
	clear(); 

	ivNotesPerBar = (
		MInt::parse(
			parentElement->getAttribute( NOTES_PER_BAR_TAG ),
			0,
			MAX_NOTES_PER_BAR,
			DEFAULT_NOTES_PER_BAR ) );

	setIndex(
		MInt::parse(
			parentElement->getAttribute( STARTS_AT_TAG ),
			0,
			640,
			0 ) );

	int stopsAt =
		MInt::parse(
			parentElement->getAttribute( STOPS_AT_TAG ),
			getStopsAt(),
			640,
			getStopsAt() );

	ivBarLength = stopsAt - ivIndex + 1;
	unsigned int stepLength = getLength() * getNotesPerBar();
	MTreeNodeIterator iter = parentElement->getIterator();
	while( iter.hasNext() )
	{
		MTreeNode* nl = iter.nextTreeNode();

		String nodeName = nl->getName();
		if( strcmp( nodeName, "MDefaultNoteList" ) == 0 )
		{
			int noteListId = MInt::parse( nl->getAttribute( "id" ) );

			MDefaultNoteList* ptNoteList = 0;
			if( noteListId == MDefaultSynthNoteList::TYPE_ID )
			{
				ptNoteList = new MDefaultSynthNoteList();
			}
			else if( noteListId == MDefaultMessageNoteList::TYPE_ID )
			{
				ptNoteList = new MDefaultMessageNoteList();
				ASSERT( false );
			}
			else if( noteListId == MBeatBoxNoteList::TYPE_ID )
			{
				ptNoteList = new MBeatBoxNoteList();
			}
			else if( noteListId == MAudioTriggerList::TYPE_ID )
			{
				ptNoteList = new MAudioTriggerList();
			}
			else
			{
				MMessageBox::showError( "INVALID_NOTELIST_ID", MUtil::toString( noteListId ) );
				return;
			}
			ptNoteList->setMaxLength( stepLength );
			ptNoteList->load( nl );
			ivPtNoteList = ptNoteList;
			if( ivPtAutomationEventList )
				ivPtAutomationEventList->setLength( stepLength );
		}
		else if( strcmp( nodeName, "elc" ) == 0 )
		{
			ivPtAutomationEventList->setLength( stepLength );
			ivPtAutomationEventList->load( nl );
		}
		else if( strcmp( nodeName, "mobject" ) == 0 )
		{
			String classType = nl->getAttribute( MObject::ATTR_CLASS );
			if( classType == "MProperties" )
				ivPtProperties->MStr2ObjMap::load( nl );
		}
	}	

	ASSERT( ivPtNoteList );
	ASSERT( ivPtAutomationEventList );
	ASSERT( ivPtNoteList->getMaxLength() == ivPtAutomationEventList->getLength() );
}

//-----------------------------------------------------------------------------
// + getEventListCollection
//-----------------------------------------------------------------------------
IEventListCollection* MBar::getEventListCollection()
{
	return ivPtAutomationEventList;
}

//-----------------------------------------------------------------------------
// + setEventListCollection
//-----------------------------------------------------------------------------
void MBar::setEventListCollection( IEventListCollection* ptCollection )
{
	if( ivPtAutomationEventList )
		ivPtAutomationEventList->release();
	ivPtAutomationEventList = ptCollection;
}

//-----------------------------------------------------------------------------
// # setUp
//-----------------------------------------------------------------------------
void MBar::setUp()
{
	setEventListCollection(
		new MEventListCollection( ivPtTracker, ivNotesPerBar * ivBarLength ) );
}

//-----------------------------------------------------------------------------
// + cleanUp
// Deletes all notes and the reserved mem to hold
//---------------------------------------------------------------------------------
void MBar::cleanUp()
{
	clear();
}

//-----------------------------------------------------------------------------
// + getTypeId (Inheritanced from ICopyCutPaste)
//	returns the type id of a MBar needed for the paste context
//-----------------------------------------------------------------------------
int MBar::getTypeId()
{
	return MBar::TYPE_ID;
}

//-----------------------------------------------------------------------------
// + getUserTypeName
//-----------------------------------------------------------------------------
char* MBar::getUserTypeName()
{
	return MBar::TYPE_USER_NAME;
}

//-----------------------------------------------------------------------------
// + destroy (Inheritanced from ICopyCutPaste)
//	destroys this item
//-----------------------------------------------------------------------------
void MBar::release()
{
	clear();
	delete this;
}

//-----------------------------------------------------------------------------
// + getCopy (Inheritanced from ICopyCutPaste)
//	returns a identical copy of this item
//-----------------------------------------------------------------------------
ICopyCutPaste* MBar::getCopy()
{
	return new MBar( this );
}

void MBar::addBarListener( IBarListener* ptListener )
{
	ivListeners.addListener( ptListener );
}

void MBar::removeBarListener( IBarListener* ptListener )
{
	ivListeners.removeListener( ptListener );
}

void MBar::fireColChanged()
{
	unsigned int count = ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IBarListener*)ivListeners.getListener(i))->barColChanged();
}

void MBar::fireRowChanged()
{
	unsigned int count = ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IBarListener*)ivListeners.getListener(i))->barRowChanged();
}

void MBar::fireLengthChanged()
{
	unsigned int count = ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IBarListener*)ivListeners.getListener(i))->barLengthChanged();
}

void MBar::fireResolutionChanged()
{
	unsigned int count = ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IBarListener*)ivListeners.getListener(i))->barResolutionChanged();
}

void MBar::fireBarReleasing()
{
	while( ivListeners.getListenerCount() > 0 )
		((IBarListener*)ivListeners.getListener(0))->barReleasing();
}

void MBar::setTracker( IAutomationTracker* ptTracker )
{
	ivPtAutomationEventList->setTracker( ptTracker );
}

IAutomationTracker* MBar::getTracker()
{
	return ivPtAutomationEventList->getTracker();
}

MProperties* MBar::getProperties()
{
	return ivPtProperties;
}