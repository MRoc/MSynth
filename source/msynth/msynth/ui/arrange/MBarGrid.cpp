#include "MBarGrid.h"
#include <gui/dialog/MMessageBox.h>

#define BAR_WIDTH 30 
#define BAR_HEIGHT 20

INIT_RTTI( MBarGrid, "MBarGrid" );
INIT_RTTI( MBarGrid::MSongListener, "MBarGrid::MSongListener" );

MBarGrid::MBarGrid( MSong *ptSong, MTimeController* ptTimeControl, IBpmControl* ptBpmControl, MSize* ptSize ) :
	MDCGrid( ptSize ),
	ivPtSong( 0 ),
	ivPtTimeControl( ptTimeControl ),
	ivBarEditMode( false ),
	ivCurrentResolution( 16 ),
	ivPtBpmControl( ptBpmControl ),
	ivPtListener( 0 )
{
	ASSERT( ptSong && ptTimeControl );

	ivBkColor = MColorMap::BK_COLOR3;

	setSong( ptSong );
	setEditMode( MDCGrid::SELECT_MODE );
}
MBarGrid::~MBarGrid()
{
	setSong( 0 );
}

void MBarGrid::setSong( MSong *ptSong )
{
	if( ivPtSong )
		SAFE_DELETE( ivPtListener );

	ivPtSong = ptSong;

	if( ivPtSong )
		ivPtListener = new MSongListener( ivPtSong, this );
}

MSong *MBarGrid::getSong()
{
	return ivPtSong;
}

void MBarGrid::paint( IGraphics* pG, const MRect &rect )
{
	MPoint target = getScrollPos();
	int x = target.getX();
	int y = target.getY();
	unsigned int width = rect.getWidth();
	unsigned int height = rect.getHeight();

	pG->fillRect( rect, ivBkColor );

	pG->setOffset( pG->getOffset() - target );

	// draw grid
	MGuiUtils::paintGrid(
		pG,
		rect,
		target,
		*ivPtPartSize,
		ivPtSong->getBarContainerCount(),
		ivPtSong->getMaxBarCount(),
		MColorMap::BK_COLOR4 );

	// draw bar Controls
	unsigned int count = ivControls.size(); 
	for( unsigned int i=0;i<count;i++ )
	//	if( ! ivControls[i]->getSelected() )
			ivControls[i]->paintControl( pG );
	for( i=0;i<count;i++ )
		if( ivControls[i]->getSelected() )
			ivControls[i]->paintControl( pG );
	if( ivPtEditControl )
		ivPtEditControl->paintControl( pG );

	// draw selection-rect
	if( getEditMode() == MDCGrid::SELECT_MODE && getMouseDownLeft() && ! ivMoveMode )
		pG->drawRect(
			MRect( ivPointMouseDown, ivPointMouseUp ),
			MColorMap::FG_COLOR3 );

	pG->setOffset( pG->getOffset() + target );
}

void MBarGrid::updateFromModel()
{
	// Delete all existing MArrangeBars
	deleteControls();
		
	// Create new MArraneBars for every Channel of the Song
	unsigned int count = ivPtSong->getBarContainerCount();
	for( unsigned int i=0;i<count;i++ )
	{
		MBar *bar = (MBar*) ivPtSong->getBarContainer( i )->getFirstItem();
		while( bar != 0 )
		{
			MBarControl* newBarControl = createBarControl( bar, i );
			bar = (MBar*) bar->getNext();
		}
	}
}

void MBarGrid::updateFromPartSize()
{
	int count = ivControls.size();
	for( int i=0;i<count;i++ )
		MDCGrid::layoutControl( ivControls[ i ] );
	repaint();
}

void MBarGrid::updateSelection( MRect rect )
{
	MDCGrid::updateSelection( rect );
}

MBarControl *MBarGrid::createBarControl( MBar *ptBar, int channel )
{
	MBarControl* ptControl = new MBarControl( ptBar );
	ptControl->setChannel( channel );
	MDCGrid::layoutControl( ptControl );

	ivControls.push_back( ptControl );

	return ptControl;
}

MDCControl *MBarGrid::createControl( MPoint point )
{
	MBar* ptBar = 0;
	MBarControl *createdControl = 0;
	int row = MDCGrid::getRow( point );
	int col = MDCGrid::getCol( point );
	if( row >= 0 && col >= 0 )
		if( (ptBar = ivPtSong->createBar( row, col, ivCurrentResolution )) != 0 )
			createdControl = createBarControl( ptBar, row );

	return createdControl;
}

