/*

	MClipBoard (C)2001 MRoc hifiShock

*/

#include "MClipBoard.h"

MClipBoard* MClipBoard::ivInstance = 0;

MClipBoard::MClipBoard() :
	ivPtContext( 0 ),
	ivContent( 0 )
{
}

MClipBoard::~MClipBoard()
{
	if( ivContent )
		delete ivContent;
}

MClipBoardContext* MClipBoard::getContent()
{
	return ivContent;
}

void MClipBoard::setContent( ICopyCutPaste* content )
{
	if( ivContent )
		delete ivContent;

	ivContent = new MClipBoardContext( content );
}

void MClipBoard::setContent( MClipBoardContext* content )
{
	if( ivContent )
		delete ivContent;

	ivContent = content;
}

void MClipBoard::deleteContent()
{
	if( ivContent )
	{
		delete ivContent;
		ivContent = 0;
	}
}

bool MClipBoard::hasContent()
{
	return ivContent != 0;
}

MClipBoard* MClipBoard::getInstance()
{
	if( ivInstance == 0 )
		ivInstance = new MClipBoard();

	return ivInstance;
}

void MClipBoard::release()
{
	if( ivInstance )
	{
		delete ivInstance;
		ivInstance = 0;
	}
}

void MClipBoard::setContext( IClipBoardContext* pTarget )
{
	ivPtContext = pTarget;
}

void MClipBoard::unsetContext( IClipBoardContext* pTarget )
{
	if( ivPtContext == pTarget )
		ivPtContext = 0;
}

IClipBoardContext* MClipBoard::getContext()
{
	return ivPtContext;
}

