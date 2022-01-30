/**
 * (C)2001-2003 MRoc
 */

#include "MDefaultMessageNote.h"

MDefaultMessageNote::MDefaultMessageNote() :
	MDefaultNote()
{
	setDuration( 1 );
}

MDefaultMessageNote::MDefaultMessageNote( MDefaultMessageNote* ptNote ) :
	MDefaultNote( ptNote )
{
	unsigned int count = (unsigned int) ptNote->getMessageCount();
	for( unsigned int i=0;i<count;i++ )
		addMessage( ptNote->getMessageAt( i )->getCopy() );
}

void MDefaultMessageNote::setDuration( unsigned int duration )
{
	// message note has allways length one
	ivDuration = 1;
}

MDefaultMessageNote::~MDefaultMessageNote()
{
	deleteContent();
}

void MDefaultMessageNote::addMessage( MMessage* ptMessage )
{
	ivMessages.push_back( ptMessage );
}

unsigned int MDefaultMessageNote::getMessageCount()
{
	return (unsigned int) ivMessages.size();
}

MMessage* MDefaultMessageNote::getMessageAt( unsigned int index )
{
	ASSERT( index < (unsigned int)ivMessages.size() );
	return ivMessages[ index ];
}

MMessage* MDefaultMessageNote::getMessageById( int id )
{
	MMessage* ptBack = 0;

	unsigned int count = ivMessages.size();
	for( unsigned int i=0;i<count;i++ )
		if( getMessageAt( i )->getId() == id )
		{
			ptBack = getMessageAt( i );
			break;
		}

	return ptBack;
}

int MDefaultMessageNote::getIndexByMessage( MMessage *ptMessage )
{
	int back = -1;

	unsigned int count = ivMessages.size();
	for( unsigned int i=0;i<count;i++ )
		if( ptMessage = ivMessages[ i ] )
		{
			back = i;
			break;
		}

	return back;
}

int MDefaultMessageNote::getIndexById( int id )
{
	int back = -1;

	unsigned int count = ivMessages.size();
	for( unsigned int i=0;i<count;i++ )
		if( ivMessages[ i ]->getId() == id )
		{
			back = i;
			break;
		}

	return back;
}

bool MDefaultMessageNote::hasMessage( MMessage* ptMessage )
{
	return getIndexByMessage( ptMessage ) != -1;
}

void MDefaultMessageNote::removeMessageAt( unsigned int index )
{
	ivMessages.erase( ivMessages.begin() + index );
}

void MDefaultMessageNote::removeMessage( MMessage *ptMessage )
{
	int index = getIndexByMessage( ptMessage );

	if( index != -1 )
		removeMessageAt( (unsigned int)(index) );
}

void MDefaultMessageNote::removeMessageForId( int id )
{
	int index = getIndexById( id );

	if( index != -1 )
		removeMessageAt( unsigned int(index) );
}

void MDefaultMessageNote::deleteMessageAt( unsigned int index )
{
	MMessage* ptMessage = getMessageAt( index );
	removeMessageAt( index );
	delete ptMessage;
}

void MDefaultMessageNote::deleteMessageForId( int id )
{
	int index = getIndexById( id );
	if( index != -1 )
		deleteMessageAt( (unsigned int)(index) );
}

void MDefaultMessageNote::deleteContent()
{
	unsigned int count = getMessageCount();
	for( unsigned int i=0;i<count;i++ )
		delete ivMessages[ i ];
	ivMessages.clear();
}

bool MDefaultMessageNote::isOverlapping( IAbstractNote *note )
{
	return note->getStartsAt() == ivIndex;
}

bool MDefaultMessageNote::wouldOverlapp( int index, unsigned int noteLength )
{
	ASSERT( noteLength == 1 );
	return getIndex() == index;
}

MTreeNode* MDefaultMessageNote::save()
{
	MTreeNode* ptBack = new MTreeNode( "dmn" );
	ptBack->setAttribute( "startsAt", MInt::toString( (int) getIndex() ) );

	unsigned int count = getMessageCount();
	for( unsigned int i=0;i<count;i++ )
		ptBack->addChild( getMessageAt( i )->save() );

	return ptBack;
}

void MDefaultMessageNote::load( MTreeNode* parentNode )
{
	setIndex(
		MInt::parse(
			parentNode->getAttribute( "startsAt" ),
			0,
			MInt::MAX_INT,
			0 ) );

	setDuration( 1 );

	MTreeNodeIterator iter = parentNode->getIterator();
	while( iter.hasNext() )
	{
		MTreeNode* node = iter.nextTreeNode();

		String nodeName = node->getName();
		if( strcmp(nodeName, "msg") == 0 )
		{
			MMessage *ptMessage = new MMessage();
			ptMessage->load( node );
			addMessage( ptMessage );
		}
		else if( strcmp(nodeName, "bbmsg") == 0 )
		{
			MBeatBoxMessage *ptMessage = new MBeatBoxMessage();
			ptMessage->load( node );
			addMessage( ptMessage );
		}
		else
		{
			TRACE( "<Unknown message-type found: %s>\n", nodeName );
		}
	}
}


MDefaultNote* MDefaultMessageNote::getCopy()
{
	return new MDefaultMessageNote( this );
}

MDefaultNote* MDefaultMessageNote::createNewInstance()
{
	return new MDefaultMessageNote();
}