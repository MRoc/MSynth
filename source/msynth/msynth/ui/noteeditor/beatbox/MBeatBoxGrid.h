/*

	MBeatBoxGrid (C)2001 MRoc hifiShock

	A ScrollPane displaying the MNoteControls,
	doubel buffered, notes can be created, dragged and edited.

*/

#ifndef __MBeatBoxGrid
#define __MBeatBoxGrid

#include <framework/MTypes.h>
#include "MMessageNoteControl.h"
#include "../../dcgrid/MDCGrid.h"
#include "../../../model/MBeatBox.h"
#include "../../../model/note/MDefaultMessageNoteList.h"

typedef std::vector<MDCControl*> NoteControlList;
typedef NoteControlList::const_iterator NoteControlIter;

class MBeatBoxGrid : public MDCGrid
{
protected:

	static int					gvInstanceCount;
	static MObjectList*			gvDynamics;
	static MObjectList*			gvPatterns;

	bool						ivNoteEditMode;

	MBeatBox*					ivPtBeatBox;

	MDefaultMessageNoteList*	ivPtNoteList;

	MPoint						ivCurPoint;
	/*CMenu*						ivPtContextMenu;
	CMenu*						ivPtDynamicsMenu;
	CMenu*						ivCreatorsMenu;*/

public:

	MBeatBoxGrid(	MBeatBox* ptDrumBox,
					MSize* ptPartSize,
					MDefaultMessageNoteList *ptNoteList );

	virtual ~MBeatBoxGrid();

	virtual void				paint( IGraphics* ptGraphics, const MRect &rect );

	virtual void				updateFromModel();
	
	virtual void				copy();
	virtual void				paste( int index );
	virtual void				deleteSelected();

	void						setNoteList( MDefaultMessageNoteList *ptNoteList );
	MDefaultMessageNoteList*	getNoteList();

protected:

	virtual void				onCommand( unsigned int id );

	virtual void				moveControl( MDCControl* ptControl, int row, int col );

	virtual int					getMaxRows();
	virtual int					getMaxCols();

	virtual int					getRow( MDCControl* ptControl );
	virtual int					getCol( MDCControl* ptControl );
	virtual int					getColSpan( MDCControl* ptControl );

	MBeatBoxNoteList*			getNoteListFromSelection();

	virtual void				onMouseDown( MMouseEvent* anEvent );
	virtual void				onMouseUp( MMouseEvent* anEvent );
	virtual void				onMouseMove( MMouseEvent* anEvent );

	virtual bool				onSetCursor();

	void						createControls( MDefaultMessageNote *ptNote );
	void						createControl( MDefaultMessageNote* ptNote, MMessage* msg );

	MDCControl*					createControl( MPoint point );
	void						deleteControl( MDCControl* _ptControl );
	void						stopControlMoving( MPoint point );
};

#endif
