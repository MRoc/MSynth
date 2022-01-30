/*

	MBeatBoxGrid (C)2001 MRoc hifiShock

	A ScrollPane displaying the MDefaultMessageNoteControls,
	doubel buffered, notes can be created, dragged and edited.

*/

#include "MBeatBoxGrid.h"
#include "MBeatBoxNoteEditor.h"
#include "../../../model/note/MDynamics.h"
#include <gui/dialog/MMessageBox.h>

int MBeatBoxGrid::gvInstanceCount = 0;
MObjectList* MBeatBoxGrid::gvDynamics = 0;
MObjectList* MBeatBoxGrid::gvPatterns = 0;

#define ID_DYN_BEG 13600
#define ID_DYN_END 13700
#define ID_PAT_BEG 14600
#define ID_PAT_END 14700

#define NOTE_BAR_WIDTH 20 
#define NOTE_BAR_HEIGHT 12

//-----------------------------------------------------------------------------
// + CONSTRUCTION
//-----------------------------------------------------------------------------
MBeatBoxGrid::MBeatBoxGrid(
	MBeatBox* ptDrumBox,
	MSize* ptPartSize,
	MDefaultMessageNoteList *ptNoteList )
	:
	MDCGrid( ptPartSize ),
	ivNoteEditMode( false ),
	ivPtBeatBox( ptDrumBox )/*,
	ivPtContextMenu( 0 ),
	ivPtDynamicsMenu( 0 ),
	ivCreatorsMenu( 0 )*/
{
	if( gvInstanceCount == 0 )
	{
		MBeatBoxGrid::gvDynamics = MDynamics::loadFromResource( "resource/dynamics" );
		MBeatBoxGrid::gvPatterns = MDynamics::loadFromResource( "resource/patterns" );
	}

/*	ivPtContextMenu = new CMenu();
	if( ivPtContextMenu->CreatePopupMenu() != TRUE )
	{
		MMessageBox::showError( "CreatePopupMenu failed" );
	}
	else
	{
		ivPtDynamicsMenu = new CMenu();
		ivCreatorsMenu = new CMenu();
		ivPtDynamicsMenu->CreatePopupMenu();
		ivCreatorsMenu->CreatePopupMenu();

		// add dynamics...
		unsigned int dynCount = MBeatBoxGrid::gvDynamics->getLength();
		for( unsigned int i=0;i<dynCount;i++ )
		{
			if( 
				ivPtDynamicsMenu->InsertMenu(
					0,
					MF_BYCOMMAND,
					ID_DYN_BEG+i,
					"dynamics " + ((MDynamics*)MBeatBoxGrid::gvDynamics->get( i ))->getName() ) != TRUE )
			{
				MMessageBox::showError( "insert menu failed" );
			}
		}

		// add pattern...
		unsigned int patCount = MBeatBoxGrid::gvPatterns->getLength();
		for( unsigned int i=0;i<patCount;i++ )
		{
			if( 
				ivCreatorsMenu->InsertMenu(
					0,
					MF_BYCOMMAND,
					ID_PAT_BEG+i,
					"pattern " + ((MDynamics*)MBeatBoxGrid::gvPatterns->get( i ))->getName() ) != TRUE )
			{
				MMessageBox::showError( "insert menu failed" );
			}
		}
	}*/

	setPartSize( ptPartSize );
	setNoteList( ptNoteList );

	setEditMode( MDCGrid::SELECT_MODE );

	gvInstanceCount++;
};

//-----------------------------------------------------------------------------
// + DESTRUCTION
//-----------------------------------------------------------------------------
MBeatBoxGrid::~MBeatBoxGrid()
{
	gvInstanceCount--;

	/*SAFE_DELETE( ivPtContextMenu );
	SAFE_DELETE( ivPtDynamicsMenu );
	SAFE_DELETE( ivCreatorsMenu );*/

	if( gvInstanceCount == 0 )
	{
		MBeatBoxGrid::gvDynamics->deleteAll();
		delete MBeatBoxGrid::gvDynamics;
		MBeatBoxGrid::gvDynamics = 0;

		MBeatBoxGrid::gvPatterns->deleteAll();
		delete MBeatBoxGrid::gvPatterns;
		MBeatBoxGrid::gvPatterns = 0;
	}
};

