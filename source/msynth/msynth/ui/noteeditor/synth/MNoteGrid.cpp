/**
 * (C)2001-2003 MRoc
 */

#include "MNoteGrid.h"
#include <gui/dialog/MMessageBox.h>
#include <gui/MApp.h>
#include "../../../../MSynthConstants.h"

#define NOTE_BAR_WIDTH 20 
#define NOTE_BAR_HEIGHT 12

MNoteGrid::MNoteGrid( MSize* ptPartSize, MDefaultSynthNoteList *ptNoteList )
	: MDCGrid( ptPartSize )
{
	setEditMode( MNoteGrid::DRAW_MODE );

	ivNoteEditMode = false;

	ivPtPartSize->setWidth( NOTE_BAR_WIDTH );
	ivPtPartSize->setHeight( NOTE_BAR_HEIGHT );

	setNoteList( ptNoteList );
}

MNoteGrid::~MNoteGrid()
{
	ivPtSelectionManager->resetSelection();
	deleteControls();
}

void MNoteGrid::copy()
{
	unsigned int count;
	std::vector<MDefaultNote*> selectedNotes;
	int min = ivPtNoteList->getMaxLength();
	int max = 0;
	int newStartAt;
	MDefaultNote* ptNote;

	// search all selected notes, minimal startsAt and maximal stopsAt
	count = ivControls.size();
	for( unsigned int i=0;i<count;i++ )
		if( ((MNoteControl*)ivControls[ i ])->getSelected() )
		{
			ptNote = ((MNoteControl*)ivControls[ i ])->getNote();
			selectedNotes.push_back( ptNote );
			if( ptNote->getIndex() < min )
				min = ptNote->getIndex();
			if( ptNote->getStopsAt() > max )
				max = ptNote->getStopsAt();
		}

	if( (count=selectedNotes.size()) )
	{
		MDefaultSynthNoteList* ptNoteList = (MDefaultSynthNoteList*) ivPtNoteList->createNewInstance();
		if( max%2 == 0 )
			max += 1;
		ptNoteList->setMaxLength( max + 1 );
		for( unsigned int i=0;i<count;i++ )
		{
			ptNote = (MDefaultNote*) selectedNotes[ i ];
			MDefaultNote* ptNewNote = ptNote->getCopy();
			newStartAt = ptNewNote->getIndex() - min;
			ptNewNote->setIndex( newStartAt );
			ptNoteList->MList::insertItem( ptNewNote );
			//ptNoteList->insertItem( ptNewNote, ptNewNote->getIndex() );
		}

		MClipBoard::getInstance()->setContent( ptNoteList );
	}
}

void MNoteGrid::paste( int position )
{
	MClipBoardContext* content = MClipBoard::getInstance()->getContent();

	if( content )
	{
		if( content->getUserObject()->getTypeId() == ivPtNoteList->getTypeId() )
		{
			TRACE( "Clipboard type: " + String( content->getUserObject()->getUserTypeName() ) ); 
			int count = 0;
			MDefaultSynthNoteList* ptNoteList = (MDefaultSynthNoteList*) content->getUserObject();
			MDefaultSynthNote* ptNote = (MDefaultSynthNote*) ptNoteList->getFirstItem();
			while( ptNote != 0 )
			{
				if( ivPtNoteList->canInsertAt(
						ptNote,
						ptNote->getIndex() + position,
						ptNote->getDuration(),
						ptNote->getValue() ) )
				{
					MDefaultSynthNote* ptNewNote = (MDefaultSynthNote*) ptNote->getCopy();
					int insertPosition = ptNewNote->getIndex() + position;
					ptNewNote->setIndex( insertPosition );
					ivPtNoteList->MList::insertItem( ptNewNote );
					//ivPtNoteList->insertItem( ptNewNote, ptNewNote->getIndex() );
					CreateNoteControl( ptNewNote );
					count++;
				}
				ptNote = (MDefaultSynthNote*) ptNote->getNext();
			}
			repaint();
			TRACE( "%i notes inserted!!!\n", count );
		}
		else
		{
			MMessageBox::showError(	"INVALID_PASTE_IN_SYNTHGRID", String( content->getUserObject()->getUserTypeName() ) );
		}

	}
	else
	{
		TRACE( " NO ClipBoardContextFound!!!>\n" );
		MMessageBox::showError(	"CLIPBOARD_EMPTY" );
	}
}

