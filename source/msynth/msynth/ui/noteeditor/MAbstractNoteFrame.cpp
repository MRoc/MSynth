/**
 * (C) 2001-2003 MRoc
 */

#include "MAbstractNoteFrame.h"
#include <framework/resource/text/MTextResource.h>

INIT_RTTI( MAbstractNoteFrame, "MAbstractNoteFrame" );
INIT_RTTI( MAbstractNoteFrame::MInstrumentListener, "MAbstractNoteFrame::MInstrumentListener" );
INIT_RTTI( MAbstractNoteFrame::MBarListener, "MAbstractNoteFrame::MBarListener" );
INIT_RTTI( MAbstractNoteFrame::MControlListener, "MAbstractNoteFrame::MControlListener" );
INIT_RTTI( MAbstractNoteFrame::MButtonListener, "MAbstractNoteFrame::MButtonListener" );

MAbstractNoteFrame::MAbstractNoteFrame() :
	MFrameWnd(),
	ivPtBar( 0 ),
	ivPtInstrument( 0 ),
	ivPtPartSize( 0 ),
	ivPtBarListener( 0 ),
	ivPtInstrumentListener( 0 ),
	ivPtScrollbarHorizontal( 0 ),
	ivPtScrollbarVertical( 0 ),
	ivPtScrollbarController( 0 ),
	ivPtGrid( 0 ),
	ivPtLocationBar( 0 ),
	ivPtInspector( 0 ),
	ivPtInspectorCombo( 0 ),
	ivPtBpmControl( 0 ),
	ivPtControlListener( 0 ),
	ivPtButtonSelect(0),
	ivPtButtonDraw(0),
	ivPtButtonListener( 0 )
{
	setActionOnClose( DESTROY );
	setBkColor( MColorMap::BK_COLOR2 );

	ivPtScrollbarHorizontal = new MScrollBar( MScrollBar::HORIZONTAL );
	addChild( ivPtScrollbarHorizontal );

	ivPtScrollbarVertical = new MScrollBar( MScrollBar::VERTICAL );
	addChild( ivPtScrollbarVertical );

	ivPtButtonSelect = new MToggleButton(
			IDB_ARROW,
			IDB_ARROW,
			MColorMap::create(255,0,0),
			MColorMap::create(255,0,0),
			MColorMap::BK_COLOR3 );
	addChild( ivPtButtonSelect );
	ivButtonControl.addToggleButton( ivPtButtonSelect );
	ivPtButtonSelect->setToolTipText( MTextResource::getInstance()->getString("TT_SYNTH_NOTEEDITOR_SELECT_BUTTON" ) );

	ivPtButtonDraw =  new MToggleButton(
			IDB_PEN,
			IDB_PEN,
			MColorMap::create(255,0,0),
			MColorMap::create(255,0,0),
			MColorMap::BK_COLOR3 );
	addChild( ivPtButtonDraw );
	ivButtonControl.addToggleButton( ivPtButtonDraw );
	ivPtButtonDraw->setToolTipText( MTextResource::getInstance()->getString("TT_SYNTH_NOTEEDITOR_DRAW_BUTTON" ) );

	ivPtBarListener = new MBarListener( this );
	ivPtButtonListener = new MButtonListener( this );
}

MAbstractNoteFrame::~MAbstractNoteFrame()
{
	TRACE( "<mabstractnoteframe::mabstractnoteframe childcount=\"%i\"/>\n", getChildCount() );

	setBar( 0 );
	setInstrument( 0 );
	ivPtGrid->removeListener( this );

	ivButtonControl.removeToggleButton( ivPtButtonSelect );
	ivButtonControl.removeToggleButton( ivPtButtonDraw );
	if( ivPtInspector )
		ivPtInspector->removeListener( this );

	SAFE_DELETE( ivPtBarListener );
	SAFE_DELETE( ivPtButtonListener );
	SAFE_DELETE( ivPtScrollbarController );
}

void MAbstractNoteFrame::setBpmControl( IBpmControl* ptControl )
{
	ivPtBpmControl = ptControl;
}

IBpmControl* MAbstractNoteFrame::getBpmControl()
{
	return ivPtBpmControl;
}