void MBeatBoxGrid::paint( IGraphics* ptGraphics, const MRect &rect )
{
	MPoint target = getScrollPos();
	int width = rect.getWidth();
	int height = rect.getHeight();
	int x = target.getX();
	int y = target.getY();

	ptGraphics->setOffset( ptGraphics->getOffset() - target );

	// draw grid
	MGuiUtils::paintGrid(
		ptGraphics,
		rect,
		target,
		*ivPtPartSize,
		getMaxRows(),
		getMaxCols(),
		MColorMap::BK_COLOR4 );

	// draw noteControls
	int count = ivControls.size();
	for( NoteControlIter i=ivControls.begin();i!=ivControls.end();i++ )
		(*i)->paintControl( ptGraphics );

	// draw selection-rect
	if( getEditMode() == MBeatBoxGrid::SELECT_MODE && ivMouseCaptured && ! ivMoveMode )
	{
		ptGraphics->drawRect( MRect( ivPointMouseDown, ivPointMouseUp ), MColorMap::BK_COLOR1 );
	}

	ptGraphics->setOffset( ptGraphics->getOffset() + target );
};

//-----------------------------------------------------------------------------
// + UPDATE FROM MODEL
//	Deletes all notecontrols (if existing) and creates new controls from the
//	notelist then calls doLayout()
//-----------------------------------------------------------------------------
void MBeatBoxGrid::updateFromModel()
{
	deleteControls();

	MDefaultMessageNote *ptNote = (MDefaultMessageNote*) ivPtNoteList->getFirstItem();
	while( ptNote != 0 )
	{
		createControls( ptNote );
		ptNote = (MDefaultMessageNote*) ptNote->getNext();
	}

	fireContentChanged();
};

//-----------------------------------------------------------------------------
// + COPY
// Copies the selected notes in a new notelist to the clipboard
//-----------------------------------------------------------------------------
void MBeatBoxGrid::copy()
{
	MBeatBoxNoteList* ptNoteList = getNoteListFromSelection();
	if( ptNoteList )
		MClipBoard::getInstance()->setContent( ptNoteList );
};

MBeatBoxNoteList* MBeatBoxGrid::getNoteListFromSelection()
{
	MBeatBoxNoteList* ptNoteList = 0;

	int min = getMaxCols();
	int max = 0;
	unsigned int count = ivPtSelectionManager->getSelectionCount();
	if( count > 0 )
	{
		for( unsigned int i=0;i<count;i++ )
		{ // search all selected notes, minimal startsAt and maximal stopsAt
			MMessageNoteControl* ptControl = (MMessageNoteControl*) ivPtSelectionManager->getSelectionAt( i );
			MDefaultNote* ptNote = ptControl->GetNote();
			if( ptNote->getIndex() < min )
				min = ptNote->getIndex();
			if( ptNote->getStopsAt() > max )
				max = ptNote->getStopsAt();
		}

		ptNoteList = (MBeatBoxNoteList*) ivPtNoteList->createNewInstance();
		if( max % 2 == 0 )
			max += 1;
		ptNoteList->setMaxLength( max + 1 );
		unsigned int newStartAt;
		for( i=0;i<count;i++ )
		{
			MMessageNoteControl* ptControl = (MMessageNoteControl*) ivPtSelectionManager->getSelectionAt( i );
			MBeatBoxMessage* vmsg = (MBeatBoxMessage*)(ptControl->getMsg());
			MDefaultNote* ptNote = (MDefaultNote*) ptControl->GetNote();
			newStartAt = ptNote->getIndex() - min;
			ptNoteList->postMessage( vmsg->getCopy(), newStartAt );
		}
	}

	return ptNoteList;
};

