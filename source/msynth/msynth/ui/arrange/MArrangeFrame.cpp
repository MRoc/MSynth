/**
 * (C)2001-2003 MRoc
 */

#include "MArrangeFrame.h"
#include <gui/dialog/MMessageBox.h>

#include <framework/resource/text/MTextResource.h>
#include <gui/MWndManager.h>

INIT_RTTI( MArrangeFrame, "MArrangeFrame" );
INIT_RTTI( MArrangeFrame::MSongListener, "MArrangeFrame::MSongListener" );
INIT_RTTI( MArrangeFrame::MComboBoxListener, "MArrangeFrame::MComboBoxListener" );
INIT_RTTI( MArrangeFrame::MToolBarListener, "MArrangeFrame::MToolBarListener" );
INIT_RTTI( MArrangeFrame::MSizerBarListener, "MArrangeFrame::MSizerBarListener" );

MArrangeFrame::MArrangeFrame( int id, MSong *ptSong, MTimeController* ptTimeController, MUpdateThread* ptUpdateThread, IBpmControl* ptBpmControl )
	: MWndCollection(),
	ivPartSize( new MSize( 50, 30 ) ),
	ivDividerPosX( 140 ),
	ivDividerPosY( 22 ),
	ivSplitBarWidth( 5 ),
	ivSplitDragged( false ),
	ivPtSplitBar( 0 ),
	ivPtGrid( 0 ),
	ivPtScrollPane( 0 ),
	ivPtChannelContainer( 0 ),
	ivPtLocationBar( 0 ),
	ivPtButtonSelect( 0 ),
	ivPtButtonDraw( 0 ),
	ivPtButtonGlow( 0 ),
	ivPtButtonCut( 0 ),
	ivPtSong( 0 ),
	ivPtTimeController( 0 ),
	ivPtUpdateThread( ptUpdateThread ),
	ivPtComboBox( 0 ),
	ivPtSelectionListener( 0 ),
	ivPtBpmControl( ptBpmControl ),
	ivPtListener( 0 ),
	ivPtToolBarListener( 0 ),
	ivPtSizerBarListener( 0 )
{
	ASSERT( ptSong && ptTimeController );

	ivBkColor = MColorMap::BK_COLOR1;

	ivPtButtonSelect		= new MToggleButton( IDB_ARROW, IDB_ARROW, MColorMap::create(255,0,0), MColorMap::create(255,0,0), MColorMap::BK_COLOR3 );
	ivPtButtonDraw			= new MToggleButton( IDB_PEN, IDB_PEN, MColorMap::create(255,0,0), MColorMap::create(255,0,0), MColorMap::BK_COLOR3 );
	ivPtButtonGlow			= new MToggleButton( IDB_GLOW, IDB_GLOW, MColorMap::create(255,0,0), MColorMap::create(255,0,0), MColorMap::BK_COLOR3 );
	ivPtButtonCut			= new MToggleButton( IDB_CUTTER, IDB_CUTTER, MColorMap::create(255,0,0), MColorMap::create(255,0,0), MColorMap::BK_COLOR3 );
	ivPtGrid				= new MBarGrid( ptSong, ptTimeController, ptBpmControl, ivPartSize );
	ivPtScrollPane			= new MScrollPane( ivPtGrid );
	ivPtLocationBar			= new MBarLocationBar( ptSong->getMaxBarCount() );
	ivPtChannelContainer	= new MArrangeChannelContainer( ptSong );
	ivPtSplitBar			= new MSplitBar( MSplitBar::HORIZONTAL );
	ivPtComboBox			= new MComboBox();

	addChild( ivPtButtonSelect );
	addChild( ivPtButtonDraw );
	addChild( ivPtButtonGlow );
	addChild( ivPtButtonCut );
	addChild( ivPtScrollPane );
	addChild( ivPtLocationBar );
	addChild( ivPtChannelContainer );
	addChild( ivPtSplitBar );
	addChild( ivPtComboBox );

	ivToolBarControl.addToggleButton( ivPtButtonSelect );
	ivToolBarControl.addToggleButton( ivPtButtonDraw );
	ivToolBarControl.addToggleButton( ivPtButtonGlow );
	ivToolBarControl.addToggleButton( ivPtButtonCut );
	ivPtToolBarListener = new MToolBarListener( this );

	ivPtButtonSelect->setToolTipText( MTextResource::getInstance()->getString("TT_ARRANGE_SELECT_BUTTON") );
	ivPtButtonDraw->setToolTipText( MTextResource::getInstance()->getString("TT_ARRANGE_DRAW_BUTTON") );
	ivPtButtonGlow->setToolTipText( MTextResource::getInstance()->getString("TT_ARRANGE_GLOW_BUTTON") );
	ivPtButtonCut->setToolTipText( MTextResource::getInstance()->getString("TT_ARRANGE_CUT_BUTTON") );

	ivPtComboBox->setToolTipText( MTextResource::getInstance()->getString("TT_ARRANGE_RESOLUTION_COMBO") );
	ivPtComboBox->getModel()->add( new MInt( 16 ) );
	ivPtComboBox->getModel()->add( new MInt( 32 ) );
	ivPtSelectionListener = new MComboBoxListener( this, ivPtComboBox );
	ivPtComboBox->setSelection( 0 );
	ivPtSizerBarListener = new MSizerBarListener( this );
	setSong( ptSong );
	setTimeController( ptTimeController );

	ivPtChannelContainer->setPartSize( *ivPartSize );
	ivPtLocationBar->setBarWidth( ivPartSize->getWidth() );

		if( ivPtChannelContainer )
		{
			ivPtChannelContainer->setBkColor( ivBkColor );
			ivPtChannelContainer->builtGui();
		}

		if( ivPtSplitBar )
		{
			ivPtSplitBar->setBkColor( MColorMap::BK_COLOR3 );
			ivPtSplitBar->setFgColor( MColorMap::FG_COLOR3 );
		}

		if( ivPtLocationBar )
		{
			ivPtLocationBar->setTimeController( ivPtTimeController );
			if( ivPtUpdateThread )
				ivPtLocationBar->setUpdateThread( ivPtUpdateThread );
		}

		if( ivPtScrollPane )
		{
			if( ivPtLocationBar )
				ivPtScrollPane->getController()->addHorizontalScrollWnd( ivPtLocationBar );
			if( ivPtChannelContainer )
				ivPtScrollPane->getController()->addVerticalScrollWnd( ivPtChannelContainer );

			MSize scrollSizeArea = ivPtGrid->calculateScrollViewSize();
			ivPtScrollPane->setScrollSize( scrollSizeArea );
		}

		if( ivPtButtonSelect )
			ivToolBarControl.setCurrentButton( ivPtButtonSelect );

		if( ivPtGrid )
			ivPtGrid->getSelectionManager()->addListener( this );

		doLayout();
}

