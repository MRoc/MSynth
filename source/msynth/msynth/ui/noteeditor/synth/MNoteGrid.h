/*

	MNoteGrid (C)2001 MRoc hifiShock

	A ScrollPane displaying the MNoteControls,
	doubel buffered, notes can be created, dragged and edited.

*/

#ifndef __MNoteGrid
#define __MNoteGrid

#include <gui/MGuiUtils.h>
#include "MNoteControl.h"
#include "../../dcgrid/MDCGrid.h"

#include "../../../../resource.h"
#include "../../clipboard/MClipBoard.h"
#include "../../../../msynth/model/Synth.h"
#include "../../../../msynth/model/note/MDefaultSynthNoteList.h"

class MNoteGrid : public MDCGrid
{
protected:

	bool			ivNoteEditMode;

	MDefaultSynthNoteList* ivPtNoteList;

public:

	MNoteGrid( MSize* ptPartSize, MDefaultSynthNoteList *ptNoteList );
	virtual ~MNoteGrid();

	virtual void	paint( IGraphics* ptGraphics, const MRect &rect );

	void			updateFromModel();
	void			updateFromPartSize();
	
	void			deleteSelected();

	void			setNoteList( MDefaultSynthNoteList *ptNoteList );
	MDefaultSynthNoteList *getNoteList();

	void			copy();
	void			paste( int position );

	void			ScrollToVisible();

protected:

	virtual void	moveControl( MDCControl* _ptControl, int rowDiv, int colDiv );

	virtual int		getMaxRows();
	virtual int		getMaxCols();

	virtual int		getRow( MPoint point );

	virtual int		getRow( MDCControl* ptControl );
	virtual int		getCol( MDCControl* ptControl );
	virtual int		getColSpan( MDCControl* ptControl );

	virtual void	onMouseDown( MMouseEvent* anEvent );
	virtual void	onMouseUp( MMouseEvent* anEvent );
	virtual void	onMouseMove( MMouseEvent* anEvent );

	virtual	bool	onSetCursor();

	MDCControl*		CreateNoteControl( MDefaultSynthNote *ptNote );
	MDCControl*		getControlAt( MPoint p );
	void			deleteControls();
	virtual void	deleteControl( MDCControl* _ptControl );

	virtual MDCControl*	createControl( MPoint point );
	void			startControlEditing( MDCControl *control );
	void			stopControlMoving( MPoint point );
	bool			updateNoteEditing( MPoint point );
};

#endif
