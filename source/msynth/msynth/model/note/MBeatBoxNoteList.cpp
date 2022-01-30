/*
	MBeatBoxNoteList (C)2001 MRoc hifiShock

		this notelist imeplements a IInstrumentListener to be informed
		if the BeatBox channel count changed.

*/

#include "MBeatBoxNoteList.h"

int	MBeatBoxNoteList::TYPE_ID = 3;
char* MBeatBoxNoteList::TYPE_USER_NAME = "beatbox note";

INIT_RTTI( MBeatBoxNoteList, "MBeatBoxNoteList" );

//-----------------------------------------------------------------------------
// + CONSTRUCTION
//-----------------------------------------------------------------------------
MBeatBoxNoteList::MBeatBoxNoteList()
	: MDefaultMessageNoteList()
{
}

//-----------------------------------------------------------------------------
// + COPY CONSTRUCTION
//-----------------------------------------------------------------------------
MBeatBoxNoteList::MBeatBoxNoteList( MBeatBoxNoteList* ptNoteList )
	: MDefaultMessageNoteList( ptNoteList )
{
};

//-----------------------------------------------------------------------------
// + DESTRUCTION
//-----------------------------------------------------------------------------
MBeatBoxNoteList::~MBeatBoxNoteList()
{
};

//-----------------------------------------------------------------------------
// + getTypeId
//	returns the type id of a MBeatBoxNoteList
//-----------------------------------------------------------------------------
int	MBeatBoxNoteList::getTypeId()
{
	return MBeatBoxNoteList::TYPE_ID;
};

//-----------------------------------------------------------------------------
// + getTypeId
//	returns the type id of a MBeatBoxNoteList
//-----------------------------------------------------------------------------
char* MBeatBoxNoteList::getUserTypeName()
{
	return MBeatBoxNoteList::TYPE_USER_NAME;
};

//-----------------------------------------------------------------------------
// + static createInstance (Inheritanced from IInstrumentListener)
//-----------------------------------------------------------------------------
MDefaultNoteList* MBeatBoxNoteList::createInstance()
{
	return new MBeatBoxNoteList();
};

//-----------------------------------------------------------------------------
// + getCopy (Inheritanced from MInstrument)
//	returns a identical copy of this noteList
//-----------------------------------------------------------------------------
ICopyCutPaste* MBeatBoxNoteList::getCopy()
{
	return new MBeatBoxNoteList( this );
};

void MBeatBoxNoteList::updateFromInstrument( MInstrument* ptInstrument )
{
	MBeatBox* ptDB = (MBeatBox*) ptInstrument;
	setHighestId( ptDB->getWavePlayerCount() );
}

IInstrumentListener* MBeatBoxNoteList::getInstrumentListener()
{
	return (IInstrumentListener*) this;
};

/**
 * inheritances from IInstrumentListener
 */
void MBeatBoxNoteList::stateChanged( MInstrumentEvent anEvent )
{
	int messageType = anEvent.getMessageType();

	if( messageType == MBeatBox::MESSAGE_CHANNEL_ADDED )
	{
	}
	else if( messageType == MBeatBox::MESSAGE_CHANNEL_REMOVED )
	{
		cutId( anEvent.getValue() );
	}
	else if( messageType == MBeatBox::MESSAGE_UPDATE_FROM_MODEL )
	{
		setHighestId( anEvent.getValue() );
	}
}

/**
 * inheritances from IInstrumentListener
 */
String MBeatBoxNoteList::getInstrumentListenerName()
{
	return "MBeatBoxNoteList";
}

MDefaultNoteList* MBeatBoxNoteList::createNewInstance()
{
	return createInstance();
};

void MBeatBoxNoteList::applyDynamics( unsigned int index, FP volume )
{
	MDefaultMessageNote *ptNote = (MDefaultMessageNote*) getFirstItem();
	while( ptNote != 0 )
	{
		if( ptNote->getStartsAt() == index )
		{
			unsigned int msgCount = ptNote->getMessageCount();
			for( unsigned int i=0;i<msgCount;i++ )
				((MBeatBoxMessage*)ptNote->getMessageAt( i ))->setVolume( volume );
		}
		ptNote = (MDefaultMessageNote*) ptNote->getNext();
	}
}