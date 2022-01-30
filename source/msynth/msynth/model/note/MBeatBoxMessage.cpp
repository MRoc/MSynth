/*

  MBeatBoxMessage (C)2001 MRoc hifiShock

  Prototype of a beatbox message

*/

#include "MBeatBoxMessage.h"

#define ID_UNDEF -1
#define DEF_VOLUME 0.8f

//-----------------------------------------------------------------------------
// + CONSTRUCTION
//-----------------------------------------------------------------------------
MBeatBoxMessage::MBeatBoxMessage() : MMessage()
{
	setVolume( DEF_VOLUME );
};

//-----------------------------------------------------------------------------
// + CONSTRUCTION 2
//-----------------------------------------------------------------------------
MBeatBoxMessage::MBeatBoxMessage( int id ) : MMessage( id )
{
	setVolume( DEF_VOLUME );
};

//-----------------------------------------------------------------------------
// + CONSTRUCTION 3
//-----------------------------------------------------------------------------
MBeatBoxMessage::MBeatBoxMessage( int id, FP volume ) : MMessage( id )
{
	setVolume( volume );
};

//-----------------------------------------------------------------------------
// + COPY CONSTRUCTION
//-----------------------------------------------------------------------------
MBeatBoxMessage::MBeatBoxMessage( MBeatBoxMessage *ptMessage ) : MMessage( ptMessage )
{
	setVolume( ptMessage->getVolume() );
};

//-----------------------------------------------------------------------------
// + DESTRUCTION
//-----------------------------------------------------------------------------
MBeatBoxMessage::~MBeatBoxMessage()
{
};

//-----------------------------------------------------------------------------
// + SET ID
//-----------------------------------------------------------------------------
void MBeatBoxMessage::setVolume( FP volume )
{
	ivVolume = volume;
};

//-----------------------------------------------------------------------------
// + GET ID
//-----------------------------------------------------------------------------
FP MBeatBoxMessage::getVolume()
{
	return ivVolume;
};

//-----------------------------------------------------------------------------
// + TO XML
//-----------------------------------------------------------------------------
MTreeNode* MBeatBoxMessage::save()
{
	MTreeNode* ptBack = new MTreeNode( "bbmsg" );
	ptBack->setAttribute( "id", MInt::toString( getId() ) );
	ptBack->setAttribute( "vol", MFloat::toString( getVolume() ) );
	return ptBack;
};

//-----------------------------------------------------------------------------
// + INIT FROM XML
//-----------------------------------------------------------------------------
void MBeatBoxMessage::load( MTreeNode* parentNode )
{
	setId(
		MInt::parse(
			parentNode->getAttribute( "id" ),
			-1,
			MInt::MAX_INT,
			ID_UNDEF ) );

	setVolume(
		MFloat::parse(
			parentNode->getAttribute( "vol" ),
			0.0f,
			1.0f,
			DEF_VOLUME ) );
};

//-----------------------------------------------------------------------------
// + getCopy
//	returns a identical copy of this beatBoxMessage
//-----------------------------------------------------------------------------
MMessage* MBeatBoxMessage::getCopy()
{
	return new MBeatBoxMessage( this );
};