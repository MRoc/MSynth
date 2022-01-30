#include "MDefaultNoteList.h"

//-----------------------------------------------------------------------------
// + CONSTRUCTION
//-----------------------------------------------------------------------------
MDefaultNoteList::MDefaultNoteList() :
	MList(),
	ivMaxLength( 16 )
{
};

//-----------------------------------------------------------------------------
// + CONSTRUCTION
//-----------------------------------------------------------------------------
MDefaultNoteList::MDefaultNoteList( unsigned int maxLength ) :
	MList(),
	ivMaxLength( maxLength )
{
};

//-----------------------------------------------------------------------------
// + COPY CONSTRUCTION
//-----------------------------------------------------------------------------
MDefaultNoteList::MDefaultNoteList( MDefaultNoteList* ptNoteList ) :
	MList(),
	ivMaxLength( ptNoteList->getMaxLength() )
{
	MDefaultNote* ptNote = (MDefaultNote*) ptNoteList->getFirstItem();
	while( ptNote != 0 )
	{
		MDefaultNote* ptNewNote = ptNote->getCopy();
		MList::insertItem( ptNewNote );
		ptNote = (MDefaultNote*) ptNote->getNext();
	}
};

//-----------------------------------------------------------------------------
// + DESTRUCTION
//-----------------------------------------------------------------------------
MDefaultNoteList::~MDefaultNoteList()
{
	clear();
};

//-----------------------------------------------------------------------------
// + GET MAXLENGTH
//-----------------------------------------------------------------------------
unsigned int MDefaultNoteList::getMaxLength()
{
	return ivMaxLength;
};

//-----------------------------------------------------------------------------
// + SET MAXLENGTH
//-----------------------------------------------------------------------------
void MDefaultNoteList::setMaxLength( unsigned int maxLength )
{
	ASSERT( maxLength % 2 == 0 );

	ivMaxLength = maxLength;
};

//-----------------------------------------------------------------------------
// + CHANGE RESOLUTION
// Changes the resolution and sets maxLength
// !!! RESOLUTION MEANS newNoteLength !!!
// !!! HANDLE WITH CARE !!!
//-----------------------------------------------------------------------------
void MDefaultNoteList::changeResolution( unsigned int newMaxLength )
{
	ASSERT( newMaxLength % 2 == 0 );

	// if any notes are set yet, they must be rescaled or deleted!
	if( getFirstItem() )
	{
		if( newMaxLength > ivMaxLength )
		{ // OK, easy expand
			unsigned int rescale = newMaxLength / ivMaxLength;
			MDefaultNote *ptNote = (MDefaultNote*) getFirstItem();
			while( ptNote )
			{
				ptNote->setIndex( ptNote->getIndex() * rescale );
				ptNote->setDuration( ptNote->getDuration() * rescale );
				ptNote = (MDefaultNote*) ptNote->getNext();
			}
		}
		else
		{ // shrink, must delete some notes
			unsigned int modDel = (ivMaxLength / newMaxLength); // eg. old 16 new 4 -> rescale = 4
			MDefaultNote *ptNote = (MDefaultNote*) getFirstItem();
			while( ptNote )
			{
				if( ptNote->getIndex() % modDel == 0 )
				{ // CHANGE startsAt and duration
					ptNote->setIndex( unsigned int(ptNote->getIndex() / modDel) );
					unsigned int newLength = ptNote->getDuration() / modDel;
					if( newLength == 0 )
					{
						newLength = 1;
					}
					ptNote->setDuration( newLength );
					ptNote = (MDefaultNote*) ptNote->getNext();
				}
				else
				{ // DELETE NOTE
					// Remember next
					MDefaultNote *ptTemp = (MDefaultNote*) ptNote->getNext();
					unlink( ptNote );
					delete ptNote;
					ptNote = ptTemp;
				}
			}
		}
	}

	ivMaxLength = newMaxLength;
};

//-----------------------------------------------------------------------------
// + CHANGE MAX LENGTH
// Changes the maxlength of the noteList.
// Deletes the notes that wont be contained anymore and adjustes too long notes
//-----------------------------------------------------------------------------
void MDefaultNoteList::changeMaxLength( unsigned int newMaxLength )
{
	if( newMaxLength < ivMaxLength )
	{
		MDefaultNote* ptNote = (MDefaultNote*) getFirstItem();

		int lastValidPosition = newMaxLength - 1;
		while( ptNote )
		{
			if( ptNote->getIndex() > lastValidPosition )
			{
				MDefaultNote* temp = ptNote;
				ptNote = (MDefaultNote*) ptNote->getNext();
				deleteItem( temp );
			}
			else if( ptNote->getStopsAt() > lastValidPosition )
			{
				ptNote->setStopsAt( lastValidPosition );
				ptNote = (MDefaultNote*) ptNote->getNext();
			}
			else
			{
				ptNote = (MDefaultNote*) ptNote->getNext();
			}
		}
	}

	setMaxLength( newMaxLength );
};