MArrangeFrame::~MArrangeFrame()
{
	ivChilds.clear();

	MClipBoard::getInstance()->unsetContext( this );

	if( ivPtGrid )
        ivPtGrid->getSelectionManager()->removeListener( this );
	if( ivPtLocationBar )
		ivPtLocationBar->setTimeController( 0 );

	if( ivPtButtonSelect )
		ivToolBarControl.removeToggleButton( ivPtButtonSelect );
	if( ivPtButtonDraw )
		ivToolBarControl.removeToggleButton( ivPtButtonDraw );
	if( ivPtButtonGlow )
		ivToolBarControl.removeToggleButton( ivPtButtonGlow );
	if( ivPtButtonCut )
		ivToolBarControl.removeToggleButton( ivPtButtonCut );

	setSong( 0 );

	if( ivPtTimeController )
		ivPtTimeController = 0;

	SAFE_DELETE( ivPtButtonSelect );
	SAFE_DELETE( ivPtButtonDraw );
	SAFE_DELETE( ivPtButtonGlow );
	SAFE_DELETE( ivPtButtonCut );

	SAFE_DELETE( ivPtToolBarListener );
	SAFE_DELETE( ivPtSizerBarListener );

	SAFE_DELETE( ivPtChannelContainer );
	SAFE_DELETE( ivPtLocationBar );
	SAFE_DELETE( ivPtScrollPane );
	SAFE_DELETE( ivPtSelectionListener );
	SAFE_DELETE( ivPtComboBox );

	SAFE_DELETE( ivPtSplitBar );

	SAFE_DELETE( ivPartSize );
}

void MArrangeFrame::updateFromModel()
{
	if( ivPtChannelContainer )
        ivPtChannelContainer->updateFromModel();

	if( ivPtGrid )
	{
		ivPtGrid->updateFromModel();
		ivPtGrid->repaint();
	}
}

