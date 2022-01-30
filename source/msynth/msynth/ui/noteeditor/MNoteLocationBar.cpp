/**
 * (C)2001-2003 MRoc hifiShock
 */

#include "MNoteLocationBar.h"

#define DEFAULT_BEAT_COUNT 4
#define DEFAULT_NOTES_PER_BEAT 4
#define DEFAULT_NOTE_WIDTH 20

MNoteLocationBar::MNoteLocationBar( int beatCount, int notesPerBeat, MSize* ptPartSize ) :
	MyInteractiveWnd(),
	ivPtPartSize( 0 ),
	ivLocatorPos( 0 )
{
	ivBkColor = MColorMap::BK_COLOR3;

	setBeatCount( beatCount );
	setNotesPerBeat( notesPerBeat );
	setNoteWidth( ptPartSize );
}

MNoteLocationBar::~MNoteLocationBar()
{
}

void MNoteLocationBar::paint( IGraphics* ptGraphics, const MRect &rect )
{
	MPoint target = getScrollPos();

	ptGraphics->fillRect( rect, ivBkColor );
	ptGraphics->drawLine(
		0,
		rect.getHeight()-1,
		rect.getWidth(),
		rect.getHeight()-1,
		MColorMap::FG_COLOR3 );

	ptGraphics->setOffset( ptGraphics->getOffset() - target );

	// Draw vertical lines
	int x = 0;
	int y1a = 0;
	int y1b = rect.getHeight() / 2;
	int y2 = rect.getHeight();
	int dX = ivPtPartSize->getWidth();
	int count = ivNotesPerBeat * ivBeatCount;
	for( int i=0;i<count;i++)
	{
		int y = y1b;
		if( i % ivNotesPerBeat == 0 )
		{
			ptGraphics->drawText(
				MInt::toString( i/ivNotesPerBeat + 1 ),
				MRect( x+2, 2, 20, 12 ),
				MColorMap::FG_COLOR3 );
			y = y1a;
		}

		ptGraphics->drawLine( x, y, x, y2, MColorMap::FG_COLOR3 );
		x += dX;
	}

	int fourthWidth = dX / 4;
	ivLocatorPoly[0].setX( - fourthWidth );
	ivLocatorPoly[0].setY( y1b );
	ivLocatorPoly[1].setX( fourthWidth );
	ivLocatorPoly[1].setY( y1b );
	ivLocatorPoly[2].setX( 0 );
	ivLocatorPoly[2].setY( y2 );

	int difLoc = dX * ivLocatorPos - getScrollPos().getX();
	ivLocatorPoly[0].setX( ivLocatorPoly[0].getX() + difLoc );
	ivLocatorPoly[1].setX( ivLocatorPoly[1].getX() + difLoc );
	ivLocatorPoly[2].setX( ivLocatorPoly[2].getX() + difLoc );
	ptGraphics->poly( ivLocatorPoly, 3, MColorMap::FG_COLOR2, MColorMap::FG_COLOR3 );

	ptGraphics->setOffset( ptGraphics->getOffset() + target );
}

void MNoteLocationBar::setBeatCount( int beatCount )
{
	ivBeatCount = beatCount;
}

int	MNoteLocationBar::getBeatCount()
{
	return ivBeatCount;
}

void MNoteLocationBar::setNotesPerBeat( int notesPerBeat )
{
	ivNotesPerBeat = notesPerBeat;
}

int MNoteLocationBar::getNotesPerBeat()
{
	return ivNotesPerBeat;
}

void MNoteLocationBar::setNoteWidth( MSize* partSize )
{
	ivPtPartSize = partSize;
}

MSize* MNoteLocationBar::getNoteWidth()
{
	return ivPtPartSize;
}

void MNoteLocationBar::setLocatorPos( int locatorPos )
{
	ivLocatorPos = locatorPos;
}

int MNoteLocationBar::getLocatorPos()
{
	return ivLocatorPos;
}

void MNoteLocationBar::onMouseDoubleClick( MMouseEvent* anEvent )
{
	ivLocatorPos = anEvent->getX() / ivPtPartSize->getWidth();
	repaint();
}
