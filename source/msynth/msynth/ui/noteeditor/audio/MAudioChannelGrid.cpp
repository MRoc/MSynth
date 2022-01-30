#include "MAudioChannelGrid.h"
#include <gui/MApp.h>

#include <gui/dialog/MMessageBox.h>
#include <gui/MGuiutils.h>
#include <gui/MGuiutils.h>
#include "../../clipboard/MClipBoard.h"

MAudioChannelGrid::MAudioChannelGrid( MSize* ptPartSize ) :
	MDCGrid( ptPartSize ),
	ivPtBpmControl( 0 ),
	ivPtBpmListener( 0 )
{
	ivPtBpmListener = new BpmListener( this );
}

MAudioChannelGrid::~MAudioChannelGrid()
{
	setBpmControl( 0 );
	delete ivPtBpmListener;
}

void MAudioChannelGrid::setBpmControl( IBpmControl* ptControl )
{
	if( this->ivPtBpmControl )
		this->ivPtBpmControl->removeBpmListener( this->ivPtBpmListener );
	this->ivPtBpmControl = ptControl;
	if( this->ivPtBpmControl )
		this->ivPtBpmControl->addBpmListener( this->ivPtBpmListener );
}

IBpmControl* MAudioChannelGrid::getBpmControl()
{
	return this->ivPtBpmControl;
}

FP MAudioChannelGrid::getPeaksPerPixel()
{
	FP back = 0.0f;
	FP peaksPerSecond = 44.1f;
	FP stepsPerBeat = 4;
	FP bpm = (FP) ivPtBpmControl->getBpm();
	FP pixelsPerStep = (FP)this->ivPtPartSize->getWidth();
	
	back = ( peaksPerSecond / ( ( bpm * stepsPerBeat) / 60 ) ) / pixelsPerStep;

	return back;
}

void MAudioChannelGrid::paint( IGraphics* ptGraphics, const MRect &rect )
{
	unsigned int width = rect.getWidth();
	unsigned int height = rect.getHeight();

	// scroll position
	MPoint target = getScrollPos();
	int x = target.getX();
	int y = target.getY();

	// draw grid
	MGuiUtils::paintGrid(
		ptGraphics,
		rect,
		target,
		*ivPtPartSize,
		1,
		ivPtTriggers->getMaxLength(),
		MColorMap::BK_COLOR4 );
	
	// draw noteControls
	unsigned int count = ivControls.size();
	for( unsigned int i=0;i<count;i++ )
		((MDCControl*) ivControls[i] )->paintControl( ptGraphics );
	if( ivPtEditControl != 0 )
		ivPtEditControl->paintControl( ptGraphics );

	// draw selection-rect
	if( getEditMode() == MDCGrid::SELECT_MODE &&
		ivMouseCaptured && ! ivMoveMode )
	{
		MRect rect( ivPointMouseDown, ivPointMouseUp );
		ptGraphics->drawRect( rect, MColorMap::BK_COLOR1 );
	}
}

void MAudioChannelGrid::updateFromModel()
{
	TRACE( "<MAudioChannelGrid::updateFromModel/>\n" );

	deleteControls();

	MAudioTrigger* ptNote = (MAudioTrigger*) ivPtTriggers->getFirstItem();
	while( ptNote != 0 )
	{
		createControl( ptNote );
		ptNote = (MAudioTrigger*) ptNote->getNext();
	}

	fireContentChanged();
	repaint();
	//repaint();
}

void MAudioChannelGrid::copy()
{
	unsigned int count;
	std::vector<MDefaultNote*> selectedNotes;
	unsigned int min = ivPtTriggers->getMaxLength();
	unsigned int max = 0;
	unsigned int newStartAt;
	MDefaultNote* ptNote;

	// search all selected notes, minimal startsAt and maximal stopsAt
	count = ivControls.size();
	for( unsigned int i=0;i<count;i++ )
		if( ((MDCControl*)ivControls[ i ])->getSelected() )
		{
			ptNote = ((MAudioTriggerControl*)ivControls[ i ])->getTrigger();
			selectedNotes.push_back( ptNote );
			if( ptNote->getIndex() < (int)min )
				min = ptNote->getIndex();
			if( ptNote->getStopsAt() > (int)max )
				max = ptNote->getStopsAt();
		}

	if( (count=selectedNotes.size()) )
	{
		MAudioTriggerList* ptTriggerList = (MAudioTriggerList*) ivPtTriggers->createNewInstance();
		if( max%2 == 0 )
			max += 1;
		ptTriggerList->setMaxLength( max + 1 );
		for( unsigned int i=0;i<count;i++ )
		{
			ptNote = (MDefaultNote*) selectedNotes[ i ];
			MDefaultNote* ptNewNote = ptNote->getCopy();
			newStartAt = ptNewNote->getIndex() - min;
			ptNewNote->setIndex( newStartAt );
			ptTriggerList->MList::insertItem( ptNewNote );
		}

		MClipBoard::getInstance()->setContent( ptTriggerList );
	}
}