void MNoteGrid::paint( IGraphics* ptGraphics, const MRect &rect )
{
	MWnd::paint( ptGraphics, rect );

	unsigned int width = rect.getWidth();
	unsigned int height = rect.getHeight();

	// scroll position
	MPoint target = MWnd::getScrollPos();
	int x = target.getX();
	int y = target.getY();

	ptGraphics->setOffset( ptGraphics->getOffset() - target );

	// draw grid
	unsigned int notes = ivPtNoteList->getOctaveCount() * NOTES_PER_OCTAVE + 1;

	// TODO paint grid
	MGuiUtils::paintGrid(
		ptGraphics,
		rect,
		target,
		*ivPtPartSize,
		notes,
		ivPtNoteList->getMaxLength(),
		MColorMap::BK_COLOR4 );

	// draw noteControls
	unsigned int count = ivControls.size();
	for( unsigned int i=0;i<count;i++ )
		((MDCControl*) ivControls[i] )->paintControl( ptGraphics );
	if( ivPtEditControl != 0 )
		ivPtEditControl->paintControl( ptGraphics );

	// draw selection-rect
	if( getEditMode() == MNoteGrid::SELECT_MODE && ivMouseCaptured && ! ivMoveMode )
	{
		ptGraphics->drawRect( MRect( ivPointMouseDown, ivPointMouseUp ), MColorMap::BK_COLOR1 );
	}

	ptGraphics->setOffset( ptGraphics->getOffset() + target );
}

void MNoteGrid::updateFromModel()
{
	TRACE( "<MNoteGrid::updateFromModel()>\n" );

	deleteControls();

	MDefaultSynthNote *ptNote = (MDefaultSynthNote*) ivPtNoteList->getFirstItem();
	while( ptNote != 0 )
	{
		CreateNoteControl( ptNote );
		ptNote = (MDefaultSynthNote*) ptNote->getNext();
	}

	repaint();
	fireContentChanged();
}

void MNoteGrid::updateFromPartSize()
{
	TRACE( "<MNoteGrid::updateFromPartSize()>\n" );

	for( unsigned int i=0;i<ivControls.size();i++ )
	{
		layoutControl( ivControls[ i ] );
	}
	repaint();
}

void MNoteGrid::deleteSelected()
{
	while( ivPtSelectionManager->getSelectionCount() > 0 )
	{
		MNoteControl *control = (MNoteControl*) ivPtSelectionManager->getSelectionAt( 0 );
		ivPtSelectionManager->removeFromSelection( (unsigned int) 0 );

		// remove from deleted MArrangeBars from ivArArrangeBars
		for( unsigned int j=0;j<ivControls.size();j++ )
			if( ivControls[ j ] == control )
				ivControls.erase( ivControls.begin() + j );

		ivPtNoteList->deleteItem( control->getNote() );
		delete control;
	}

	repaint();
	fireContentChanged();
}

int MNoteGrid::getMaxRows()
{
	return ivPtNoteList->getOctaveCount() * NOTES_PER_OCTAVE;
}

int MNoteGrid::getMaxCols()
{
	return ivPtNoteList->getMaxLength();
}

int MNoteGrid::getRow( MPoint point )
{
	int row = point.getY() / (int)ivPtPartSize->getHeight();
	//Convert to note-Value
	return getMaxRows() - ( row + 1 );
}

int MNoteGrid::getRow( MDCControl* ptControl )
{
	MNoteControl* ptNoteControl = (MNoteControl*) ptControl;
	MDefaultSynthNote *ptNote = (MDefaultSynthNote*) ptNoteControl->getNote();
	return (ivPtNoteList->getOctaveCount() * NOTES_PER_OCTAVE) - (ptNote->getValue()+1);
}

int MNoteGrid::getCol( MDCControl* ptControl )
{
	MNoteControl* ptNoteControl = (MNoteControl*) ptControl;
	MDefaultSynthNote *ptNote = (MDefaultSynthNote*) ptNoteControl->getNote();
	return ptNote->getIndex();
}

