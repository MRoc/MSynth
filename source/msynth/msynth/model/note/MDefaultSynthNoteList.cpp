/*
	(C)2001 MRoc hifiShock
*/

#include "MDefaultSynthNoteList.h"

#include <gui/MColorMap.h>

int MDefaultSynthNoteList::TYPE_ID = 1;
char* MDefaultSynthNoteList::TYPE_USER_NAME = "Synth note";

//-----------------------------------------------------------------------------
// + CONSTRUCTION 1
//-----------------------------------------------------------------------------
MDefaultSynthNoteList::MDefaultSynthNoteList() :
	MDefaultNoteList()
{
};

//-----------------------------------------------------------------------------
// + COPY CONSTRUCTION
//-----------------------------------------------------------------------------
MDefaultSynthNoteList::MDefaultSynthNoteList( MDefaultSynthNoteList* ptNoteList ) :
	MDefaultNoteList( ptNoteList )
{
};

//-----------------------------------------------------------------------------
// + DESTRUCTION
//-----------------------------------------------------------------------------
MDefaultSynthNoteList::~MDefaultSynthNoteList()
{
	clear(); 
};

//-----------------------------------------------------------------------------
// + GET TYPE ID
// Returns the type id of this NoteList, needed for the dragNDrop-Operations
// between different channels of the arrangeview
//-----------------------------------------------------------------------------
int MDefaultSynthNoteList::getTypeId()
{
	return MDefaultSynthNoteList::TYPE_ID;
};

//-----------------------------------------------------------------------------
// + CAN INSERT AT
// Returns true if the given note can be insert at the specified index
//-----------------------------------------------------------------------------
bool MDefaultSynthNoteList::canInsertAt( MDefaultSynthNote *note, unsigned int index, unsigned int length, unsigned int value )
{
	bool back = true;

	MDefaultSynthNote *ptNote = (MDefaultSynthNote*) note;

	if( index < 0 || index+length > ivMaxLength || value < 0 || value > unsigned int(getOctaveCount()*NOTES_PER_OCTAVE) )
	{
		back = false;
	}
	else
	{
		MDefaultSynthNote* enumNote = (MDefaultSynthNote*) getFirstItem();

		while( enumNote != 0 )
		{
			if( note != enumNote )
			{
				if( ((MDefaultSynthNote*)enumNote)->getValue() == value )
				{
					if( ((MDefaultSynthNote*)enumNote)->wouldOverlapp( index, length ) )
					{
						back = false;
					}
				}
				else if( enumNote->getIndex() == index )
				{
					back = false;
				}
			}
			enumNote = (MDefaultSynthNote*) enumNote->getNext();
		}
	}

	return back;
};

//-----------------------------------------------------------------------------
// + GET MAXIMAL POSSIBLE LENGTH
// Returns the maximal possible length the given note can have
//-----------------------------------------------------------------------------
int MDefaultSynthNoteList::getMaxPossibleLength( MDefaultSynthNote *ptNote )
{
	int back = -1; // means not set

	MDefaultSynthNote *curNote = (MDefaultSynthNote*) ptNote->getNext();
	int noteValue = ptNote->getValue();
	int noteStopsAt = ptNote->getStopsAt();

	while( curNote != 0 )
	{
		if( noteValue == curNote->getValue() )
		{
			back = curNote->getIndex() - ptNote->getIndex();
			break;
		}
		curNote = (MDefaultSynthNote*) curNote->getNext();
	}

	if( back == -1 )
	{
		back = ivMaxLength - ptNote->getIndex();
	}

	return back;
};

//-----------------------------------------------------------------------------
// + GET OCTAVE COUNT
//-----------------------------------------------------------------------------
unsigned int MDefaultSynthNoteList::getOctaveCount()
{
	return OCTAVE_COUNT;
};

//-----------------------------------------------------------------------------
// + CREATE DEFAULT NOTE
//-----------------------------------------------------------------------------
IAbstractNote *MDefaultSynthNoteList::createDefaultNote()
{
	return new MDefaultSynthNote();
};

MTreeNode* MDefaultSynthNoteList::save()
{
	MTreeNode* ptBack = new MTreeNode( "MDefaultNoteList" );
	ptBack->setAttribute( "id", MInt::toString( MDefaultSynthNoteList::TYPE_ID ) );

	MDefaultSynthNote *note = (MDefaultSynthNote*) getFirstItem();
	while( note != 0 )
	{
		ptBack->addChild( note->save() );
		note = (MDefaultSynthNote*) note->getNext();
	}
	
	return ptBack;
}

void MDefaultSynthNoteList::load( MTreeNode* parentNode )
{
	MTreeNodeIterator iter = parentNode->getIterator();
	while( iter.hasNext() )
	{
		MTreeNode* node = iter.nextTreeNode();
		String nodeName = node->getName();
		if( strcmp(nodeName, "dsn") == 0 )
		{
			MDefaultNote *ptNote = (MDefaultNote*) createDefaultNote();
			ptNote->load( node );
			MList::insertItem( ptNote );
		}
		else
		{
			TRACE( "<Unknown note-type found: %s>\n", nodeName );
		}
	}
}

//-----------------------------------------------------------------------------
// + getPreferedColor
//	with this color this notelist is displayed in the arrangeview
//-----------------------------------------------------------------------------
MColor MDefaultSynthNoteList::getPreferedColor()
{
	return MColorMap::create( 175, 170, 247 );
};

//-----------------------------------------------------------------------------
// + createNewInstance
//	returns a new instance from this, calls the static class function
//-----------------------------------------------------------------------------
MDefaultNoteList* MDefaultSynthNoteList::createNewInstance()
{
	return createInstance();
};

//-----------------------------------------------------------------------------
// + getCopy
//	returns a identical copy of this notelist
//-----------------------------------------------------------------------------
ICopyCutPaste* MDefaultSynthNoteList::getCopy()
{
	return new MDefaultSynthNoteList( this );
};

//-----------------------------------------------------------------------------
// + createInstance
//	 static class function
//-----------------------------------------------------------------------------
MDefaultNoteList* MDefaultSynthNoteList::createInstance()
{
	return new MDefaultSynthNoteList();
};

//-----------------------------------------------------------------------------
// + getAverageNoteValue
//	returns the average note value of all notes this list contains
//-----------------------------------------------------------------------------
int MDefaultSynthNoteList::getAverageNoteValue()
{
	int noteCount = 0;
	int noteValueSum = 0;

	MDefaultSynthNote* pt = (MDefaultSynthNote*) getFirstItem();
	while( pt != 0 )
	{
		noteCount++;
		noteValueSum += pt->getValue();
		pt = (MDefaultSynthNote*) pt->getNext();
	}

	int back = 0;
	if( noteCount != 0 )
		back = noteValueSum / noteCount;

	return back;
};

char* MDefaultSynthNoteList::getUserTypeName() 
{
	return MDefaultSynthNoteList::TYPE_USER_NAME;
};

void MDefaultSynthNoteList::applyDynamics( unsigned int index, FP volume )
{
	MDefaultSynthNote* pt = (MDefaultSynthNote*) getFirstItem();
	while( pt != 0 )
	{
		((MDefaultSynthNote*)pt)->setVolume( volume );
		pt = (MDefaultSynthNote*) pt->getNext();
	}
}