void MBarGrid::layoutControl( MDCControl *ptControl, int startsAt, int stopsAt )
{
	ASSERT( ptControl != 0 );

	MBarControl* ptBC = (MBarControl*) ptControl;

	int x = startsAt * ivPtPartSize->getWidth();
	int y = ptBC->getChannel() * ivPtPartSize->getHeight();
	int width = ivPtPartSize->getWidth() * ( stopsAt - startsAt + 1 ) + 1;
	int height = ivPtPartSize->getHeight() + 1;

	ptControl->setPosition(
		MRect(
			x,
			y,
			width,
			height ) );
}

bool MBarGrid::hasSelectedControls()
{
	return ivPtSelectionManager->getSelectionCount() > 0;
}

MBarControl* MBarGrid::getFirstSelection()
{
	if( ivPtSelectionManager->getSelectionCount() )
		return (MBarControl*) ivPtSelectionManager->getSelectionAt( 0 );
	else
		return 0;
}

void MBarGrid::deleteSelected()
{
	TRACE( "<MBarGrid::deleteSelected()>\n" );

	while( ivPtSelectionManager->getSelectionCount() > 0 )
	{
		MBarControl* control = (MBarControl*) ivPtSelectionManager->getSelectionAt( 0 );
		MBar* ptBar = control->getBar();
		int channel = control->getChannel();
		deleteControl( control );
		ivPtSong->getBarContainer( channel )->deleteItem( ptBar );
	}
	repaint();
}

void MBarGrid::deleteFirstSelection()
{
	TRACE( "<MBarGrid::deleteSelected()>\n" );

	if( ivPtSelectionManager->hasSelection() )
	{
		MBarControl* control = (MBarControl*) ivPtSelectionManager->getSelectionAt( 0 );
		MBar* ptBar = control->getBar();
		int channel = control->getChannel();
		deleteControl( control );
		ivPtSong->getBarContainer( channel )->deleteItem( ptBar );
		repaint();
	}
}

void MBarGrid::onMouseDown( MMouseEvent* anEvent )
{
	MDCGrid::onMouseDown( anEvent );

	ivPointMouseDown = ivPointMouseUp = anEvent->getPoint();

	MDCControl *ptControl = getControlAt( ivPointMouseDown );
	if( ptControl == 0 )
	{ // No control found at the clicked position
		if( getEditMode() == MDCGrid::DRAW_MODE )
		{ // DRAW_MODE -> try to create a new bar and barcontrol at the given point
			MDCControl *ptControl = 0;
			if( ( ptControl = createControl( ivPointMouseDown ) ) != 0 )
			{
				startControlEditing( ptControl );
				repaint();
			}
		}
	}
	else
	{ // A existing control was found at the clicked position
		ivCurrentEditOffset = ivPointMouseDown - ptControl->getPosition().getPoint();
		TRACE( "editoffset: %s\n", ivCurrentEditOffset.toString().getData() );
		if( getEditMode() == MDCGrid::DRAW_MODE )
		{ // DRAW_MODE -> START redrawing bar-length
			startControlEditing( ptControl );
		}
		else if( getEditMode() == MDCGrid::SELECT_MODE )
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
			startControlMoving( (MBarControl*) ptControl );
		}
		else if( getEditMode() == MDCGrid::GLOW_MODE )
			glowBar( anEvent->getPoint(), ((MBarControl*)ptControl) );
		else if( getEditMode() == MDCGrid::CUT_MODE )
			cutBar( anEvent->getPoint(), ((MBarControl*)ptControl) );
	}
}

void MBarGrid::onMouseUp( MMouseEvent* anEvent )
{
	// If mouse was captured -> release capture
	if( getMouseDownLeft() )
	{
		if( getEditMode() == MDCGrid::DRAW_MODE )
		{
			// If a bar was resized -> turn of resize-mode and set barlength
			if( ivBarEditMode )
			{
				ivBarEditMode = false;
				updateControlEditing( anEvent->getPoint() );
				stopControlEditing();
			}
		}
		else if( getEditMode() == MDCGrid::SELECT_MODE )
		{
			if( ivMoveMode )
				stopControlMoving( anEvent->getPoint() );
			else if( anEvent->getPoint().equals( ivPointMouseDown ) )
				ivPtSelectionManager->resetSelection();
		}
		repaint();
	}

	MDCGrid::onMouseUp( anEvent );
}