//-----------------------------------------------------------------------------
// + PASTE
//	Inserts the notes in the clipboard at the specified index
//-----------------------------------------------------------------------------
void MBeatBoxGrid::paste( int index )
{
	MClipBoardContext* content = MClipBoard::getInstance()->getContent();

	if( content )
	{
		if( content->getUserObject()->getTypeId() == ivPtNoteList->getTypeId() )
		{
			MBeatBoxNoteList* ptNoteList = (MBeatBoxNoteList*) content->getUserObject();
			MDefaultMessageNote* ptNote = (MDefaultMessageNote*) ptNoteList->getFirstItem();
			int maxId = getMaxRows();
			while( ptNote != 0 )
			{
				int msgCount = ptNote->getMessageCount();
				for( int i=0;i<msgCount;i++ )
				{
					MBeatBoxMessage* msg = (MBeatBoxMessage*) ptNote->getMessageAt( i );
					if( msg->getId() < maxId )
					{
						if( ivPtNoteList->canPostMessageAt( ptNote->getIndex() + index, msg->getId() ) )
						{
							int insertPos = ptNote->getIndex() + index;
							MMessage* ptNewMessage = msg->getCopy();
							ivPtNoteList->postMessage( ptNewMessage, insertPos );
							createControl(
								(MDefaultMessageNote*)ivPtNoteList->getItemAt( insertPos ),
								ptNewMessage );
						}
					}
				}
				ptNote = (MDefaultMessageNote*) ptNote->getNext();
			}
			repaint();
			//repaint();
		}
		else
		{
			MMessageBox::showError( "INVALID_PASTE_IN_BEATBOXGRID", String( content->getUserObject()->getUserTypeName() ) );
		}

	}
	else
	{
		TRACE( " NO ClipBoardContextFound!!!>\n" );
		MMessageBox::showError( "CLIPBOARD_EMPTY" );
	}
};

//-----------------------------------------------------------------------------
// + DELETE SELECTED
// Deletes the selected notes
//-----------------------------------------------------------------------------
void MBeatBoxGrid::deleteSelected()
{
 	while( ivPtSelectionManager->getSelectionCount() > 0 )
	{
		MMessageNoteControl *control = (MMessageNoteControl*) ivPtSelectionManager->getSelectionAt( 0 );
		ivPtSelectionManager->removeFromSelection( (unsigned int) 0 );

		// remove from deleted MArrangeBars from ivArArrangeBars
		for( unsigned int j=0;j<ivControls.size();j++ )
			if( ivControls[ j ] == control )
				ivControls.erase( ivControls.begin() + j );

		// delete message from notelist
		ivPtNoteList->deleteMessageForId(	control->GetNote()->getIndex(),
											control->getMsg()->getId() );

		// delete control
		SAFE_DELETE( control );
	}

	repaint();
	fireContentChanged();
};

int MBeatBoxGrid::getRow( MDCControl* ptControl )
{
	ASSERT( ptControl );
	return ((MMessageNoteControl*)ptControl)->getMsg()->getId();
}

int MBeatBoxGrid::getCol( MDCControl* ptControl )
{
	ASSERT( ptControl );
	return ((MMessageNoteControl*)ptControl)->GetNote()->getIndex();
}

int MBeatBoxGrid::getColSpan( MDCControl* _ptControl )
{
	MMessageNoteControl* ptControl = (MMessageNoteControl*) _ptControl;
	return ptControl->GetNote()->getDuration();
};

int MBeatBoxGrid::getMaxRows()
{
	return ivPtBeatBox->getWavePlayerCount();
}

int MBeatBoxGrid::getMaxCols()
{
	return ivPtNoteList->getMaxLength();
}

//-----------------------------------------------------------------------------
// + SET NOTE LIST
//-----------------------------------------------------------------------------
void MBeatBoxGrid::setNoteList( MDefaultMessageNoteList *ptNoteList )
{
	ivPtNoteList = ptNoteList;
};

//-----------------------------------------------------------------------------
// + GET NOTE LIST
//-----------------------------------------------------------------------------
MDefaultMessageNoteList* MBeatBoxGrid::getNoteList()
{
	return ivPtNoteList;
};