int MNoteGrid::getColSpan( MDCControl* ptControl )
{
	MNoteControl* ptNoteControl = (MNoteControl*) ptControl;
	MDefaultSynthNote *ptNote = (MDefaultSynthNote*) ptNoteControl->getNote();
	return ptNote->getDuration();
}

void MNoteGrid::setNoteList( MDefaultSynthNoteList *ptNoteList )
{
	ivPtNoteList = ptNoteList;
}

MDefaultSynthNoteList *MNoteGrid::getNoteList()
{
	return ivPtNoteList;
}

MDCControl *MNoteGrid::CreateNoteControl( MDefaultSynthNote *note )
{
	TRACE( "<MNoteGrid::CreateNoteControl()>\n" );

	MNoteControl *ptControl = new MNoteControl( note );
	ivControls.push_back( ptControl );
	layoutControl( ptControl );

	return ptControl;
}

void MNoteGrid::onMouseDown( MMouseEvent* anEvent )
{
	if( ivMouseCaptured )
	{
		MDCGrid::onMouseUp( anEvent );
		return;
	}

	MDCGrid::onMouseDown( anEvent );

	MNoteControl *ptControl = (MNoteControl *) getControlAt( anEvent->getPoint() );
	if( ptControl == 0 )
	{ // No control found at the clicked position
		TRACE( "NO CONTROL FOUND>\n" );
		if( getEditMode() == MNoteGrid::DRAW_MODE )
		{ // DRAW_MODE -> try to create a new note and notecontrol an the given point
			MDCControl* ptControl = 0;
			if( (ptControl = createControl( anEvent->getPoint() )) != 0 )
			{
				startControlEditing( ptControl );
				repaint();
				this->fireContentChanged();
			}
		}
	}
	else
	{ // A existing control was found at the clicked position
		ivCurrentEditOffset = anEvent->getPoint().sub( ptControl->getPosition().getPoint() );
		TRACE( "A CONTROL WAS FOUND>\n" );
		if( getEditMode() == MNoteGrid::DRAW_MODE )
		{ // DRAW_MODE -> START redrawing note-length
			startControlEditing( ptControl );
		}
		else if( getEditMode() == MNoteGrid::SELECT_MODE )
		{ // SELECT_MODE -> Select ptControl
			if( anEvent->getShiftPressed() )
			{ // MULTIPLE SELECITON
				if( ! ivPtSelectionManager->isInSelection( ptControl ) )
				{
					ivPtSelectionManager->addToSelection( ptControl );
					repaint();
				}
			}
			else
			{ // SINGLE SELECTION
				if( ! ivPtSelectionManager->isInSelection( ptControl ) )
				{
					ivPtSelectionManager->singleSelection( ptControl );
					repaint();
				}
			}
			// Start DRAGGING
			startControlMoving( ptControl );
		}
	}

	// Capture mouse
	if( ! ivMouseCaptured )
	{
		ivMouseCaptured = true;
		ivPointMouseDown = anEvent->getPoint();
	}
}

void MNoteGrid::onMouseUp( MMouseEvent* anEvent )
{
	MDCGrid::onMouseUp( anEvent );

	// If mouse was captured -> release capture
	if( ivMouseCaptured )
	{
		if( getEditMode() == MNoteGrid::DRAW_MODE )
		{
			// If a note was resized -> turn of resize-mode
			if( ivNoteEditMode )
			{
				ivNoteEditMode = false;
				ivPtEditControl = 0;
			}
		}
		else if( getEditMode() == MNoteGrid::SELECT_MODE )
		{
			if( ivMoveMode )
			{
				stopControlMoving( anEvent->getPoint() );
			}
			else if( anEvent->getPoint().equals( ivPointMouseDown ) )
			{ // If mouse not moved (single click) then reset selection
				ivPtSelectionManager->resetSelection();
			}
		}

		// release mouse-capture
		ivMouseCaptured = false;
		ReleaseCapture();
		ivPointMouseUp = anEvent->getPoint();

		// Update graphics
		repaint();
	}
}