void MArrangeFrame::doLayout()
{
	if( ivPtButtonSelect && 
		ivPtButtonDraw && 
		ivPtButtonGlow &&
		ivPtButtonCut && 
		ivPtComboBox && 
		ivPtChannelContainer && 
		ivPtSplitBar && 
		ivPtLocationBar && 
		ivPtScrollPane )
	{
		//TRACE( "MArrangeFrame::doLayout()\n" );
		MSize size = getSize();
		int height = (int) size.getHeight();
		int width = (int) size.getWidth();

		int x2 = ivSplitBarWidth + ivDividerPosX;
		int width2 = width-x2;

		ivPtButtonSelect->setRect( MRect( 0, 0, 24, ivDividerPosY ) );
		ivPtButtonDraw->setRect( MRect( 24, 0, 24, ivDividerPosY ) );
		ivPtButtonGlow->setRect( MRect( 48, 0, 24, ivDividerPosY ) );
		ivPtButtonCut->setRect( MRect( 72, 0, 24, ivDividerPosY ) );
		ivPtComboBox->setRect( MRect( 96, 0, 43, ivDividerPosY ) );
		ivPtChannelContainer->setRect( MRect( 0, ivDividerPosY, ivDividerPosX, height ) );
		ivPtSplitBar->setRect( MRect( ivDividerPosX, 0, ivSplitBarWidth, height ) );
		ivPtLocationBar->setRect( MRect( x2, 0, width2, ivDividerPosY ) );
		ivPtScrollPane->setRect( MRect( x2, ivDividerPosY, width2, height-ivDividerPosY ) );

		ivPtScrollPane->getController()->setScrollSizes(
			ivPtGrid->calculateScrollViewSize(),
			size );
	}
}


void MArrangeFrame::selectionChanged( MObject* newSelection, MObject* ptSource )
{
	if( ivPtGrid->getFirstSelection() != 0 )
	{
		switch( ivPtGrid->getFirstSelection()->getBar()->getNotesPerBar() )
		{
			case 16:
				ivPtComboBox->setSelection( 0 );
				break;
			case 32:
				ivPtComboBox->setSelection( 1 );
				break;
		};
	}
}

void MArrangeFrame::setSong( MSong* ptSong )
{
	if( ivPtSong )
		SAFE_DELETE( ivPtListener );

	ivPtSong = ptSong;

	if( ivPtSong )
		ivPtListener = new MSongListener( ivPtSong, this );
}

MSong* MArrangeFrame::getSong()
{
	return ivPtSong;
}

void MArrangeFrame::setTimeController( MTimeController* ptController )
{
	ivPtTimeController = ptController;
}

MTimeController* MArrangeFrame::getTimeController()
{
	return ivPtTimeController;
}

MSize* MArrangeFrame::getPartSize()
{
	return ivPartSize;
}

void MArrangeFrame::updateFromPartSize()
{
	if( ivPtChannelContainer &&
		ivPtLocationBar &&
		ivPtGrid )
	{
		ivPtChannelContainer->doLayout();
		ivPtLocationBar->repaint();
		ivPtGrid->updateFromPartSize();
		doLayout();
	}
}

void MArrangeFrame::onCopy()
{
	ivPtGrid->copy();
}

void MArrangeFrame::onCut()
{
	ivPtGrid->cut();
}

void MArrangeFrame::onPaste()
{
	ivPtGrid->paste( ivPtTimeController->getCurrentPattern() );
}

void MArrangeFrame::onDelete()
{
	if( ivPtGrid->hasSelectedControls() )
		ivPtGrid->deleteSelected();
	else if( ivPtChannelContainer->GetSelectedChannel() != -1 &&
		MMessageBox::showQuestion( "DELETE_CHANNEL" ) == IDYES )
	{
		ivPtSong->deleteBarContainer( ivPtChannelContainer->GetSelectedChannel(), true );
		doLayout();
	}
}


void MArrangeFrame::onMouseUp( MMouseEvent* anEvent )
{
	if( ivSplitDragged )
	{
		ivSplitDragged = false;
		ivPtSplitBar->setSelected( false );
		ivPtSplitBar->repaint();
		if( ReleaseCapture() == false )
			MLogger::logError(
				"MArrangeFrame::onMouseUp: ReleaseCapture failed" );
	}
}

void MArrangeFrame::onMouseMove( MMouseEvent* anEvent )
{
	MRect clientRect = getRect();
	MPoint p = anEvent->getPoint();

	if( ivSplitDragged )
	{
		if( p.getX() < 140 )
			p.setX( 140 );
		else if( p.getX() > ((int)clientRect.getWidth()) - 50 )
			p.setX( ((int)clientRect.getWidth()) - 50 );

		ivDividerPosX = p.getX();
		doLayout();
		repaint();
	}
}

void MArrangeFrame::closeChildFrames()
{
	if( ivPtChannelContainer )
		ivPtChannelContainer->closeChildFrames();
	if( ivPtGrid )
		ivPtGrid->closeChildFrames();
}

