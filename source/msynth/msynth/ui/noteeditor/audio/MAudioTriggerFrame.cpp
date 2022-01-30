#include "MAudioTriggerFrame.h"

#include "../../../../MAppData.h"
#include <gui/MApp.h>

#define DEFAULT_PART_SIZE_X		20
#define DEFAULT_PART_SIZE_Y		12

MAudioTriggerFrame::MAudioTriggerFrame() :
	MAbstractNoteFrame(),
	ivPtAudioFiles( 0 ),
	ivPtFEInspector( 0 )
{
	ivPtPartSize = new MSize( DEFAULT_PART_SIZE_X, DEFAULT_PART_SIZE_Y );
	ivTitle = "Audiochannel";
}

MAudioTriggerFrame::~MAudioTriggerFrame()
{
	MClipBoard::getInstance()->unsetContext( this );

	ivPtAudioFiles->setModel( 0 );
	ivPtInspector->setBar( 0 );
	ivPtInspector->removeInspector( ivPtFEInspector );

	setBar( 0 );
	setInstrument( 0 );

	SAFE_DELETE( ivPtGrid );
	SAFE_DELETE( ivPtFEInspector );
	SAFE_DELETE( ivPtPartSize );
	SAFE_DELETE( ivPtGrid );

	SAFE_DELETE( ivPtAudioFiles );
}

bool MAudioTriggerFrame::create( MRect rect, MTopWnd *parentWnd )
{
	ASSERT( ivPtPartSize );
	ASSERT( ivPtBar );
	ASSERT( ivPtInstrument );
	ASSERT( ivPtBpmControl );

	MAudioTriggerList* ptNoteList = (MAudioTriggerList*) ivPtBar->getNoteList();

	ivPtAudioFiles = new MComboBox( ((MAudioChannel*)ivPtInstrument)->getFileList() );

	ivPtGrid = new MAudioChannelGrid( ivPtPartSize );
	((MAudioChannelGrid*)ivPtGrid)->setBpmControl( ivPtBpmControl );
	((MAudioChannelGrid*)ivPtGrid)->setData(
		((MAudioChannel*)ivPtInstrument),
		ptNoteList );

	ivDividerPosY1 = 22;
	ivDividerPosY2 = 80;
	ivDividerPosX1 = 100;

	bool back =	MAbstractNoteFrame::create(	rect, MApp::getInstance()->getMainWnd() );
	if( back )
	{
		ivPtFEInspector = new MFloatEventInspector();
		ivPtInspector->addInspector( ivPtFEInspector );
		addChild( ivPtAudioFiles );
		addChild( ivPtGrid );
		ivPtInspectorCombo->setBar( ivPtBar );
		setText( "Audiochannel" );
		doLayout();
	}
	return back;
};

void MAudioTriggerFrame::setBar( MBar* ptBar )
{
	MAbstractNoteFrame::setBar( ptBar );
	if( ivPtBar )
		setProperties( ptBar->getProperties() );
	else
		setProperties( 0 );
}

void MAudioTriggerFrame::updateFromModel()
{
	ivPtGrid->updateFromModel();
	ivPtGrid->repaint();

	ivPtLocationBar->setNotesPerBeat( ivPtBar->getNotesPerBar() / 4 );
	ivPtLocationBar->setBeatCount( 4 * ivPtBar->getLength() );
	ivPtLocationBar->setScrollPos( MPoint( ivPtGrid->getScrollPos().getX(), 0 ) );
	ivPtLocationBar->repaint();

	ivPtInspector->repaint();

	this->updateWindowText();
}

void MAudioTriggerFrame::doLayout()
{
	MRect rect = getRect();

	ivPtButtonSelect->setRect( MRect( 0, 0, 22, 22 ) );
	ivPtButtonDraw->setRect( MRect( 22, 0, 22, 22 ) );

	ivPtPartSize->setHeight( rect.getHeight() - ivDividerPosY1 - ivDividerPosY2 - 16 );

	ivPtScrollbarHorizontal->setRect( MRect( ivDividerPosX1, rect.getHeight() - 16, rect.getWidth()-16, rect.getHeight() ) );
	ivPtScrollbarVertical->setRect( MRect( rect.getWidth() - 16, ivDividerPosY1, rect.getWidth(), rect.getHeight()-16 ) );

	ivPtGrid->setRect(
		MRect(
			ivDividerPosX1,
			ivDividerPosY1,
			rect.getWidth() - ivDividerPosX1 - 16,
			rect.getHeight() - ivDividerPosY1 - ivDividerPosY2 - 16 ) );
	
	ivPtAudioFiles->setRect(
		MRect(
			0,
			ivDividerPosY1,
			ivDividerPosX1,
			20 ) );

	ivPtLocationBar->setRect(
		MRect(
			ivDividerPosX1,
			0,
			rect.getWidth()-ivDividerPosX1,
			ivDividerPosY1 ) );

	ivPtInspector->setRect(
		MRect(
			ivDividerPosX1,
			rect.getHeight() - ivDividerPosY2 - 16,
			rect.getWidth()-ivDividerPosX1 - 16,
			ivDividerPosY2 ) );

	ivPtInspectorCombo->setRect(
		MRect(
			0,
			rect.getHeight() - ivDividerPosY2 - 16, 
			ivDividerPosX1,
			20 ) );

	ivPtScrollbarController->setScrollSizes(
		ivPtGrid->calculateScrollViewSize(),
		ivPtGrid->getSize() );

	ivButtonControl.setCurrentButton( this->ivPtButtonSelect );
}

MAbstractNoteFrame* MAudioTriggerFrame::createInstance()
{
	return new MAudioTriggerFrame();
}

void MAudioTriggerFrame::onCopy()
{
	TRACE( "<MNoteFrame::onCopy()>\n" );
	ivPtGrid->copy();
}

void MAudioTriggerFrame::onCut()
{
	TRACE( "<MNoteFrame::onCut()>\n" );
	ivPtGrid->copy();
	ivPtGrid->deleteSelected();
	ivPtInspector->repaint();
}

void MAudioTriggerFrame::onPaste()
{
	TRACE( "<MNoteFrame::onPaste()>\n" );
	ivPtGrid->paste( ivPtLocationBar->getLocatorPos() );
	ivPtInspector->repaint();
}

void MAudioTriggerFrame::onDelete()
{
	TRACE( "<MNoteFrame::onDelete()>\n" );
	ivPtGrid->deleteSelected();
	ivPtInspector->repaint();
}