bool MAbstractNoteFrame::create( MRect rect, MTopWnd* parentWnd )
{
	ASSERT( ivPtInstrument != 0 );
	ASSERT( ivPtPartSize != 0 );
	ASSERT( ivPtBar != 0 );
	ASSERT( ivPtScrollbarHorizontal );
	ASSERT( ivPtScrollbarVertical );

	// try to restore window position
	MRect rect2;
	MRect* ptRect = 0;
	if( ivPtProperties && (ptRect=(MRect*)ivPtProperties->get( "windowposition" )) )
		rect2 = *ptRect;
	else
	{
		int sH = GetSystemMetrics( SM_CYFULLSCREEN );
		int sW = GetSystemMetrics( SM_CXFULLSCREEN );
		int pH = 480;
		int pW = 640;
		rect2 =
			MRect(
				(sW-pW) / 2,
				(sH-pH) / 2,
				pW,
				pH );
	}

	// create scrollbar controller,
	// at this point, the ivPtGrid must be created!
	ivPtScrollbarController =
		new MScrollBarController(
			ivPtGrid,
			ivPtScrollbarHorizontal,
			ivPtScrollbarVertical );

	// create MFrameWnd
	bool back =
		MFrameWnd::create(
			rect2,
			parentWnd );
	if( back )
	{
		ivPtGrid->addListener( this );

		// init note location bar
		ivPtLocationBar = new MNoteLocationBar(
				4 * ivPtBar->getLength(),
				ivPtBar->getNotesPerBar() / 4,
				ivPtPartSize );
		addChild( ivPtLocationBar );
		ivPtLocationBar->setNotesPerBeat( ivPtBar->getNotesPerBar() / 4);
		ivPtScrollbarController->addHorizontalScrollWnd( ivPtLocationBar );

		// init note inspector
		ivPtInspector = new MNoteInspector(
				ivPtBar,
				ivPtPartSize );
		addChild( ivPtInspector );
		ivPtScrollbarController->addHorizontalScrollWnd( ivPtInspector );
		ivPtInspector->addListener( this );

		// init inspector combo
		if( ivPtInspector )
		{
			ivPtInspectorCombo = new MInspectorCombo();
			addChild( ivPtInspectorCombo );
			ivPtInspectorCombo->setNoteInspector( ivPtInspector );
		}

		TRACE( "MAbstractNoteFrame::create childs: %i\n", getChildCount() );
	}
	return back;
}

void MAbstractNoteFrame::setInstrument( MInstrument* ptInstrument )
{
	if( ivPtInstrument )
	{
		SAFE_DELETE( ivPtInstrumentListener );
		SAFE_DELETE( ivPtControlListener );
	}

	ivPtInstrument = ptInstrument;

	if( ivPtInstrument )
	{
		ivPtInstrumentListener = new MAbstractNoteFrame::MInstrumentListener( ivPtInstrument, this );
		ivPtControlListener = new MAbstractNoteFrame::MControlListener( this );
	}
}

MInstrument* MAbstractNoteFrame::getInstrument()
{
	return ivPtInstrument;
}

void MAbstractNoteFrame::setBar( MBar *ptBar )
{
	if( ivPtBar )
		ivPtBar->removeBarListener( ivPtBarListener );
	ivPtBar = ptBar;
	if( ivPtBar )
		ivPtBar->addBarListener( ivPtBarListener );

	if( ivPtInspectorCombo )
		ivPtInspectorCombo->setBar( ptBar );
	if( ivPtInspector )
		ivPtInspector->setBar( ptBar );
}

MBar* MAbstractNoteFrame::getBar()
{
	return ivPtBar;
}

void MAbstractNoteFrame::updateWindowText()
{
	String buffer("");
	buffer.Format(
		"%s [%s] [Channel: %i Bar: %i]",
		ivTitle.getData(),
		ivPtInstrument->getChannelName().getData(),
		ivPtBar->getRow() + 1,
		ivPtBar->getIndex() + 1 );
	setText( buffer );
}

MSize* MAbstractNoteFrame::getPartSize()
{
	return ivPtPartSize;
}

void MAbstractNoteFrame::setPartSize( MSize* ptSize )
{
	ivPtPartSize = ptSize;
}

void MAbstractNoteFrame::setVisible( bool value )
{
	updateFromModel();
	MFrameWnd::setVisible( value );
}

void MAbstractNoteFrame::onActivate( bool active )
{
	if( active )
		MClipBoard::getInstance()->setContext( this );
}

void MAbstractNoteFrame::storeWindowPosition()
{
	TRACE( "<MAbstractNoteFrame::storeWindowPosition()>\n" );

	MFrameWnd::storeWindowPosition();
	if( ivPtProperties )
	{
		/*MPoint* ptPoint = new MPoint(
			ivPtScrollbarHorizontal->getModel()->getValue(),
			ivPtScrollbarVertical->getModel()->getValue() );
		ivPtProperties->store( "scrollposition", ptPoint );*/
	}
}