//-----------------------------------------------------------------------------
// # CREATE NOTE CONTROL
// Creates  some MMessageNoteControls from a given note and places them in
//	the grid
//-----------------------------------------------------------------------------
void MBeatBoxGrid::createControls( MDefaultMessageNote *ptNote )
{
	TRACE( "<MBeatBoxGrid::CreateNoteControl()>\n" );

	int messagecount = ptNote->getMessageCount();
	for( int i=0;i<messagecount;i++ )
		createControl( ptNote, ptNote->getMessageAt( i ) );
};

//-----------------------------------------------------------------------------
// # CREATE NOTE CONTROL
// Creates  some MMessageNoteControls from a given note and places them in
//	the grid
//-----------------------------------------------------------------------------
void MBeatBoxGrid::createControl( MDefaultMessageNote* ptNote, MMessage* msg )
{
	MMessageNoteControl *ptControl = new MMessageNoteControl( ptNote, msg );
	ivControls.push_back( ptControl );
	layoutControl( ptControl );
};

//-----------------------------------------------------------------------------
// # DELETE NOTE
// Deletes a MMessageNoteControls from a given note and removes them from
// the notelist
//-----------------------------------------------------------------------------
void MBeatBoxGrid::deleteControl( MDCControl* _ptControl )
{
	MMessageNoteControl* ptControl = (MMessageNoteControl*) _ptControl;
	int index = GetIndexOfControl( ptControl );
	if( index > -1 )
		ivControls.erase( ivControls.begin() + index );

	ivPtNoteList->deleteMessageForId(	ptControl->GetNote()->getIndex(),
										ptControl->getMsg()->getId() );

	SAFE_DELETE( ptControl );
};

void MBeatBoxGrid::onMouseDown( MMouseEvent* anEvent )
{
	if( anEvent->getButton() == MMouseEvent::BUTTON2 )
	{
		// context menu...
	}
	else
	{
		MDCGrid::onMouseDown( anEvent );

		if( ivMouseCaptured )
		{
			onMouseUp( anEvent );
			return;
		}

		MDCControl *ptControl = getControlAt( anEvent->getPoint() );
		if( ptControl == 0 )
		{ // No control found at the clicked position
			TRACE( "NO CONTROL FOUND>\n" );
			if( getEditMode() == MBeatBoxGrid::DRAW_MODE )
			{ // DRAW_MODE -> try to create a new note and notecontrol an the given point
				if( (ptControl = createControl( anEvent->getPoint() )) != 0 )
				{
					repaint();
					fireContentChanged();
				}
			}
			else if( getEditMode() == MBeatBoxGrid::SELECT_MODE )
			{
				ivPtSelectionManager->resetSelection();
				repaint();
			}
		}
		else
		{ // A existing control was found at the clicked position
			TRACE( "A CONTROL WAS FOUND>\n" );
			ivCurrentEditOffset = anEvent->getPoint().sub( ptControl->getPosition().getPoint() );
			if( getEditMode() == MBeatBoxGrid::DRAW_MODE )
			{ // DRAW_MODE -> START redrawing note-length
				deleteControl( ptControl );
				//repaint();
				repaint();
				fireContentChanged();
			}
			else if( getEditMode() == MBeatBoxGrid::SELECT_MODE )
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
						ivPtSelectionManager->singleSelection( ptControl );
					repaint();
				}

				// Start DRAGGING
				startControlMoving( ptControl );
			}
		}

		// Capture mouse
		if( ! ivMouseCaptured )
		{
			ivMouseCaptured = true;
		}
	}
}

