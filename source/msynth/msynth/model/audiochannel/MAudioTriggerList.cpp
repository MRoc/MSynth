#include "MAudioTriggerList.h"
#include <gui/MColorMap.h>

const int MAudioTriggerList::TYPE_ID = 4;

char* MAudioTriggerList::TYPE_USER_NAME = "Audio Wave";

INIT_RTTI( MAudioTriggerList, "MAudioTriggerList" );

MAudioTriggerList::MAudioTriggerList()
{
}

MAudioTriggerList::~MAudioTriggerList()
{
}

MAudioTriggerList::MAudioTriggerList( MAudioTriggerList* ptList ) :
	MDefaultNoteList( ptList )
{
}

void MAudioTriggerList::applyDynamics( unsigned int index, FP value )
{
}

IAbstractNote* MAudioTriggerList::createDefaultNote()
{
	return new MAudioTrigger();
}

MColor MAudioTriggerList::getPreferedColor()
{
	return MColorMap::create( 255, 0, 0 );
}

MDefaultNoteList* MAudioTriggerList::createNewInstance()
{
	return new MAudioTriggerList();
}

int MAudioTriggerList::getTypeId()
{
	return MAudioTriggerList::TYPE_ID;
}

ICopyCutPaste* MAudioTriggerList::getCopy()
{
	return new MAudioTriggerList( this );
}

MTreeNode* MAudioTriggerList::save()
{
	MTreeNode* ptBack = new MTreeNode( "MDefaultNoteList" );
	ptBack->setAttribute( "id", MInt::toString( MAudioTriggerList::TYPE_ID ) );

	MAudioTrigger *note = (MAudioTrigger*) getFirstItem();
	while( note != 0 )
	{
		ptBack->addChild( note->save() );
		note = (MAudioTrigger*) note->getNext();
	}
	
	return ptBack;
}

void MAudioTriggerList::load( MTreeNode* ptNode )
{
	MTreeNodeIterator iter = ptNode->getIterator();
	while( iter.hasNext() )
	{
		MTreeNode* ptChild = iter.nextTreeNode();

		String nodeName = ptChild->getName();
		if( strcmp( nodeName, "at" ) == 0 )
		{
			MAudioTrigger *ptNote = (MAudioTrigger*) createDefaultNote();
			ptNote->load( ptChild );
			MList::insertItem( ptNote );
		}
		else
		{
			TRACE( "<Unknown note-type found: %s>\n", nodeName );
		}
	}
}

char* MAudioTriggerList::getUserTypeName()
{
	return MAudioTriggerList::TYPE_USER_NAME;
}

void MAudioTriggerList::stateChanged( MInstrumentEvent anEvent )
{
}

String MAudioTriggerList::getInstrumentListenerName()
{
	return "MAudioTriggerList";
}

MDefaultNoteList* MAudioTriggerList::createInstance()
{
	return new MAudioTriggerList();
}

bool MAudioTriggerList::canInsertAt( MAudioTrigger* ptTrigger, unsigned int col, unsigned int duration )
{
	ASSERT( ptTrigger );

	bool back = true;

	if( col < 0 || col+duration > ivMaxLength )
	{
		TRACE( "<MAudioTriggerList::canInsertAt msg=\"invalid index\"/>\n" );
		back = false;
	}
	else
	{
		MAudioTrigger* enumNote = (MAudioTrigger*) getFirstItem();

		while( enumNote != 0 )
		{
			if( ptTrigger != enumNote &&
				((MAudioTrigger*)enumNote)->wouldOverlapp( col, duration ) )
			{
				back = false;
				break;
			}
			enumNote = (MAudioTrigger*) enumNote->getNext();
		}
	}

	return back;
}

bool MAudioTriggerList::hasVolumeControl()
{
	return false;
}