/*

 MBeatBoxNoteEditor (C)2001 MRoc hifiShock

*/
#include "MBeatBoxNoteEditor.h"
#include <gui/MApp.h>

#define DEFAULT_PART_SIZE_X		15
#define DEFAULT_PART_SIZE_Y		20

INIT_RTTI( MBeatBoxNoteEditor, "MBeatBoxNoteEditor" );

MBeatBoxNoteEditor::MBeatBoxNoteEditor()
	: MAbstractNoteFrame(),
	ivPtChannelControl( 0 ),
	ivPtVolInspector( 0 ),
	ivPtFEInspector( 0 )
{
	ivPtPartSize = new MSize( DEFAULT_PART_SIZE_X, DEFAULT_PART_SIZE_Y );
}

MBeatBoxNoteEditor::~MBeatBoxNoteEditor()
{
	MClipBoard::getInstance()->unsetContext( this );

	setBar( 0 );
	setInstrument( 0 );

	ivPtInspector->removeInspector( ivPtVolInspector );
	ivPtInspector->removeInspector( ivPtFEInspector );

	SAFE_DELETE( ivPtPartSize );
	SAFE_DELETE( ivPtVolInspector );
	SAFE_DELETE( ivPtFEInspector );
}

void MBeatBoxNoteEditor::setBar( MBar* ptBar )
{
	MAbstractNoteFrame::setBar( ptBar );

	if( ivPtBar )
		setProperties( ptBar->getProperties() );
	else
		setProperties( 0 );
}

void MBeatBoxNoteEditor::setInstrument( MInstrument* ptInstrument )
{
	if( ivPtInstrument )
		ivPtInstrument->removeInstrumentListener( this );

	MAbstractNoteFrame::setInstrument( ptInstrument );

	if( ivPtInstrument )
		ivPtInstrument->addInstrumentListener( this );
}

bool MBeatBoxNoteEditor::create( MRect rect, MTopWnd *parentWnd )
{
	ASSERT( ivPtInstrument != 0 );
	ASSERT( ivPtPartSize != 0 );
	ASSERT( ivPtBar != 0 );

	MDefaultMessageNoteList* ptNoteList = (MDefaultMessageNoteList*) ivPtBar->getNoteList();

	ivPtChannelControl = new MBeatBoxChannelControlContainer(
			(MBeatBox*)ivPtInstrument,
			ivPtPartSize );

	ivPtGrid = new MBeatBoxGrid(
			(MBeatBox*)ivPtInstrument,
			ivPtPartSize,
			ptNoteList );

	ivPtVolInspector = new MBeatBoxMessageInspector();
	ivPtFEInspector = new MFloatEventInspector();

	ivDividerPosY1 = 22;
	ivDividerPosY2 = 80;
	ivDividerPosX1 = 100;

	bool back = MAbstractNoteFrame::create(	rect, MApp::getInstance()->getMainWnd() );
	if( back )
	{
		ivPtInspector->addInspector( ivPtVolInspector );
		ivPtInspector->addInspector( ivPtFEInspector );

		addChild( ivPtChannelControl );
		addChild( ivPtGrid );
		
		ivPtInspectorCombo->setBar( ivPtBar );

		doLayout();

		setText( "DrumGrid" );

		ivPtScrollbarController->addVerticalScrollWnd( this->ivPtChannelControl );

		this->ivButtonControl.setCurrentButton( this->ivPtButtonSelect );

		onChannelSelected( ((MBeatBox*)ivPtInstrument)->getSelection() );
	}
	return back;
}

void MBeatBoxNoteEditor::updateFromModel()
{
	ASSERT( ivPtGrid );
	ASSERT( ivPtInspector );
	ASSERT( ivPtLocationBar );
	ASSERT( ivPtChannelControl );
	ivPtChannelControl->updateFromModel();
	ivPtChannelControl->repaint();
	ivPtGrid->updateFromModel();
	ivPtGrid->repaint();
	ivPtLocationBar->setNotesPerBeat( ivPtBar->getNotesPerBar() / 4 );
	ivPtLocationBar->setBeatCount( 4 * ivPtBar->getLength() );
	ivPtLocationBar->setScrollPos( MPoint( ivPtGrid->getScrollPos().getX(), 0 ) );
	ivPtLocationBar->repaint();
	ivPtInspector->repaint();
	doLayout();
}