void MBeatBoxGrid::onMouseUp( MMouseEvent* anEvent )
{
	if( anEvent->getButton() == MMouseEvent::BUTTON2 )
	{
		/*while( ivPtContextMenu->GetMenuItemCount() )
			ivPtContextMenu->RemoveMenu( 0, MF_BYPOSITION );

		// context menu...
		MPoint p = anEvent->getPoint();
		int row = MDCGrid::getRow( p );
		int col = MDCGrid::getCol( p );
		if(	row >= 0 &&
			row < getMaxRows() &&
			col >= 0 &&
			col < getMaxCols() )
		{
			if( ivPtContextMenu->AppendMenu(
				MF_POPUP,
				(UINT_PTR)ivCreatorsMenu->m_hMenu,
				"patterns" ) != TRUE )
			{
				MMessageBox::showError( "AppendMenu creators menu failed" );
			}
			ivCurPoint = MPoint( col, row );
		}

		if( ivPtContextMenu->AppendMenu(
			MF_POPUP,
			(UINT_PTR)ivPtDynamicsMenu->m_hMenu,
			"dynamics" ) != TRUE )
		{
			MMessageBox::showError( "AppendMenu dynamics menu failed" );
		}

		RECT wRect;
		GetWindowRect( ivHwnd, &wRect );

		if( ivPtContextMenu->TrackPopupMenu(
				TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
				wRect.left + anEvent->getX(),
				wRect.top + anEvent->getY(),
				this
				) != TRUE )
		{
			MMessageBox::showError( "failed tracking popup" );
		}*/
	}
	else
	{
		MDCGrid::onMouseUp( anEvent );
		// If mouse was captured -> release capture
		if( ivMouseCaptured )
		{
			if( getEditMode() == MBeatBoxGrid::DRAW_MODE )
			{
				ivNoteEditMode = false;
				ivPtEditControl = 0;
			}
			else if( getEditMode() == MBeatBoxGrid::SELECT_MODE )
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

			// Update graphics
			repaint();
		}
	}
}

void MBeatBoxGrid::onMouseMove( MMouseEvent* anEvent )
{
	MDCGrid::onMouseMove( anEvent );
	if( ivMouseCaptured )
	{
		if( getEditMode() == MBeatBoxGrid::SELECT_MODE )
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
	}
}

//-----------------------------------------------------------------------------
// # ON SET CURSOR
// Called by the framework to set the current used cursour
//-----------------------------------------------------------------------------
bool MBeatBoxGrid::onSetCursor()
{
	HCURSOR c = 0;
	/*if( getEditMode() == MBeatBoxGrid::SELECT_MODE )
		c = LoadCursor( NULL, IDC_ARROW );
	else if( getEditMode() == MBeatBoxGrid::DRAW_MODE )
		c = LoadCursor( MApp::getInstance()->getHInstance(), MAKEINTRESOURCE( IDC_PEN ) );

	if( c != 0 )
		SetCursor( c );
	else
		MLogger::logError( "MBeatBoxGrid::onSetCursor: LoadCursor failed" );*/
	return c != 0;
}

//-----------------------------------------------------------------------------
// # CREATE Message
// Tries to create a message at the given point (true if succeeeded)
//-----------------------------------------------------------------------------
MDCControl *MBeatBoxGrid::createControl( MPoint point )
{
	MDCControl* ptCreatedControl = 0;

	int row = MDCGrid::getRow( point );
	int col = MDCGrid::getCol( point );
	if(	row >= 0 &&
		row < getMaxRows() &&
		col >= 0 &&
		col < getMaxCols() )
	{
		if( ivPtNoteList->canPostMessageAt( col, row ) )
		{
			MBeatBoxMessage *ptMessage = new MBeatBoxMessage( row );
			ivPtNoteList->postMessage( ptMessage, col );
			ptCreatedControl = new MMessageNoteControl(
						(MDefaultMessageNote*)ivPtNoteList->getItemAt( col ),
						ptMessage );
			ivControls.push_back( ptCreatedControl );
			layoutControl( ptCreatedControl );
		}
	}

	return ptCreatedControl;
};

