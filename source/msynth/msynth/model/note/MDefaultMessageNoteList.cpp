/*
	(C)2001 MRoc hifiShock
*/

#include "MDefaultMessageNoteList.h"
#include <gui/MColorMap.h>

int MDefaultMessageNoteList::TYPE_ID = 2;
char* MDefaultMessageNoteList::TYPE_USER_NAME = "Default message note list";

//-----------------------------------------------------------------------------
// + CONSTRUCTION 1
//-----------------------------------------------------------------------------
MDefaultMessageNoteList::MDefaultMessageNoteList()
	: MDefaultNoteList()
{
};

//-----------------------------------------------------------------------------
// + COPY CONSTRUCTION
//-----------------------------------------------------------------------------
MDefaultMessageNoteList::MDefaultMessageNoteList( MDefaultMessageNoteList* ptNoteList )
	: MDefaultNoteList( ptNoteList )
{
};

//-----------------------------------------------------------------------------
// + DESTRUCTION
//-----------------------------------------------------------------------------
MDefaultMessageNoteList::~MDefaultMessageNoteList()
{
	clear(); 
};

void MDefaultMessageNoteList::applyDynamics( unsigned int index, FP value )
{
}

//-----------------------------------------------------------------------------
// + GET TYPE ID
// Returns the type id of this NoteList, needed for the dragNDrop-Operations
// between different channels of the arrangeview
//-----------------------------------------------------------------------------
int MDefaultMessageNoteList::getTypeId()
{
	return MDefaultMessageNoteList::TYPE_ID;
};

char* MDefaultMessageNoteList::getUserTypeName()
{
	return MDefaultMessageNoteList::TYPE_USER_NAME;
};

//-----------------------------------------------------------------------------
// + POST MESSAGE
// Posts a message in a note, if no note exists, it creates one
//-----------------------------------------------------------------------------
MDefaultMessageNote* MDefaultMessageNoteList::postMessage( MMessage *message, int index )
{
	ASSERT( message != 0 && index >= 0 );

	MDefaultMessageNote *ptNote = (MDefaultMessageNote*) getItemAt( index );
	if( ptNote == 0 )
	{
		ptNote = (MDefaultMessageNote*) createDefaultNote();
		ptNote->setIndex( index );
		ptNote->setDuration( 1 );
		//MList::insertAt( ptNote/*, index*/ );
		MList::insertItem( ptNote );
	}
	ptNote->addMessage( message );

	return ptNote;
};

//-----------------------------------------------------------------------------
// + GET MESSAGE FOR ID
// Returns a message at the given index with the given id (0 if not found)
//-----------------------------------------------------------------------------
MMessage* MDefaultMessageNoteList::getMessageForId( int index, int id )
{
	MMessage *back = 0;
	MDefaultMessageNote *ptNote = (MDefaultMessageNote*) getItemAt( index );
	if( ptNote != 0 )
	{
		back = ptNote->getMessageById( id );
	}

	return back;
};

//-----------------------------------------------------------------------------
// + DELETE MESSAGE FOR ID
// Deletes a message at the given index with the given id
// If the containing note does not contain any message then, its deleted
//-----------------------------------------------------------------------------
void MDefaultMessageNoteList::deleteMessageForId( int index, int id )
{
	MDefaultMessageNote *ptNote = (MDefaultMessageNote*) getItemAt( index );
	if( ptNote != 0 )
	{
		ptNote->deleteMessageForId( id );
	}
	if( ptNote->getMessageCount() == 0 )
	{
		deleteItem( ptNote );
	}
};

//-----------------------------------------------------------------------------
// + cutId
//	deletes all messages for an id. increments all ids over the given id.
//-----------------------------------------------------------------------------
void MDefaultMessageNoteList::cutId( int id )
{
	TRACE( "<MDefaultMessageNoteList::cutId %i>\n", id );
	MDefaultMessageNote *ptNote = (MDefaultMessageNote*) getFirstItem();
	while( ptNote != 0 )
	{
		ptNote->deleteMessageForId( id );
		for( unsigned int i=0;i<ptNote->getMessageCount();i++ )
		{
			int noteId = ptNote->getMessageAt( i )->getId();
			if( noteId > id )
			{
				ptNote->getMessageAt( i )->setId( noteId - 1 );
			}
		}
		if( ! ptNote->getMessageCount() )
		{
			MDefaultMessageNote* note = ptNote;
			ptNote = (MDefaultMessageNote*) ptNote->getNext();
			deleteItem( note );
		}
		else
		{
			ptNote = (MDefaultMessageNote*) ptNote->getNext();
		}
	}
};