void MBeatBoxNoteEditor::doLayout()
{
	if( ivPtButtonSelect && 
		ivPtButtonDraw &&
		ivPtScrollbarHorizontal	&&
		ivPtScrollbarVertical &&
		ivPtChannelControl &&
		ivPtGrid &&
		ivPtLocationBar &&
		ivPtInspector &&
		ivPtInspectorCombo )
	{
		MSize rect = getClientSize();
		MSize scrollSizeArea = ivPtGrid->calculateScrollViewSize();
		MPoint currentPosition = ivPtGrid->getScrollPos();

		ivPtButtonSelect->setRect( MRect( 0, 0, 22, 22 ) );
		ivPtButtonDraw->setRect( MRect( 22, 0, 22, 22 ) );

		ivPtScrollbarHorizontal->setRect( MRect( ivDividerPosX1, rect.getHeight() - 16, rect.getWidth()-(16+ivDividerPosX1), 16 ) );
		ivPtScrollbarVertical->setRect( MRect( rect.getWidth() - 16, ivDividerPosY1, 16, rect.getHeight()-(16+ivDividerPosY1) ) );

		ivPtChannelControl->setScrollPos( MPoint( 0, currentPosition.getY() ) );
		ivPtChannelControl->setRect(
			MRect(
				0,
				ivDividerPosY1,
				ivDividerPosX1,
				rect.getHeight() - ivDividerPosY1 - ivDividerPosY2-16 ) );
		
		ivPtGrid->setRect(
			MRect(
				ivDividerPosX1,
				ivDividerPosY1,
				rect.getWidth()-ivDividerPosX1-16,
				rect.getHeight()-ivDividerPosY1 - ivDividerPosY2-16 ) );

		ivPtLocationBar->setRect(
			MRect(
				ivDividerPosX1,
				0,
				rect.getWidth()-ivDividerPosX1-16,
				ivDividerPosY1 ) );

		ivPtLocationBar->setScrollPos( MPoint( currentPosition.getX(), 0 ) );

		ivPtInspector->setRect(
			MRect(
				ivDividerPosX1,
				rect.getHeight() - ivDividerPosY2-16,
				rect.getWidth()-ivDividerPosX1-16,
				ivDividerPosY2 ) );
		ivPtInspector->setScrollPos( MPoint( currentPosition.getX(), 0 ) );

		ivPtInspectorCombo->setRect(
			MRect(
				0,
				rect.getHeight() - ivDividerPosY2 - 16, 
				ivDividerPosX1,
				20 ) );

		ivPtScrollbarController->setScrollSizes( scrollSizeArea, ivPtGrid->getSize() );
	}
}

void MBeatBoxNoteEditor::onChannelSelected( unsigned int index )
{
	ivPtVolInspector->setEditIndex( index );
	ivPtInspector->repaint();
}

void MBeatBoxNoteEditor::onUpdateFromModel()
{
	updateFromModel();
	MSize scrollViewSize = ivPtGrid->calculateScrollViewSize();
	int maxWidth = scrollViewSize.getWidth() + 28 + ivDividerPosX1;
	int maxHeight = scrollViewSize.getHeight() + 48 + ivDividerPosY1 + ivDividerPosY2;
	setSize( MSize( maxWidth, maxHeight ) );
}

void MBeatBoxNoteEditor::onCopy()
{
	TRACE( "<MBeatBoxNoteEditor::onCopy()>\n" );
	ivPtGrid->copy();
}

void MBeatBoxNoteEditor::onCut()
{
	TRACE( "<MBeatBoxNoteEditor::onCut()>\n" );
	onCopy();
	onDelete();
}

void MBeatBoxNoteEditor::onPaste()
{
	TRACE( "<MBeatBoxNoteEditor::onPaste()>\n" );

	ivPtGrid->paste( ivPtLocationBar->getLocatorPos() );
	ivPtInspector->repaint();
}

void MBeatBoxNoteEditor::onDelete()
{
	TRACE( "<MBeatBoxNoteEditor::onDelete()>\n" );
	ivPtGrid->deleteSelected();
}

void MBeatBoxNoteEditor::stateChanged( MInstrumentEvent anEvent )
{
	if( anEvent.getMessageType() == MBeatBox::MESSAGE_CHANNEL_ADDED ||
		anEvent.getMessageType() == MBeatBox::MESSAGE_UPDATE_FROM_MODEL ||
		anEvent.getMessageType() == MBeatBox::MESSAGE_CHANNEL_REMOVED )
	{
		onUpdateFromModel();
	}
	else if( anEvent.getMessageType() == MBeatBox::MESSAGE_CHANNEL_SELECTED )
	{
		onChannelSelected( ((MBeatBox*)ivPtInstrument)->getSelection() );
	}
}

String MBeatBoxNoteEditor::getInstrumentListenerName()
{
	return "MBeatBoxNoteEditor::MBeatBoxListener";
}