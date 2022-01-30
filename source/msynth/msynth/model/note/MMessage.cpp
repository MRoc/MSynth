/*

  MMessage (C)2001 MRoc hifiShock

  Prototype of a message

*/

#include "MMessage.h"

#define ID_UNDEF -1


//-----------------------------------------------------------------------------
// + CONSTRUCTION
//-----------------------------------------------------------------------------
MMessage::MMessage()
{
	setId( ID_UNDEF );
};

//-----------------------------------------------------------------------------
// + CONSTRUCTION 2
//-----------------------------------------------------------------------------
MMessage::MMessage( int id )
{
	setId( id );
};

//-----------------------------------------------------------------------------
// + COPY CONSTRUCTION
//-----------------------------------------------------------------------------
MMessage::MMessage( MMessage* message )
{
	setId( message->getId() );
};

//-----------------------------------------------------------------------------
// + DESTRUCTION
//-----------------------------------------------------------------------------
MMessage::~MMessage()
{
};

//-----------------------------------------------------------------------------
// + SET ID
//-----------------------------------------------------------------------------
void MMessage::setId( int id )
{
	ivId = id;
};

//-----------------------------------------------------------------------------
// + GET ID
//-----------------------------------------------------------------------------
int MMessage::getId()
{
	return ivId;
};

//-----------------------------------------------------------------------------
// + TO XML
//-----------------------------------------------------------------------------
MTreeNode* MMessage::save()
{
	MTreeNode* ptBack = new MTreeNode( "msg" );
	ptBack->setAttribute( "id", MInt::toString( getId() ) );
	return ptBack;
};

//-----------------------------------------------------------------------------
// + INIT FROM XML
//-----------------------------------------------------------------------------
void MMessage::load( MTreeNode* parentNode )
{
	setId(
		MInt::parse(
			parentNode->getAttribute( "id" ),
			-1,
			MInt::MAX_INT,
			ID_UNDEF ) );
};

MMessage* MMessage::getCopy()
{
	return new MMessage( this );
};