//-----------------------------------------------------------------------------
// + IS FREE
// Returns true if no note starts at the specified index
//-----------------------------------------------------------------------------
bool MDefaultNoteList::isFree( unsigned int index )
{
	bool back = true;

	if( index >= ivMaxLength )
	{
		back = false;
	}
	else
	{
		MDefaultNote *note = (MDefaultNote*) getFirstItem();

		while( note != 0 && note->getIndex() <= (int)index )
		{
			if( note->getIndex() == index )
			{
				back = false;
				break;
			}
			note = (MDefaultNote*) note->getNext();
		}
	}
	
	return back;
};

//-----------------------------------------------------------------------------
// + GET ITEM AT
// Returns the item which starts at the given index
//-----------------------------------------------------------------------------
MListItem *MDefaultNoteList::getItemAt( unsigned int index )
{
	MListItem *back = 0;

	MDefaultNote *ptNote = (MDefaultNote*) getFirstItem();

	while( ptNote != 0 )
	{
		if( ptNote->getIndex() > (int)index )
			break;
		else if( ptNote->getIndex() == index )
		{
			back = ptNote;
			break;
		}
		ptNote = (MDefaultNote*) ptNote->getNext();
	}

	return back;
};

//-----------------------------------------------------------------------------
// + GET ITEM STOPS AT
// Returns the which stops at the given index
//-----------------------------------------------------------------------------
MListItem* MDefaultNoteList::getItemStopsAt( unsigned int index )
{
	MListItem *back = 0;

	MDefaultNote *ptNote = (MDefaultNote*) getFirstItem();

	while( ptNote != 0 )
	{
		if( ptNote->getIndex() > (int)index )
			break;
		else if( ptNote->getStopsAt() == index )
		{
			back = ptNote;
			break;
		}
		ptNote = (MDefaultNote*) ptNote->getNext();
	}

	return back;
};

//-----------------------------------------------------------------------------
// + insertAt
//	appends another note list
//-----------------------------------------------------------------------------
void MDefaultNoteList::insertAt( MList* ptList, unsigned int index, unsigned int newLength )
{
	TRACE( "[MDefaultNoteList::insertAt index=%i length=%i]\n", index, newLength );
	setMaxLength( newLength );

	MDefaultNote* ptNote = (MDefaultNote*) ptList->getFirstItem();

	while( ptNote != 0 )
	{
		MDefaultNote* temp = (MDefaultNote*) ptNote->getNext();
		ptList->unlink( ptNote );
		ptNote->setIndex( ptNote->getIndex() + index );
		MList::insertItem( ptNote );
		ptNote = temp;
	}
};

//-----------------------------------------------------------------------------
// + cutAt
//-----------------------------------------------------------------------------
MDefaultNoteList* MDefaultNoteList::cutAt( unsigned int index )
{
	TRACE( "<MDefaultNoteList* MDefaultNoteList::cutAt( %i )>\n", index );

	MDefaultNoteList* back = createNewInstance();
	back->setMaxLength( getMaxLength() - index );

	MDefaultNote* ptNote = (MDefaultNote*) getFirstItem();
	while( ptNote != 0 )
	{
		if( ptNote->getIndex() >= (int)index )
		{
			MDefaultNote* ptNext = (MDefaultNote*) ptNote->getNext();
			unlink( ptNote );
			ptNote->setIndex( ptNote->getIndex() - index );
			back->MList::insertItem( ptNote );
			ptNote = ptNext;
		}
		else
			ptNote = (MDefaultNote*) ptNote->getNext();
	}

	setMaxLength( index );

	return back;
};

//-----------------------------------------------------------------------------
// + getInstrumentListener
//	if returned, this notelist is registered as a listener to the instrument
//-----------------------------------------------------------------------------
IInstrumentListener* MDefaultNoteList::getInstrumentListener()
{
	return 0;
};

void MDefaultNoteList::release()
{
	clear();
	delete this;
};

// do not store this pointer!!!
void MDefaultNoteList::updateFromInstrument( MInstrument* ptInstrument )
{
};

bool MDefaultNoteList::hasVolumeControl()
{
	return true;
}