void MBarGrid::onMouseDoubleClick( MMouseEvent* anEvent )
{
	MDCGrid::onMouseDoubleClick( anEvent );

	if( getEditMode() == MDCGrid::SELECT_MODE || 
		getEditMode() == MDCGrid::DRAW_MODE )
	{
		MBarControl *ptControl = (MBarControl*) getControlAt( anEvent->getPoint() );
		if( ptControl )
		{
			int channel = MDCGrid::getRow( anEvent->getPoint() );
			MAbstractNoteFrame *ptEditor = ptControl->getEditor();
			if( ptEditor == 0 )
			{
				ptEditor =
					MNoteFactory::getInstance()->getNoteEditor(
						ptControl->getBar()->getNoteList()->getTypeId() );

				if( ptEditor == 0 )
				{
					MMessageBox::showError( "OPEN_NOTEEDITOR_FAILED" );
				}
				else
				{
					ptEditor->setInstrument( ivPtSong->getBarContainer( channel )->getInstrument() );
					ptEditor->setBar( ptControl->getBar() );
					ptEditor->setBpmControl( this->ivPtBpmControl );

					ptControl->setEditor( ptEditor );
					ptEditor->create( MRect( 0, 0, 640, 480 ), (MTopWnd*)getTopParent() ); 
				}
			}
			if( ptEditor )
				ptEditor->setVisible( true );
		}
		else if( getEditMode() == MDCGrid::DRAW_MODE )
		{
			// DRAW_MODE -> try to create a new bar and barcontrol at the given point
			if( (ptControl = (MBarControl*) createControl( anEvent->getPoint() )) != 0 )
			{
				startControlEditing( ptControl );
				updateControlEditing( anEvent->getPoint() );
				stopControlEditing();
				repaint();
			}
		}
	}
}

void MBarGrid::onMouseMove( MMouseEvent* anEvent )
{
	MDCGrid::onMouseMove( anEvent );

	bool redraw = false;

	if( this->getMouseDownLeft() )
	{
		if( getEditMode() == MDCGrid::SELECT_MODE )
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
				redraw = true;
			}
			else
			{ // SELECTON
				ivPointMouseUp = anEvent->getPoint();
				updateSelection( MRect( ivPointMouseDown, ivPointMouseUp ) );
				redraw = true;
			}
		}
		else if( getEditMode() == MDCGrid::DRAW_MODE && ivBarEditMode )
		{
			ASSERT( ivPtEditControl != 0 );
			// Do something if mouse was moved right of the first pressed point (RETHINK about!!!)
			updateControlEditing( anEvent->getPoint() );
			redraw = true;
		}
	}

	if( redraw )
		repaint();
}

bool MBarGrid::onSetCursor()
{
	HCURSOR cursor = 0;

	/*if( getEditMode() == MDCGrid::SELECT_MODE )
		cursor = LoadCursor( NULL, IDC_ARROW );
	else if( getEditMode() == MDCGrid::DRAW_MODE )
		cursor = LoadCursor( MApp::getInstance()->getHInstance(), MAKEINTRESOURCE( IDC_PEN ) );
	else if( getEditMode() == MDCGrid::GLOW_MODE )
		cursor = LoadCursor( MApp::getInstance()->getHInstance(), MAKEINTRESOURCE( IDC_GLOW ) );
	else if( getEditMode() == MDCGrid::CUT_MODE )
		cursor = LoadCursor( MApp::getInstance()->getHInstance(), MAKEINTRESOURCE( IDC_CUT ) );

	if( cursor == 0 )
		MLogger::logError(
			"MBarGrid::onSetCursor: LoadCursor failed" );
	else
		SetCursor( cursor );*/

	return cursor != 0;
}

void MBarGrid::startControlEditing( MDCControl *ptControl )
{
	TRACE(
		"MBarGrid::startControlEditing index=\"%i\" length=\"%i\"\n",
		((MBarControl*)ptControl)->getBar()->getIndex(),
		((MBarControl*)ptControl)->getBar()->getLength() );

	ASSERT( ptControl != 0 );
	ASSERT( getEditMode() == MDCGrid::DRAW_MODE );

	ivPtEditControl = ptControl;
	ivPointMouseDown = ptControl->getPosition().getPoint();
	ivBarEditMode = true;
}