//-----------------------------------------------------------------------------
// + setHighestId
//	Deletes all messages above the given id
//-----------------------------------------------------------------------------
void MDefaultMessageNoteList::setHighestId( int id )
{
	MDefaultMessageNote *ptNote = (MDefaultMessageNote*) getFirstItem();
	while( ptNote != 0 )
	{
		for( unsigned int i=0;i<ptNote->getMessageCount();i++ )
		{
			int noteId = ptNote->getMessageAt( i )->getId();
			if( noteId >= id )
			{
				ptNote->deleteMessageForId( noteId );
				i--;
			}
		}
		if( ! ptNote->getMessageCount() )
		{
			MDefaultMessageNote* note = ptNote;
			ptNote = (MDefaultMessageNote*) ptNote->getNext();
			deleteItem( note );
		}
		else
		{
			ptNote = (MDefaultMessageNote*) ptNote->getNext();
		}
	}
};

//-----------------------------------------------------------------------------
// + REMOVE MESSAGE FOR ID
// Removes a message at the given index with the given id
// If the containing note does not contain any message then, its deleted
//-----------------------------------------------------------------------------
void MDefaultMessageNoteList::removeMessageForId( int index, int id )
{
	MDefaultMessageNote *ptNote = (MDefaultMessageNote*) getItemAt( index );
	if( ptNote != 0 )
	{
		ptNote->removeMessageForId( id );
	}
	if( ptNote->getMessageCount() == 0 )
	{
		deleteItem( ptNote );
	}
};

//-----------------------------------------------------------------------------
// + CAN INSERT AT
//	Returns true if the given message can be insert at the specified index
//-----------------------------------------------------------------------------
bool MDefaultMessageNoteList::canPostMessageAt( unsigned int index, int id )
{
	bool back = true;

	MDefaultMessageNote *ptNote = (MDefaultMessageNote*) getItemAt( index );

	if( ptNote != 0 )
		if( ptNote->getMessageById( id ) )
			back = false;

	if( index >= getMaxLength() )
		back = false;

	return back;
};

//-----------------------------------------------------------------------------
// + CREATE DEFAULT MESSAGE
//-----------------------------------------------------------------------------
MMessage* MDefaultMessageNoteList::createDefaultMessage()
{
	return new MMessage();
};

//-----------------------------------------------------------------------------
// + CREATE DEFAULT NOTE
//-----------------------------------------------------------------------------
IAbstractNote *MDefaultMessageNoteList::createDefaultNote()
{
	return new MDefaultMessageNote();
}

MTreeNode* MDefaultMessageNoteList::save()
{
	MTreeNode* ptBack = new MTreeNode( "MDefaultNoteList" );
	ptBack->setAttribute( "id", MInt::toString( getTypeId() ) );

	MDefaultMessageNote *note = (MDefaultMessageNote*) getFirstItem();
	while( note != 0 )
	{
		ptBack->addChild( note->save() );
		note = (MDefaultMessageNote*) note->getNext();
	}
	
	return ptBack;
}

void MDefaultMessageNoteList::load( MTreeNode* parentNode )
{
	MTreeNodeIterator iter = parentNode->getIterator();
	while( iter.hasNext() )
	{
		MTreeNode* node = iter.nextTreeNode();
		String nodeName = node->getName();
		if( strcmp( nodeName, "dmn" ) == 0 )
		{
			MDefaultMessageNote *ptNote = (MDefaultMessageNote*) createDefaultNote();
			ptNote->load( node );
			MList::insertItem( ptNote );
		}
		else
		{
			TRACE( "<Unknown note-type found: %s>\n", nodeName );
		}
	}
};

//-----------------------------------------------------------------------------
// + getCopy
//	returns a identical copy of this notelist
//-----------------------------------------------------------------------------
ICopyCutPaste* MDefaultMessageNoteList::getCopy()
{
	return new MDefaultMessageNoteList( this );
};

MColor MDefaultMessageNoteList::getPreferedColor()
{
	return MColorMap::create( 86, 86, 175 );
};

MDefaultNoteList* MDefaultMessageNoteList::createNewInstance()
{
	return createInstance();
};

MDefaultNoteList* MDefaultMessageNoteList::createInstance()
{
	return new MDefaultMessageNoteList();
};