void MNoteGrid::onMouseMove( MMouseEvent* anEvent )
{
	MDCGrid::onMouseMove( anEvent );

	if( ivMouseCaptured )
	{
		if( getEditMode() == MNoteGrid::SELECT_MODE )
		{
			if( ivMoveMode )
			{ // MOVE
				ASSERT( ivPtEditControl != 0 );

				ivPtEditControl->moveControl( anEvent->getPoint().sub( ivCurrentEditOffset ) );
				MPoint topLeft = ivPtEditControl->getPosition().getPoint();
				unsigned int count = ivPtSelectionManager->getSelectionCount();
				for( unsigned int i=0;i<count;i++ )
				{
					MDCControl* ptControl = ivPtSelectionManager->getSelectionAt( i );
					if( ptControl != ivPtEditControl )
						ptControl->moveControl( topLeft.add( ptControl->getPoint() ) );
				}
			}
			else
			{ // SELECTON
				ivPointMouseUp = anEvent->getPoint();
				updateSelection( MRect( ivPointMouseDown, ivPointMouseUp ) );
			}
			repaint();
		}
		else if( getEditMode() == MNoteGrid::DRAW_MODE && ivNoteEditMode )
		{
			ASSERT( ivPtEditControl != 0 );

			// Do something if mouse was moved right of the first pressed point (RETHINK about!!!)
			if( anEvent->getPoint().getX() > ivPointMouseDown.getX() )
			{
				if( updateNoteEditing( anEvent->getPoint() ) )
				{
					repaint();
				}
			}
		}
	}
}

bool MNoteGrid::onSetCursor()
{
	HCURSOR c = 0;
	/*if( getEditMode() == MNoteGrid::SELECT_MODE )
		c = LoadCursor( NULL, IDC_ARROW );
	else if( getEditMode() == MNoteGrid::DRAW_MODE )
		c = LoadCursor( MApp::getInstance()->getHInstance(), MAKEINTRESOURCE( IDC_PEN ) );
	if( c == 0 )
		MLogger::logError( "MNoteGrid::onSetCursor: LoadCursor failed" );
	else
		SetCursor( c );*/
	return c != 0;
}

MDCControl *MNoteGrid::createControl( MPoint point )
{
	MDCControl *createdControl = 0;

	int noteValue = getRow( point );
	if( noteValue >= 0 )
	{
		int position = MDCGrid::getCol( point );
		if( position >= 0 )
		{
			if( ivPtNoteList->isFree( position ) )
			{
				MDefaultSynthNote *note = new MDefaultSynthNote();
				TRACE( "Creating %s\n", Synth::gvNotesMatrixHuman[ noteValue ] );
				note->setValue( noteValue );
				note->setIndex( position );
				note->setDuration( 1 );
				ivPtNoteList->MList::insertItem( note );
				createdControl = CreateNoteControl( note );
			}
		}
	}

	return createdControl;
}

void MNoteGrid::startControlEditing( MDCControl* ptControl )
{
	ASSERT( getEditMode() == MNoteGrid::DRAW_MODE );

	ivPtEditControl = ptControl;
	ivPointMouseDown = ptControl->getPosition().getPoint();
	ivNoteEditMode = true;
	ivMouseCaptured = true;
}

void MNoteGrid::stopControlMoving( MPoint point )
{
	MPoint div = ivPointMouseUp.sub( ivPointMouseDown );
	int oldCol = MDCGrid::getCol( ivPointMouseDown );
	int newCol = MDCGrid::getCol( ivPointMouseUp );
	int oldRow = getRow( ivPointMouseDown );
	int newRow = getRow( ivPointMouseUp );
	int colDiv = newCol - oldCol;
	int rowDiv = newRow - oldRow;
	/*TRACE( "colDiv: %i RowDif: %i oldCol: %i oldRow: %i newCol: %i newRow: %i\n",
		colDiv,
		rowDiv,
		oldCol,
		oldRow,
		newCol,
		newRow );*/

	// b1  b0
	// 0   0   STD_LO
	// 0   1   STD_HI
	// 1   0   REV_LO
	// 1   1   REV_HI
	// colDiv > 0 (rightMove) -> b0 = 1 (reverse)
	// rowDiv > 0 (upMove) -> b1 = 1 (hiPrio)
	int st = 0;
	if( colDiv > 0 ) 
		st |= 2;
	if( rowDiv > 0 )
		st |= 1;

	//TRACE( "COMPARATORMODE = %i\n", st );

	MMCComparator::SortType sortType = (MMCComparator::SortType) st;

	MMCComparator* ptComparator = new MMCComparator( sortType );
	MDCList* ptDcList = new MDCList( ptComparator, ivPtSelectionManager );

	unsigned int count = ptDcList->getControlCount();
	for( unsigned int i=0;i<count;i++ )
	{
		MNoteControl* ptControl = (MNoteControl*) (ptDcList->getControl( i ));
		/*TRACE(
			"X: %i Y: %i\n",
			ptControl->getPosition().TopLeft().x,
			ptControl->getPosition().TopLeft().y );*/
		moveControl(
			ptControl,
			rowDiv,
			colDiv );
		layoutControl( ptControl );
	}

	ivMoveMode = false;
	ivPtEditControl = 0;

	fireContentChanged();

	delete ptDcList;
	delete ptComparator;
}

