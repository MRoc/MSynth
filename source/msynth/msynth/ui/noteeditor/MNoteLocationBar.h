/*

  MNoteLocationBar (C)2001 MRoc hifiShock

  This class displays a locationbar which is scrollable,
  to display the location of a note

*/

#ifndef __MNoteLocationBar
#define __MNoteLocationBar

#include <framework/MUtil.h>
#include <gui/MyInteractiveWnd.h>

#include "../../../resource.h"

class MNoteLocationBar :
	public MyInteractiveWnd
{
protected:

	int				ivBeatCount;
	int				ivNotesPerBeat;
	MSize*			ivPtPartSize;

	int				ivLocatorPos;
	MPoint			ivLocatorPoly[3];

public:

	MNoteLocationBar( int beatCount, int notesPerBeat, MSize* ptPartSize );

	virtual ~MNoteLocationBar();

	virtual void paint( IGraphics* ptGraphics, const MRect &rect );

	virtual void setBeatCount( int beatCount );
	virtual int getBeatCount();

	virtual void setNotesPerBeat( int resolution );
	virtual int getNotesPerBeat();

	virtual void setNoteWidth( MSize* ptPartSize );
	virtual MSize* getNoteWidth();

	virtual void setLocatorPos( int locatorPos );
	virtual int getLocatorPos();

	virtual void onMouseDoubleClick( MMouseEvent* anEvent );
};

#endif