void MAudioChannelGrid::paste( int index )
{
	MClipBoardContext* content = MClipBoard::getInstance()->getContent();

	if( content )
	{
		if( content->getUserObject()->getTypeId() == ivPtTriggers->getTypeId() )
		{
			TRACE( "Clipboard type: " + String( content->getUserObject()->getUserTypeName() ) ); 
			int count = 0;
			MAudioTriggerList* ptNoteList = (MAudioTriggerList*) content->getUserObject();
			MAudioTrigger* ptNote = (MAudioTrigger*) ptNoteList->getFirstItem();
			while( ptNote != 0 )
			{
				if( ivPtTriggers->isFree( ptNote->getIndex() + index ) )
				{
					MAudioTrigger* ptNewNote = (MAudioTrigger*) ptNote->getCopy();
					int insertPosition = ptNewNote->getIndex() + index;
					ptNewNote->setIndex( insertPosition );
					ivPtTriggers->MList::insertItem( ptNewNote );
					this->createControl( ptNewNote );
					count++;
				}
				ptNote = (MAudioTrigger*) ptNote->getNext();
			}
			repaint();
			//repaint();

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

void MAudioChannelGrid::deleteSelected()
{
	while( ivPtSelectionManager->getSelectionCount() > 0 )
	{
		MAudioTriggerControl *control = (MAudioTriggerControl*) ivPtSelectionManager->getSelectionAt( 0 );
		ivPtSelectionManager->removeFromSelection( (unsigned int) 0 );

		// remove from deleted MArrangeBars from ivArArrangeBars
		for( unsigned int j=0;j<ivControls.size();j++ )
			if( ivControls[ j ] == control )
				ivControls.erase( ivControls.begin() + j );

		ivPtTriggers->deleteItem( control->getTrigger() );
		delete control;
	}

	repaint();
	fireContentChanged();
}

int MAudioChannelGrid::getMaxRows()
{
	return 1;
}

int MAudioChannelGrid::getMaxCols()
{
	return ivPtTriggers->getMaxLength();
}

int MAudioChannelGrid::getRow( MDCControl* ptControl )
{
	return 0;
}

int MAudioChannelGrid::getCol( MDCControl* ptControl )
{
	return ((MAudioTriggerControl*)ptControl)->getTrigger()->getIndex();
}

int MAudioChannelGrid::getColSpan( MDCControl* ptControl )
{
	return ((MAudioTriggerControl*)ptControl)->getTrigger()->getDuration();
}

void MAudioChannelGrid::stopControlMoving( MPoint point )
{
	MPoint div = ivPointMouseUp.sub( ivPointMouseDown );
	int oldCol = MDCGrid::getCol( ivPointMouseDown );
	int newCol = MDCGrid::getCol( ivPointMouseUp );
	int colDiff = newCol - oldCol;

	// b1  b0
	// 0   0   STD
	// 1   0   REV
	// colDiv > 0 (rightMove) -> b0 = 1 (reverse)
	int st = 0;
	if( colDiff > 0 ) 
		st |= 2;

	TRACE(
		"<MAudioChannelGrid::stopControlMoving oldcol=\"%i\" newcol=\"%i\" coldiff=\"%i\" sorttype=\"%s\"/>\n",
		oldCol,
		newCol,
		colDiff,
		(((unsigned int)st)&2) ? "reverse" : "standard" );

	MDefaultDCComparator::SortType sortType = (MDefaultDCComparator::SortType) st;

	MATCComparator* ptComparator = new MATCComparator( sortType );
	MDCList* ptDcList = new MDCList( ptComparator, ivPtSelectionManager );

	unsigned int count = ptDcList->getControlCount();
	for( unsigned int i=0;i<count;i++ )
	{
		MAudioTriggerControl* ptControl = (MAudioTriggerControl*) (ptDcList->getControl( i ));

		TRACE(
			"<MAudioChannelGrid::stopControlMoving controlno=\"%i\" start=\"%i\"/>\n",
			i,
			ptControl->getTrigger()->getIndex() );

		moveControl(
			ptControl,
			colDiff );
		layoutControl( ptControl );
	}

	ivMoveMode = false;
	ivPtEditControl = 0;

	fireContentChanged();

	delete ptDcList;
	delete ptComparator;
}

void MAudioChannelGrid::moveControl( MDCControl* _ptControl, int colDiv )
{
 	MAudioTriggerControl* ptControl = (MAudioTriggerControl*) _ptControl;
	MAudioTrigger *ptTrigger = (MAudioTrigger*) ptControl->getTrigger();
	int col = getCol( ptControl );
	int newCol = col + colDiv;
		
	if( ivPtTriggers->canInsertAt( ptTrigger, newCol, ptTrigger->getDuration() ) )
	{
		TRACE(
			"<MAudioChannelGrid::moveControl oldcol=\"%i\" newcol=\"%i\" coldiff=\"%i\"/>\n",
			col,
			newCol,
			colDiv );

		ivPtTriggers->unlink( ptTrigger );
		ptTrigger->setIndex( newCol );
		ivPtTriggers->MList::insertItem( ptTrigger );
	}
};

MDCControl* MAudioChannelGrid::createControl( MPoint point )
{
	MDCControl* ptControl = 0;

	MAudioFile* ptFile = (MAudioFile*) ivPtAudioChannel->getFileList()->getSelectedObject();
	if( ptFile )
	{
		int col = MDCGrid::getCol( point );
		if( col >= 0 &&
			col < (int)ivPtTriggers->getMaxLength() &&
			ivPtTriggers->isFree( col ) )
		{
			MAudioTrigger* ptTrigger = new MAudioTrigger();
			ptTrigger->setFile( ptFile );
			ptTrigger->setDuration( 1 );
			ptTrigger->setIndex( col );
			ivPtTriggers->insertItem( ptTrigger );
			ptControl = (MAudioTriggerControl*) createControl( ptTrigger );
		}
	}

	return ptControl;
}

MDCControl* MAudioChannelGrid::createControl( MAudioTrigger* ptTrigger )
{
	ASSERT( ptTrigger );

	MAudioTriggerControl* ptControl = new MAudioTriggerControl( ptTrigger );
	ptControl->setStepWidthInPixel( ivPtPartSize->getWidth() );
	ptControl->setPeaksPerPixel( this->getPeaksPerPixel() );
	layoutControl( ptControl );
	ivControls.push_back( ptControl );

	return ptControl;
}

void MAudioChannelGrid::deleteControl( MDCControl* _ptControl )
{
	MAudioTriggerControl* ptControl = (MAudioTriggerControl*) _ptControl;
	int index = GetIndexOfControl( ptControl );
	if( index > -1 )
		ivControls.erase( ivControls.begin() + index );

	ivPtTriggers->deleteItem( ptControl->getTrigger() );
	SAFE_DELETE( ptControl );
}

void MAudioChannelGrid::setData( MAudioChannel* ptChannel, MAudioTriggerList* ptTriggers )
{
	this->ivPtAudioChannel = ptChannel;
	this->ivPtTriggers = ptTriggers;
}

bool MAudioChannelGrid::onSetCursor()
{
	HCURSOR cursor = 0;

	/*if( getEditMode() == MDCGrid::SELECT_MODE )
		cursor = LoadCursor( NULL, IDC_ARROW );
	else if( getEditMode() == MDCGrid::DRAW_MODE )
		cursor = LoadCursor( MApp::getInstance()->getHInstance(), MAKEINTRESOURCE( IDC_PEN ) );

	if( cursor == 0 )
		MLogger::logError( "MVFloatGui::onSetCursor: LoadCursor failed" );
	else
		SetCursor( cursor );*/

	return cursor != 0;
}

void MAudioChannelGrid::onMouseDown( MMouseEvent* anEvent )
{
	if( ivMouseCaptured )
	{
		onMouseUp( anEvent );
		return;
	}

	MDCGrid::onMouseDown( anEvent );

	MAudioTriggerControl* ptControl = (MAudioTriggerControl*) getControlAt( anEvent->getPoint() );
	if( ptControl == 0 )
	{ // No control found at the clicked position
		TRACE( "<msg>no control found</msg>\n" );
		if( getEditMode() == DRAW_MODE )
		{ // DRAW_MODE -> try to create a new note and notecontrol an the given point
			MDCControl* ptControl = 0;
			if( (ptControl = createControl( anEvent->getPoint() )) != 0 )
			{
				startControlEditing( ptControl );
				repaint();
				fireContentChanged();
			}
		}
	}
	else
	{ // A existing control was found at the clicked position
		ivCurrentEditOffset = anEvent->getPoint().sub( ptControl->getPosition().getPoint() );
		TRACE( "<msg>a control was found</msg>\n" );
		if( getEditMode() == DRAW_MODE )
		{ // DRAW_MODE -> START redrawing note-length
			startControlEditing( ptControl );
		}
		else if( getEditMode() == SELECT_MODE )
		{ // SELECT_MODE -> Select ptControl
			if( anEvent->getShiftPressed() )
			{ // MULTIPLE SELECITON
				if( ! ivPtSelectionManager->isInSelection( ptControl ) )
				{
					ivPtSelectionManager->addToSelection( ptControl );
					repaint();
					//repaint();
				}
			}
			else
			{ // SINGLE SELECTION
				if( ! ivPtSelectionManager->isInSelection( ptControl ) )
				{
					ivPtSelectionManager->singleSelection( ptControl );
					repaint();
					//repaint();
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

void MAudioChannelGrid::onMouseUp( MMouseEvent* anEvent )
{
	MDCGrid::onMouseUp( anEvent );

	// If mouse was captured -> release capture
	if( ivMouseCaptured )
	{
		if( getEditMode() == DRAW_MODE )
		{
			// If a note was resized -> turn of resize-mode
			if( ivPtEditControl )
				ivPtEditControl = 0;
		}
		else if( getEditMode() == SELECT_MODE )
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
		//repaint();
	}
}

void MAudioChannelGrid::onMouseMove( MMouseEvent* anEvent )
{
	if( ivMouseCaptured )
	{
		if( getEditMode() == SELECT_MODE )
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
				
				repaint();
				//repaint();
			}
			else
			{ // SELECTON
				ivPointMouseUp = anEvent->getPoint();
				updateSelection( MRect( ivPointMouseDown, ivPointMouseUp ) );
				repaint();
				//repaint();
			}
		}
		else if(
			getEditMode() == DRAW_MODE &&
			ivPtEditControl &&
			anEvent->getPoint().getX() > ivPointMouseDown.getX() &&
			updateEditing( anEvent->getPoint() ) )
		{
			repaint();
			//repaint();
		}
	}
}

void MAudioChannelGrid::startControlEditing( MDCControl* ptControl )
{
	ASSERT( getEditMode() == DRAW_MODE );

	ivPtEditControl = ptControl;
	ivPointMouseDown = ptControl->getPosition().getPoint();
	ivMouseCaptured = true;
}

bool MAudioChannelGrid::updateEditing( MPoint point )
{
	ASSERT( ivPtEditControl != 0 );

	bool back = false;

	MAudioTrigger* editedTrigger = (MAudioTrigger*) ((MAudioTriggerControl*)ivPtEditControl)->getTrigger();
	ASSERT( editedTrigger );

	int targetPos = MDCGrid::getCol( point );

	// clip from bars length...
	if( targetPos < 0 )
		targetPos = 0;
	else if( targetPos >= (int) ivPtTriggers->getMaxLength() )
		targetPos = ivPtTriggers->getMaxLength() - 1;

	// clip from next trigger if found
	MAudioTrigger* nextTrigger = (MAudioTrigger*) editedTrigger->getNext();
	if( nextTrigger && targetPos >= (int) nextTrigger->getIndex() )
		targetPos = nextTrigger->getIndex() - 1;

	// test and start resizing
	int noteStart = editedTrigger->getIndex();
	if( targetPos >= noteStart )
	{
		int targetLength = targetPos - noteStart + 1;
		int maxNoteLength = ivPtTriggers->getMaxLength();
		if( targetLength > maxNoteLength )
		{
			targetLength = maxNoteLength;
		}
		editedTrigger->setDuration( targetLength );
		layoutControl( ivPtEditControl );
		back = true;
	}

	return back;
}

void MAudioChannelGrid::doLayout()
{
	unsigned int count = ivControls.size();
	for( unsigned int i=0;i<count;i++ )
		layoutControl( ivControls[ i ] );

	if( count > 0 )
		repaint();
}

void MAudioChannelGrid::updateControlPeaksPerPixel()
{
	FP ppp = this->getPeaksPerPixel();
	unsigned int count = ivControls.size();
	for( unsigned int i=0;i<count;i++ )
		((MAudioTriggerControl*) ivControls[ i ] )->setPeaksPerPixel( ppp );
}

MAudioChannelGrid::BpmListener::BpmListener( MAudioChannelGrid* ptGrid ) :
	ivPtGrid( ptGrid )
{
}

MAudioChannelGrid::BpmListener::~BpmListener()
{
}

void MAudioChannelGrid::BpmListener::bpmChanged( unsigned int bpm )
{
	ivPtGrid->updateControlPeaksPerPixel();
	ivPtGrid->repaint();
	//ivPtGrid->repaint();
}