bool MAbstractNoteFrame::restoreWindowPosition()
{
	TRACE( "<MAbstractNoteFrame::restoreWindowPosition()>\n" );

	bool back = MFrameWnd::restoreWindowPosition();

	if( ivPtProperties )
	{
		MPoint* ptPoint = (MPoint*) ivPtProperties->get( "scrollposition" );
		if( ptPoint )
		{
			ivPtScrollbarHorizontal->getModel()->setValue( ptPoint->getX() );
			ivPtScrollbarVertical->getModel()->setValue( ptPoint->getY() );
		}
	}

	return back;
}

MAbstractNoteFrame::MInstrumentListener::MInstrumentListener( MInstrument* ptInstrument, MAbstractNoteFrame* ptFrame ) :
	ivPtInstrument( ptInstrument ),
	ivPtFrame( ptFrame )
{
	ivPtInstrument->addFinalizeListener( this );
}

MAbstractNoteFrame::MInstrumentListener::~MInstrumentListener()
{
	ivPtInstrument->removeFinalizeListener( this );
}

/**
 * called if the object is deleted, after recievieing this message,
 * the object MUST deregister itself as listener because the observed
 * object will be deleted, if not there it will result in an endless loop!!!
 */
void MAbstractNoteFrame::MInstrumentListener::objectTerminated( void* obj )
{
	ivPtFrame->setInstrument( 0 );
}

MAbstractNoteFrame::MBarListener::MBarListener( MAbstractNoteFrame* ptEditor ) :
	ivPtBarEditor( ptEditor )
{
}

MAbstractNoteFrame::MBarListener::~MBarListener()
{
}

void MAbstractNoteFrame::MBarListener::barColChanged()
{
	ivPtBarEditor->updateWindowText();
}

void MAbstractNoteFrame::MBarListener::barRowChanged()
{
	ivPtBarEditor->updateWindowText();
}

void MAbstractNoteFrame::MBarListener::barLengthChanged()
{
}

void MAbstractNoteFrame::MBarListener::barResolutionChanged()
{
}

void MAbstractNoteFrame::MBarListener::barReleasing()
{
	ivPtBarEditor->setBar( 0 );
	delete ivPtBarEditor;
	ivPtBarEditor = 0;
}

MAbstractNoteFrame::MControlListener::MControlListener( MAbstractNoteFrame* ptFrame ) :
	ivPtFrame( ptFrame )
{
	ivPtFrame->ivPtInstrument->getControl( MDefaultMixerChannel::NAME )->addControlListener( this );
}

MAbstractNoteFrame::MControlListener::~MControlListener()
{
	ivPtFrame->ivPtInstrument->getControl( MDefaultMixerChannel::NAME )->removeControlListener( this );
}

void MAbstractNoteFrame::MControlListener::valueChanged( MControlListenerEvent *anEvent )
{
	ivPtFrame->updateWindowText();
}

MAbstractNoteFrame::MButtonListener::MButtonListener( MAbstractNoteFrame* ptFrame ) :
	ivPtFrame( ptFrame )
{
	ivPtFrame->ivButtonControl.addActionListener( this );
}

MAbstractNoteFrame::MButtonListener::~MButtonListener()
{
	ivPtFrame->ivButtonControl.removeActionListener( this );
}

void MAbstractNoteFrame::MButtonListener::onActionPerformed( void* ptSrc )
{
	MToggleButton* ptButton = ivPtFrame->ivButtonControl.getCurrentButton();
	if( ptButton == ivPtFrame->ivPtButtonSelect )
		ivPtFrame->ivPtGrid->setEditMode( MDCGrid::SELECT_MODE );
	else if( ptButton == ivPtFrame->ivPtButtonDraw )
		ivPtFrame->ivPtGrid->setEditMode( MDCGrid::DRAW_MODE );
	else
		ASSERT( false );
}

void MAbstractNoteFrame::onGridContentChanged()
{
	ivPtInspector->repaint();
}

/**
 * invoked when note inspector made any changes on notes
 */
void MAbstractNoteFrame::onInspectorChanged()
{
	ivPtGrid->repaint();
}

void MAbstractNoteFrame::onCopy()
{
}

void MAbstractNoteFrame::onCut()
{
}

void MAbstractNoteFrame::onPaste()
{
}

void MAbstractNoteFrame::onDelete()
{
}