void MArrangeFrame::storeChildPositions()
{
	if( ivPtChannelContainer )
		ivPtChannelContainer->storeChildFramePosition();
	if( ivPtGrid )
		ivPtGrid->storeChildFramePosition();
}

MArrangeFrame::MSongListener::MSongListener( MSong* ptSong, MArrangeFrame* ptFrame ) :
	ivPtSong( ptSong ),
	ivPtFrame( ptFrame )
{
	ivPtSong->addSongListener( this );
}

MArrangeFrame::MSongListener::~MSongListener()
{
	ivPtSong->removeSongListener( this );
}

void MArrangeFrame::MSongListener::instrumentAdded( unsigned int index )
{
	ivPtFrame->ivPtChannelContainer->onChannelAdded();
	ivPtFrame->doLayout();
	ivPtFrame->repaint();
}

void MArrangeFrame::MSongListener::instrumentRemoved( unsigned int index )
{
	ivPtFrame->ivPtChannelContainer->onChannelRemoved( index );
	ivPtFrame->doLayout();
	ivPtFrame->repaint();
}

void MArrangeFrame::MSongListener::instrumentsSwapped( unsigned int index1, unsigned int index2 )
{
	ivPtFrame->ivPtChannelContainer->onChannelsSwapped( index1, index2 );
	ivPtFrame->repaint();
}

void MArrangeFrame::MSongListener::songModelChanged()
{
	ivPtFrame->ivPtGrid->updateFromModel();
	ivPtFrame->ivPtChannelContainer->builtGui();
	ivPtFrame->doLayout();
	ivPtFrame->repaint();
}

void MArrangeFrame::MSongListener::songDestroy()
{
}

String MArrangeFrame::MSongListener::getClassName()
{
	return "MArrangeFrame::MSongListener";
}


MArrangeFrame::MComboBoxListener::MComboBoxListener( MArrangeFrame* ptParent, MComboBox* ptComboBox ) :
	ivPtFrame( ptParent ),
	ivPtComboBox( ptComboBox )
{
	ivPtComboBox->getModel()->addSelectionListener( this );
}

MArrangeFrame::MComboBoxListener::~MComboBoxListener()
{
	ivPtComboBox->getModel()->removeSelectionListener( this );
}

void MArrangeFrame::MComboBoxListener::selectionChanged( MObject* newSelection, MObject* ptSource )
{
	MInt* ptInt = (MInt*) ivPtFrame->ivPtComboBox->getSelectedObject();
	ivPtFrame->ivPtGrid->changeResolution( ptInt->getValue() );
}

String MArrangeFrame::MComboBoxListener::getClassName()
{
	return "MArrangeFrame::MComboBoxListener";
}

MArrangeFrame::MToolBarListener::MToolBarListener( MArrangeFrame* ptFrame ) :
	ivPtFrame( ptFrame )
{
	ivPtFrame->ivToolBarControl.addActionListener( this );
}

MArrangeFrame::MToolBarListener::~MToolBarListener()
{
	ivPtFrame->ivToolBarControl.removeActionListener( this );
}

void MArrangeFrame::MToolBarListener::onActionPerformed( void* ptSrc )
{
	MToggleButton *ptButton = ivPtFrame->ivToolBarControl.getCurrentButton();
	if( ptButton == ivPtFrame->ivPtButtonSelect )
		ivPtFrame->ivPtGrid->setEditMode( MBarGrid::SELECT_MODE );
	else if( ptButton == ivPtFrame->ivPtButtonDraw )
		ivPtFrame->ivPtGrid->setEditMode( MBarGrid::DRAW_MODE );
	else if( ptButton == ivPtFrame->ivPtButtonGlow )
		ivPtFrame->ivPtGrid->setEditMode( MBarGrid::GLOW_MODE );
	else if( ptButton == ivPtFrame->ivPtButtonCut )
		ivPtFrame->ivPtGrid->setEditMode( MBarGrid::CUT_MODE );
}


MArrangeFrame::MSizerBarListener::MSizerBarListener( MArrangeFrame* ptFrame ) :
	ivPtFrame( ptFrame )
{
	ivPtFrame->ivPtSplitBar->addActionListener( this );
}

MArrangeFrame::MSizerBarListener::~MSizerBarListener()
{
	ivPtFrame->ivPtSplitBar->removeActionListener( this );
}

void MArrangeFrame::MSizerBarListener::onActionPerformed( void* ptSrc )
{
	ivPtFrame->ivSplitDragged = true;
	MWndManager::getInstance()->setMouseCapture( ivPtFrame );
}