void MNoteGrid::moveControl( MDCControl* _ptControl, int rowDiv, int colDiv )
{
 	MNoteControl* ptControl = (MNoteControl*) _ptControl;
	MDefaultSynthNote *note = (MDefaultSynthNote*) ptControl->getNote();
	int col = getCol( ptControl );
	int row = getRow( ptControl );
	int newCol = col + colDiv;
	int newRow = row - rowDiv;
		
	TRACE( "moveControl(oldCol: %i oldRow: %i newCol: %i newRow: %i)\n",
		col,
		row,
		newCol,
		newRow );

	if( ( newRow >= 0 ) &&
		( newRow < getMaxRows() ) &&
		( newCol >= 0 ) &&
		( newCol < getMaxCols() ) &&
		( ivPtNoteList->canInsertAt( note, newCol, note->getDuration(), getMaxRows() - (newRow+1)) ) )
	{
		ivPtNoteList->unlink( note );
		note->setValue( getMaxRows() - (newRow+1) );
		note->setIndex( newCol );
		ivPtNoteList->MList::insertItem( note );
	}
}

bool MNoteGrid::updateNoteEditing( MPoint point )
{
	ASSERT( ivPtEditControl != 0 );

	bool back = false;

	MDefaultSynthNote *editedNote = (MDefaultSynthNote*) ((MNoteControl*)ivPtEditControl)->getNote();

	int targetPos = MDCGrid::getCol( point );
	if( targetPos == -1 )
	{
		targetPos = ivPtNoteList->getMaxLength();
	}
	int noteStart = editedNote->getIndex();

	// If new end of note is after start of note then resize
	if( targetPos >= noteStart )
	{
		int targetLength = targetPos - noteStart + 1;
		int maxNoteLength = ivPtNoteList->getMaxPossibleLength( editedNote );
		if( targetLength > maxNoteLength )
		{
			targetLength = maxNoteLength;
		}
		editedNote->setDuration( targetLength );
		layoutControl( ivPtEditControl );
		back = true;
	}

	return back;
}

MDCControl *MNoteGrid::getControlAt( MPoint point )
{
	MNoteControl *control = 0;

	for( unsigned int i=0;i<ivControls.size();i++ )
	{
		// If point is in the position-rect of the noteControl, the control is found
		if( ((MNoteControl*)ivControls[i])->getPosition().contains( point ) )
		{
			control = (MNoteControl*) ivControls[i];
			break;
		}
	}

	return control;
}

void MNoteGrid::deleteControls()
{
	ivPtSelectionManager->resetSelection();

	for( unsigned int i=0;i<ivControls.size();i++ )
		SAFE_DELETE( ivControls[ i ] );

	ivControls.clear();
}

void MNoteGrid::deleteControl( MDCControl* _ptControl )
{
	MNoteControl* ptControl = (MNoteControl*) _ptControl;
	int index = GetIndexOfControl( ptControl );
	if( index > -1 )
		ivControls.erase( ivControls.begin() + index );

	ivPtNoteList->deleteItem( ptControl->getNote() );
	SAFE_DELETE( ptControl );
}

void MNoteGrid::ScrollToVisible()
{
/*	CRect rect = getRect();
	int avg = ivPtNoteList->getAverageNoteValue();
	int noteCount = ivPtNoteList->getOctaveCount() * 12;
	int negAvg = noteCount - avg;
	int yOffset = negAvg * ivPartSize.cy + rect.Height() / 2;
	ScrollToDevicePosition( CPoint( 0, yOffset ) );*/
};