//-----------------------------------------------------------------------------
// # STOP NOTE MOVING
// stops the dragNDrop move of a note, doesnt repaint
//-----------------------------------------------------------------------------
void MBeatBoxGrid::stopControlMoving( MPoint point )
{
	MPoint div = ivPointMouseUp.sub( ivPointMouseDown );
	int oldCol = MDCGrid::getCol( ivPointMouseDown );
	int newCol = MDCGrid::getCol( ivPointMouseUp );
	int oldRow = MDCGrid::getRow( ivPointMouseDown );
	int newRow = MDCGrid::getRow( ivPointMouseUp );
	int colDiv = newCol - oldCol;
	int rowDiv = newRow - oldRow;

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

	MMNCComparator::SortType sortType = (MMNCComparator::SortType) st;

	MMNCComparator* ptComparator = new MMNCComparator( sortType );
	MDCList* ptDcList = new MDCList( ptComparator, ivPtSelectionManager );

	unsigned int count = ptDcList->getControlCount();
	for( unsigned int i=0;i<count;i++ )
	{
		MMessageNoteControl* ptControl = (MMessageNoteControl*) (ptDcList->getControl( i ));
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

void MBeatBoxGrid::moveControl( MDCControl* _ptControl, int rowDiv, int colDiv )
{
	MMessageNoteControl* ptControl = (MMessageNoteControl*) _ptControl;
	int col = getCol( ptControl );
	int row = getRow( ptControl );
	int newCol = col + colDiv;
	int newRow = row + rowDiv;
		
	if( ( newRow >= 0 ) &&
		( newRow < getMaxRows() ) &&
		( newCol >= 0 ) &&
		( newCol < getMaxCols() ) &&
		ivPtNoteList->canPostMessageAt( newCol, newRow ) )
	{
		ivPtNoteList->removeMessageForId( col, row );
		ptControl->getMsg()->setId( newRow ); 
		ptControl->SetNote( ivPtNoteList->postMessage( ptControl->getMsg(), newCol ) );
	}
};

void MBeatBoxGrid::onCommand( unsigned int id )
{
	if( id >= ID_DYN_BEG && id <= ID_DYN_END )
	{
		ASSERT( id >= ID_DYN_BEG && id <= ID_DYN_END );
		unsigned int index = (unsigned int) (id - ID_DYN_BEG);
		if( ivPtSelectionManager->getSelectionCount() )
		{
			unsigned int sc = ivPtSelectionManager->getSelectionCount();
			for( unsigned int i=0;i<sc;i++ )
			{
				MDCControl* pControl = ivPtSelectionManager->getSelectionAt( i );
				unsigned int ii = ((MDefaultMessageNote*)((MMessageNoteControl*)pControl)->GetNote())->getStartsAt();

				((MBeatBoxMessage*)((MMessageNoteControl*)pControl)->getMsg())->setVolume(
					(FP) ((MDynamics*)gvDynamics->get( index ))->get( ii )
					);
			}
		}
		else
			((MDynamics*)gvDynamics->get(index ))->apply( ivPtNoteList );
		repaint();
		fireContentChanged();
	}
	else if( id >= ID_PAT_BEG && id <= ID_PAT_END )
	{
		ASSERT( id >= ID_PAT_BEG && id <= ID_PAT_END );
		unsigned int index = (unsigned int) (id - ID_PAT_BEG);

		MDynamics* pPat = ((MDynamics*)gvPatterns->get( index ));

		unsigned int maxLen = ivPtNoteList->getMaxLength();
		unsigned int cur = ivCurPoint.getX();
		unsigned int run = 0;
		while( cur < maxLen )
		{
			double value = pPat->get( run );
			if( value != 0.0 )
				if( ((MBeatBoxNoteList*)ivPtNoteList)->canPostMessageAt( cur, ivCurPoint.getY() ) )
				{
					MBeatBoxMessage* msg = new MBeatBoxMessage( ivCurPoint.getY(), (FP) value );
					MDefaultMessageNote* pNote = ((MBeatBoxNoteList*)ivPtNoteList)->postMessage( msg, cur );
					createControl( pNote, msg );
				}
			cur++;
			run++;
		}
		this->repaint();
		fireContentChanged();
		
		ivCurPoint;
	}
}