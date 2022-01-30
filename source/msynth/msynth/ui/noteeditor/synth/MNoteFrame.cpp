/*

	MNoteEditor (C)2001 MRoc hifiShock

*/
#include "MNoteFrame.h"
#include <gui/framewnd/MRootPane.h>

#define DEFAULT_PART_SIZE_X		20
#define DEFAULT_PART_SIZE_Y		12

#define ID_NOTE_PIANO			104
#define ID_NOTE_GRID			105

#include <gui/MApp.h>

/** constructor */
MNoteFrame::MNoteFrame() : MAbstractNoteFrame(),
	ivPtPiano(0),
	ivPtVolInspector( 0 ),
	ivPtFEInspector( 0 )
{
	ivPtPartSize = new MSize( DEFAULT_PART_SIZE_X, DEFAULT_PART_SIZE_Y );
}

/** destructor */
MNoteFrame::~MNoteFrame()
{
	MClipBoard::getInstance()->unsetContext( this );

	if( ivPtInspector ){
		if( ivPtVolInspector ){
			ivPtVolInspector->setBar( 0 );
			ivPtInspector->removeInspector( ivPtVolInspector );
			delete ivPtVolInspector;
			ivPtVolInspector = 0;
		}
		if( ivPtFEInspector ){
			ivPtFEInspector->setBar( 0 );
			ivPtInspector->removeInspector( ivPtFEInspector );
			delete ivPtFEInspector;
			ivPtFEInspector = 0;
		}
	}
	setBar( 0 );
	setInstrument( 0 );

	SAFE_DELETE( ivPtPartSize );
}

/** creates the noteframe */ 
bool MNoteFrame::create( MRect rect, MTopWnd *parentWnd )
{
	ASSERT( ivPtPartSize != 0 );
	ASSERT( ivPtBar != 0 );
	ASSERT( ivPtInstrument != 0 );

	MDefaultSynthNoteList* ptNoteList = (MDefaultSynthNoteList*) ivPtBar->getNoteList();

	ivPtPiano = new MNotePiano( ((Synth*)ivPtInstrument) );
	addChild( ivPtPiano );

	ivPtGrid = new MNoteGrid( ivPtPartSize,	ptNoteList );
	addChild( ivPtGrid );

	ivDividerPosY1 = 22;
	ivDividerPosY2 = 80;
	ivDividerPosX1 = 80;//ivPtPiano->getWidth();

	bool back =	MAbstractNoteFrame::create( rect, MApp::getInstance()->getMainWnd() );
	if( back )
	{
		if( ivPtInspector )
		{
			ivPtVolInspector = new MVolumeInspector();
			ivPtFEInspector = new MFloatEventInspector();
			ivPtInspector->addInspector( ivPtVolInspector );
			ivPtInspector->addInspector( ivPtFEInspector );
			ivPtInspectorCombo->setBar( ivPtBar );
		}
		ivPtScrollbarController->addVerticalScrollWnd( ivPtPiano );
		setText( "KeyboardGrid" );
		ivButtonControl.setCurrentButton( ivPtButtonSelect );
		doLayout();
	}
	return back;
}

void MNoteFrame::setBar( MBar* ptBar )
{
	MAbstractNoteFrame::setBar( ptBar );
	if( ivPtBar )
		setProperties( ptBar->getProperties() );
	else
		setProperties( 0 );
}

/** update ui from model */
void MNoteFrame::updateFromModel()
{
	TRACE( "<MNoteFrame::updateFromModel>\n" );

	ivPtGrid->updateFromModel();

	ivPtLocationBar->setNotesPerBeat( ivPtBar->getNotesPerBar() / 4 );
	ivPtLocationBar->setBeatCount( 4 * ivPtBar->getLength() );
	ivPtLocationBar->setScrollPos( MPoint( ivPtGrid->getScrollPos().getX(), 0 ) );
	ivPtLocationBar->repaint();

	doLayout();
}

/** layouts all child component */
void MNoteFrame::doLayout()
{
	MSize size = getClientSize();

	if( ivPtButtonSelect )
		ivPtButtonSelect->setRect( MRect( 0, 0, 22, 22 ) );
	if( ivPtButtonDraw )
		ivPtButtonDraw->setRect( MRect( 22, 0, 22, 22 ) );
	if( ivPtScrollbarHorizontal )
		ivPtScrollbarHorizontal->setRect( MRect( ivDividerPosX1, size.getHeight() - 16, size.getWidth()-(16+ivDividerPosX1), 16 ) );
	if( ivPtScrollbarVertical )
		ivPtScrollbarVertical->setRect( MRect( size.getWidth() - 16, ivDividerPosY1, 16, size.getHeight()-(16+ivDividerPosY1) ) );

	if( ivPtGrid )
		ivPtGrid->setRect(
			MRect(
				ivDividerPosX1,
				ivDividerPosY1,
				size.getWidth() - ivDividerPosX1 - 16,
				size.getHeight() - ivDividerPosY1 - ivDividerPosY2 - 16 ) );
	
	if( ivPtPiano )
		ivPtPiano->setRect(
			MRect(
				0,
				ivDividerPosY1,
				ivDividerPosX1,
				size.getHeight() - ivDividerPosY1 - ivDividerPosY2- 16 ) );

	if( ivPtLocationBar )
		ivPtLocationBar->setRect(
			MRect(
				ivDividerPosX1,
				0,
				size.getWidth()-ivDividerPosX1,
				ivDividerPosY1 ) );

	if( ivPtInspector )
		ivPtInspector->setRect(
			MRect(
				ivDividerPosX1,
				size.getHeight() - ivDividerPosY2 - 16,
				size.getWidth()-ivDividerPosX1 - 16,
				ivDividerPosY2 ) );

	if( ivPtInspectorCombo )
		ivPtInspectorCombo->setRect(
			MRect(
				0,
				size.getHeight() - ivDividerPosY2 - 16, 
				ivDividerPosX1,
				20 ) );

	if( ivPtScrollbarController &&
		ivPtGrid )
		ivPtScrollbarController->setScrollSizes(
			ivPtGrid->calculateScrollViewSize(),
			ivPtGrid->getSize() );
}

void MNoteFrame::onCopy()
{
	TRACE( "<MNoteFrame::onCopy()>\n" );
	ivPtGrid->copy();
}

void MNoteFrame::onCut()
{
	TRACE( "<MNoteFrame::onCut()>\n" );
	ivPtGrid->copy();
	ivPtGrid->deleteSelected();
	ivPtInspector->repaint();
}

void MNoteFrame::onPaste()
{
	TRACE( "<MNoteFrame::onPaste()>\n" );
	ivPtGrid->paste( ivPtLocationBar->getLocatorPos() );
	ivPtInspector->repaint();
}

void MNoteFrame::onDelete()
{
	TRACE( "<MNoteFrame::onDelete()>\n" );
	ivPtGrid->deleteSelected();
	ivPtInspector->repaint();
}

MAbstractNoteFrame* MNoteFrame::createInstance()
{
	return new MNoteFrame();
}
