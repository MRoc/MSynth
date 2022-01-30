/*
  MBeatBoxMessageInspector (C)2001-2002 MRoc hifiShock
*/

#include "MBeatBoxMessageInspector.h"


int	MBeatBoxMessageInspector::VOLUME_MODE = 1;

MBeatBoxMessageInspector::MBeatBoxMessageInspector() :
	ivOffsetX( 0 ),
	ivPtBar( 0 ),
	ivPtPartSize( 0 ),
	ivEditIndex( 0 ),
	ivSize( MSize( 0, 0 ) )
{
}

MBeatBoxMessageInspector::~MBeatBoxMessageInspector()
{
}

String MBeatBoxMessageInspector::getName()
{
	return "Volume";
}

void MBeatBoxMessageInspector::setBar( MBar* ptBar )
{
	ivPtBar = ptBar;
}

MBar* MBeatBoxMessageInspector::getBar()
{
	return ivPtBar;
}

ToolTipInfo MBeatBoxMessageInspector::getToolTip( MPoint point )
{
	ToolTipInfo back;
	back.show = false;
	MBeatBoxMessage* ptMsg = getMessage( point );
	if( ptMsg )
	{
		back.show = true;

		String tp("");
		tp.Format( "Volume %f", ptMsg->getVolume() );
		back.toolTipText = tp;

		back.toolTipPoint.setX( (( point.getX() + ivOffsetX ) / ivPtPartSize->getWidth()) * ivPtPartSize->getWidth() - this->ivOffsetX );
		back.toolTipPoint.setY( -20 );
	}
	return back;
}

//------------------------------------------------------------------------------------
// # GET NOTE
// Returns the note at the given point, null if none was found
//------------------------------------------------------------------------------------
MBeatBoxMessage* MBeatBoxMessageInspector::getMessage( MPoint point )
{
	int index = ( point.getX() + ivOffsetX ) / ivPtPartSize->getWidth();
	MDefaultMessageNote* ptNote = (MDefaultMessageNote*) ivPtBar->getNoteList()->getItemAt( index );
	MBeatBoxMessage* ptMessage = 0;
	if( ptNote != 0 )
		ptMessage = (MBeatBoxMessage*) ptNote->getMessageById( ivEditIndex );

	return ptMessage;
};

void MBeatBoxMessageInspector::setSize( MSize dimension )
{
	ivSize = dimension;
}

MSize MBeatBoxMessageInspector::getSize()
{
	return ivSize;
}

void MBeatBoxMessageInspector::setPartSize( MSize* ptSize )
{
	ivPtPartSize = ptSize;
}

MSize* MBeatBoxMessageInspector::getPartSize()
{
	return ivPtPartSize;
}

void MBeatBoxMessageInspector::setOffsetX( int x )
{
	ivOffsetX = x;
}

int MBeatBoxMessageInspector::getOffsetX()
{
	return ivOffsetX;
}

void MBeatBoxMessageInspector::release()
{
	delete this;
}

void MBeatBoxMessageInspector::paint( IGraphics* ptGraphics, const MRect& rect )
{
	MDefaultNoteList* ivPtNoteList = ivPtBar->getNoteList();
	MDefaultMessageNote *ptNote = (MDefaultMessageNote*) ivPtNoteList->getFirstItem();
	while( ptNote != 0 )
	{
		MBeatBoxMessage* ptMessage = (MBeatBoxMessage*) ptNote->getMessageById( ivEditIndex );
		if( ptMessage != 0 )
		{
			MRect fillRect = getBar( ptNote->getIndex(), ptMessage->getVolume() );
			int col = 255 - ((int)(ptMessage->getVolume() * 255.0f));
			MColor barColor = MColorMap::create( col, col, 255 );
			ptGraphics->fillRect( fillRect, barColor );
			ptGraphics->drawRect( fillRect, MColorMap::FG_COLOR3 );
		}
		ptNote = (MDefaultMessageNote*) ptNote->getNext();
	}
}

//-----------------------------------------------------------------------------
// # GetBar
// returns the rect starting at index x and with the
// height of the window multiplied with the given value
//-----------------------------------------------------------------------------
MRect MBeatBoxMessageInspector::getBar( int index, FP value )
{
	ASSERT( value >= 0.0f && value <= 1.0f );
	int x = -ivOffsetX + index * ivPtPartSize->getWidth();
	return
		MRect(
			x,
			int(ivSize.getHeight() *  ( 1.0f - value)),
			ivPtPartSize->getWidth() - 1,
			ivSize.getHeight() );
}

bool MBeatBoxMessageInspector::onMouseDown( MMouseEvent* pEvent )
{
	bool back = false;
	MBeatBoxMessage *ptMessage = getMessage( pEvent->getPoint() );
	if( ptMessage != 0 )
	{
		FP volume = FP(ivSize.getHeight() - pEvent->getY()) / FP(ivSize.getHeight());
		ptMessage->setVolume( volume );
		back = true;
	}
	return back;
}

bool MBeatBoxMessageInspector::onMouseUp( MMouseEvent* pEvent )
{
	return false;
}

bool MBeatBoxMessageInspector::onMouseMoved( MMouseEvent* pEvent )
{
	bool back = false;
	MBeatBoxMessage* ptMessage = (MBeatBoxMessage*) getMessage( pEvent->getPoint() );
	if( ptMessage != 0 )
	{
		MPoint p = pEvent->getPoint();
		if( p.getY() < 0 )
			p.setY( 0 );
		else if( p.getY() > (int)ivSize.getHeight() )
			p.setY( ivSize.getHeight() );
		FP volume = FP(ivSize.getHeight() - p.getY() ) / FP(ivSize.getHeight());
		ptMessage->setVolume( volume );
		back = true;
	}
	return back;
}

bool MBeatBoxMessageInspector::getForwardEvents()
{
	return true;
}

void MBeatBoxMessageInspector::setContainingWnd( MWnd* ptContainer )
{
	ivPtContainer = ptContainer;
}

MWnd* MBeatBoxMessageInspector::getContainingWnd()
{
	return ivPtContainer;
}

void MBeatBoxMessageInspector::setEditIndex( unsigned int index )
{
	ivEditIndex = index;
}

unsigned int MBeatBoxMessageInspector::getEditIndex()
{
	return ivEditIndex;
}