bool MBarGrid::updateControlEditing( MPoint point )
{
	ASSERT( ivPtEditControl != 0 );
	ASSERT( getEditMode() == MDCGrid::DRAW_MODE );

	/*TRACE(
		"MBarGrid::updateControlEditing index=\"%i\" length=\"%i\"\n",
		((MBarControl*)ivPtEditControl)->getBar()->getIndex(),
		((MBarControl*)ivPtEditControl)->getBar()->getLength() );*/

	MSynchronize cls( &ivPtSong->ivCriticalSection );

	bool back = false;
	bool layouted = false;

	MBarControl* ptBC = (MBarControl*)ivPtEditControl;
	MBar *editedBar = ptBC->getBar();
	int channel = ptBC->getChannel();
	int targetPos = MDCGrid::getCol( point );

	int barStartsAt = editedBar->getIndex();

	if( targetPos >= barStartsAt )
	{
		int targetLength = targetPos - barStartsAt + 1;
		int maxNoteLength = ivPtSong->getBarContainer( channel )->getMaxPossibleLength( editedBar );
		if( targetLength > maxNoteLength )
		{
			targetLength = maxNoteLength;
		}

		if( ivBarEditMode )
		{ // ONLY CHANGE GUI-WORK (Still dragging)
			layoutControl( ivPtEditControl, barStartsAt, barStartsAt + targetLength - 1 );
			layouted = true;
		}
		else
		{ // CHANGE DATA AND UPDATE GUI (mouse up)
			editedBar->setLength( targetLength );
			MDCGrid::layoutControl( ivPtEditControl );
			if( ((MBarControl*)ivPtEditControl)->getEditor() != 0 )
				((MBarControl*)ivPtEditControl)->getEditor()->updateFromModel();
			layouted = true;
		}
		back = true;
	}
	if( ! layouted )
		MDCGrid::layoutControl( ivPtEditControl );
	
	return back;
}

void MBarGrid::stopControlEditing()
{
	ASSERT( ivPtEditControl );

	TRACE(
		"MBarGrid::stopControlEditing index=\"%i\" length=\"%i\"\n",
		((MBarControl*)ivPtEditControl)->getBar()->getIndex(),
		((MBarControl*)ivPtEditControl)->getBar()->getLength() );

	ivPtEditControl = 0;
	ivBarEditMode =	false;
}

void MBarGrid::startControlMoving( MBarControl *ptControl )
{
	TRACE( "MBarGrid::startControlMoving\n" );

	ASSERT( ivPtEditControl == 0 );
	ASSERT( ptControl != 0 );
	ASSERT( getEditMode() == MDCGrid::SELECT_MODE );

	ivPtEditControl = ptControl;
	ivPointMouseDown = ptControl->getPosition().getPoint();
	ivMoveMode = true;
}

void MBarGrid::stopControlMoving( MPoint point )
{
	TRACE( "MBarGrid::stopControlMoving\n" );

	ASSERT( ivPtEditControl != 0 );
	ASSERT( getEditMode() == MDCGrid::SELECT_MODE );

	MPoint div = ivPointMouseUp - ivPointMouseDown;
	int oldCol = MDCGrid::getCol( ivPointMouseDown );
	int newCol = MDCGrid::getCol( point );
	int oldRow = MDCGrid::getRow( ivPointMouseDown );
	int newRow = MDCGrid::getRow( point );

	TRACE( "old: %i/%i new: %i/%i\n", oldCol, oldRow, newCol, newRow );
	int colDiv = newCol - oldCol;
	int rowDiv = newRow - oldRow;

	colDiv -= (ivCurrentEditOffset.getX() / ivPtPartSize->getWidth() );
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

	MBCComparator::SortType sortType = (MBCComparator::SortType) st;
	MBCComparator* ptComparator = new MBCComparator( sortType );
	MDCList* ptDcList = new MDCList( ptComparator, ivPtSelectionManager );

	unsigned int count = ptDcList->getControlCount();
	for( unsigned int i=0;i<count;i++ )
	{
		MBarControl* ptControl = (MBarControl*) (ptDcList->getControl( i ));
		moveControl(
			ptControl,
			rowDiv,
			colDiv  );
		MDCGrid::layoutControl( ptControl );
	}

	ivMoveMode = false;
	ivPtEditControl = 0;

	delete ptDcList;
	delete ptComparator;
}

void MBarGrid::moveControl( MDCControl* _ptControl, int rowDiv, int colDiv )
{
 	MBarControl* ptControl = (MBarControl*) _ptControl;
	unsigned int col = getCol( ptControl );
	unsigned int row = getRow( ptControl );

	if( ivPtSong->moveBar( row, col, rowDiv, colDiv ) )
	{
		unsigned int newRow = row + rowDiv;
		if( newRow != row )
			ptControl->closeEditor();
		ptControl->setChannel( newRow );
	}
}

bool MBarGrid::glowBar( MPoint point, MBarControl* control )
{
	bool back = false;
	MBar *bar = control->getBar();
	MBar *nextBar = (MBar*) bar->getNext();
	if( nextBar )
	{
		ivPtSelectionManager->resetSelection();

		MBarControl *nextControl = getControlForBar( nextBar );

		if( (back = ivPtSong->glow( MDCGrid::getRow( point ), bar )) )
		{
			// delete control 1
			deleteControl( nextControl );

			// if open editor -> update editor
			if( control->getEditor() )
			{
				control->getEditor()->doLayout();
				control->getEditor()->updateFromModel();
			}

			MDCGrid::layoutControl( control );
			
			repaint();
		}
	}

	return back;
}

bool MBarGrid::cutBar( MPoint point, MBarControl* control )
{
	bool back = false;

	int row = MDCGrid::getRow( point );
	int index = MDCGrid::getCol( point );

	MBar* ptBar = ivPtSong->cutBar( row, index, control->getBar() );
	if( ptBar )
	{
		createBarControl( ptBar, row );
		MDCGrid::layoutControl( control );
		if( control->getEditor() )
		{
			control->getEditor()->doLayout();
			control->getEditor()->updateFromModel();
		}
		back = true;
	}

	return back;
}

void MBarGrid::deleteControl( MDCControl* _ptControl )
{
	if( ivPtSelectionManager->isInSelection( _ptControl ) )
		ivPtSelectionManager->removeFromSelection( _ptControl );

	MBarControl* ptControl = (MBarControl*) _ptControl;
	// open editor ? close it
	if( ptControl->getEditor() )
	{
		MAbstractNoteFrame* ptEditor = ptControl->getEditor();
		ptControl->setEditor( 0 );
		delete ptEditor;
	}

	int count = ivControls.size();
	for( int i=0;i<count;i++ )
		if( ivControls[ i ] == ptControl )
		{
			ivControls.erase( ivControls.begin() + i );
			break;
		}

	delete ptControl;
	repaint();
}

MBarControl* MBarGrid::getControlForBar( MBar* ptBar )
{
	MBarControl *back = 0;

	unsigned int count = ivControls.size();
	for( unsigned int i=0;i<count;i++ )
		if( ((MBarControl*)(ivControls[ i ]))->getBar() == ptBar )
		{
			back = (MBarControl*) ivControls[ i ];
			break;
		}

	return back;
}

void MBarGrid::barAdded( MBar* ptBar, int row )
{
	MBarControl* newControl = createBarControl( ptBar, row );
	MDCGrid::layoutControl( newControl );
	repaint();
}

void MBarGrid::changeResolution( int resolution )
{
	MSynchronize cls( &ivPtSong->ivCriticalSection );

	ivCurrentResolution = resolution;

	unsigned int count = ivPtSelectionManager->getSelectionCount();
	for( unsigned int i=0;i<count;i++ )
	{
		MBarControl* control = (MBarControl*) ivPtSelectionManager->getSelectionAt( i );
		control->getBar()->setNotesPerBar( resolution );
		if( control->getEditor() != 0 )
			control->getEditor()->updateFromModel();
	}
}

void MBarGrid::copy()
{
	if( ivPtSelectionManager->hasSelection() )
	{
		MBCBWList* ptList = getCopy();
		MClipBoard::getInstance()->setContent( ptList );
		ivPtTimeControl->setCurrentPatternEx( ptList->getIndex() + ptList->getLength() + 1 );
	}
	else
		MMessageBox::showError( "NO_BARS_SELECTED" );
}

void MBarGrid::cut()
{
	if( ivPtSelectionManager->hasSelection() )
	{
		MBCBWList* ptList = getCopy();
		MClipBoard::getInstance()->setContent( ptList );
		ivPtTimeControl->setCurrentPatternEx( ptList->getIndex() );
		deleteSelected();
	}
	else
		MMessageBox::showError( "NO_BARS_SELECTED" );
}

void MBarGrid::paste( int index )
{
	MClipBoardContext* ptContext = MClipBoard::getInstance()->getContent();
	if( ptContext )
	{
		int id = ptContext->getUserObject()->getTypeId();
		if( id == MBar::TYPE_ID ) // MBar
		{
			MBCBWList* ptList = (MBCBWList*) ptContext->getUserObject();
			paste( ptList, index );
			ivPtTimeControl->setCurrentPatternEx( index + ptList->getLength() + 1 );
		}
		else
		{
			TRACE( " -> NO MBar!!!>\n" );
			MMessageBox::showError(	"INVALID_PASTE_IN_BARGRID", String( ptContext->getUserObject()->getUserTypeName() ) );
		}
	}
	else
	{
		TRACE( " NO ClipBoardContextFound!!!>\n" );
		MMessageBox::showError( "CLIPBOARD_EMPTY" );
	}
}

int MBarGrid::getMaxRows()
{
	ASSERT( ivPtSong != 0 );
	return ivPtSong->getBarContainerCount();
}

int MBarGrid::getMaxCols()
{
	ASSERT( ivPtSong != 0 );
	return ivPtSong->getMaxBarCount();
}

int MBarGrid::getRow( MDCControl* ptControl )
{
	MBarControl* ptBC = (MBarControl*) ptControl;
	return ptBC->getChannel();
}

int MBarGrid::getCol( MDCControl* ptControl )
{
	MBarControl* ptBC = (MBarControl*) ptControl;
	return ptBC->getBar()->getIndex();
}

int MBarGrid::getColSpan( MDCControl* ptControl )
{
	MBarControl* ptBC = (MBarControl*) ptControl;
	return ptBC->getBar()->getLength();
}

MBCBWList* MBarGrid::getCopy()
{
	ASSERT( ivPtSelectionManager->hasSelection() );
	unsigned int offset = ivPtSong->getMaxBarCount();
	unsigned int lastBar = 0;
	unsigned int count = ivPtSelectionManager->getSelectionCount();
	for( unsigned int i=0;i<count;i++ )
	{
		MBarControl* ptBC = (MBarControl*) ivPtSelectionManager->getSelectionAt( i );
		unsigned int startsAt = ptBC->getBar()->getIndex();
		unsigned int stopsAt = ptBC->getBar()->getStopsAt();
		if( startsAt < offset )
			offset = ptBC->getBar()->getIndex();
		if( stopsAt > lastBar )
			lastBar = stopsAt;
	}
	MBCBWList* ptList = new MBCBWList();
	for( i=0;i<count;i++ )
	{
		MBarControl* ptBC = (MBarControl*) ivPtSelectionManager->getSelectionAt( i );
		MBar* ptCopy = (MBar*) ptBC->getBar()->getCopy();
		ptCopy->setIndex( ptCopy->getIndex() - offset );
		MBarClipBoardWrapper* ptW = new MBarClipBoardWrapper( ptCopy, ptBC->getChannel() );
		ptList->addBCBW( ptW );
	}

	ptList->setLength( lastBar - offset );
	ptList->setIndex( offset );

	return ptList;
}

void MBarGrid::paste( MBCBWList* ptList, unsigned int index )
{
	unsigned int count = ptList->getBCBWCount();
	for( unsigned int i=0;i<count;i++ )
	{
		MBarClipBoardWrapper* ptW = ptList->getBCBW( i );
		MBar* ptBar = ptW->getBar();
		if( ptW->getChannelNum() < ivPtSong->getBarContainerCount() )
		{
			MBarContainer* ptBC = ivPtSong->getBarContainer( ptW->getChannelNum() );
			if( ptBC->canInsertBar( ptBar, index + ptBar->getIndex() ) )
			{
				MBar* newBar = (MBar*) ptBar->getCopy();
				newBar->setIndex( newBar->getIndex() + index );
				ptBC->insertAt( newBar );
				createBarControl( newBar, ptW->getChannelNum() );
			}
		}
	}
	repaint();
}

void MBarGrid::closeChildFrames()
{
	unsigned int count = ivControls.size();	
	for( unsigned int i=0;i<count;i++ )
	{
		MBarControl* ptBC = (MBarControl*) (ivControls[ i ]);
		if( ptBC->getEditor() )
		{
			MAbstractNoteFrame* ptEditor = ptBC->getEditor();
			ptBC->setEditor( 0 );
			delete ptEditor;
		}
	}
}

void MBarGrid::storeChildFramePosition()
{
	unsigned int count = ivControls.size();	
	for( unsigned int i=0;i<count;i++ )
	{
		MBarControl* ptBC = (MBarControl*) (ivControls[ i ]);
		if( ptBC->getEditor() )
			ptBC->getEditor()->storeWindowPosition();
	}
}

MBarGrid::MSongListener::MSongListener( MSong* ptSong, MBarGrid* ptEditor ) :
	ivPtSong( ptSong ),
	ivPtEditor( ptEditor )
{
	ivPtSong->addSongListener( this );
}

MBarGrid::MSongListener::~MSongListener()
{
	ivPtSong->removeSongListener( this );
}

void MBarGrid::MSongListener::instrumentAdded( unsigned int _index )
{
	unsigned int index = ivPtSong->getBarContainerCount() - 1;
	MBar *bar = (MBar*) ivPtSong->getBarContainer( index )->getFirstItem();
	while( bar != 0 )
	{
		MBarControl* newBarControl = ivPtEditor->createBarControl( bar, index );
		bar = (MBar*) bar->getNext();
	}
}

void MBarGrid::MSongListener::instrumentRemoved( unsigned int index )
{
	ivPtEditor->ivPtSelectionManager->resetSelection();

	for( unsigned int i=0;i<ivPtEditor->ivControls.size();i++ )
		if( ((MBarControl*)(ivPtEditor->ivControls[i]))->getChannel() == index )
		{
			MBarControl* ptTemp = (MBarControl*) ivPtEditor->ivControls[i];
			ivPtEditor->ivControls.erase( ivPtEditor->ivControls.begin() + i );
			delete ptTemp;
			i--;
		}

	for( i=0;i<ivPtEditor->ivControls.size();i++ )
		if( ((MBarControl*)(ivPtEditor->ivControls[i]))->getChannel() > index )
		{
			((MBarControl*)(ivPtEditor->ivControls[i]))->setChannel(
				((MBarControl*)(ivPtEditor->ivControls[i]))->getChannel() - 1 );
			ivPtEditor->MDCGrid::layoutControl( ivPtEditor->ivControls[i] );
		}
}

void MBarGrid::MSongListener::instrumentsSwapped( unsigned int index1, unsigned int index2 )
{
	ivPtEditor->ivPtSelectionManager->resetSelection();

	unsigned int count = ivPtEditor->ivControls.size();
	for( unsigned int i=0;i<count;i++ )
	{
		MBarControl* ptBC = (MBarControl*) ivPtEditor->ivControls[ i ];
		if( ptBC->getChannel() == index1)
		{
			ptBC->setChannel( index2 );
			ivPtEditor->MDCGrid::layoutControl( ptBC );
		}
		else if( ptBC->getChannel() == index2 )
		{
			ptBC->setChannel( index1 );
			ivPtEditor->MDCGrid::layoutControl( ptBC );
		}
	}

	ivPtEditor->repaint();
}

void MBarGrid::MSongListener::songModelChanged()
{
	ivPtEditor->updateFromModel();
}

void MBarGrid::MSongListener::songDestroy()
{
	ivPtEditor->setSong( 0 );
}

String MBarGrid::MSongListener::getClassName()
{
	return "MBarGrid::MSongListener";
}

//-----------------------------------------------------------------------------
// # deleteControls
// Deletes all controls (but not the notes)
//-----------------------------------------------------------------------------
void MBarGrid::deleteControls()
{
	ivPtSelectionManager->resetSelection();

	unsigned int count = ivControls.size();
	for( unsigned int i=0;i<count;i++ )
		SAFE_DELETE( ivControls[ i ] );

	ivControls.clear();
}