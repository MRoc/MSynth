/**
 * MSynthEditor (C)2000-2003 MRoc hifiShock
 *
 * v. 1.0			- Working
 * v. 1.1			- Inheritanced from MInstrumentEditor,
 *						removed step sequencer.
 * v. 1.2(11.2001) - Add/remove/Swap support
 */

#include "MSynthEditor.h"
#include <gui/dialog/MFileDialog.h>
#include "../../../../resource.h"

INIT_RTTI( MSynthEditor, "MSynthEditor" );

MSynthEditor::MSynthEditor() :
	MInstrumentEditor(),
	ivPtSynth( 0 ),
	ivSynthView( 0 ),
	ivPtScrollPane( 0 ),
	ivPtEditor( 0 )
{
	ivBkColor = MColorMap::create(173,170,173);
	ivSynthView	= new MSynthScrollView();
	ivPtScrollPane = new MScrollPane( ivSynthView );
	addChild( ivPtScrollPane );
}

MSynthEditor::~MSynthEditor()
{
	SAFE_DELETE( ivPtEditor );
}

bool MSynthEditor::create( MRect rect, MTopWnd *parentWnd )
{
	bool back = MInstrumentEditor::create( rect, parentWnd );
	if( back )
	{
		MMenu* pMenu = new MMenu();
		pMenu->load( IDR_SYNTH_EDITOR );
		setMenu( pMenu );
		doLayout( true );
	}
	return back;
}

void MSynthEditor::doLayout( bool autoSize )
{
	if( ivSynthView )
	{
		setMaximumSize(	ivSynthView->getPreferredSize().add( 16, 16 ) );
		if( autoSize )
		{
			doLayout();
			setPreferredSize( ivSynthView->getPreferredSize() );
			doLayout();
			MWnd::repaint();
		}
	}
}

void MSynthEditor::doLayout()
{
	if( ivPtScrollPane )
		ivPtScrollPane->setRect( MRect( MPoint(), getClientSize() ) );
}

void MSynthEditor::updateFromModel()
{
	MInstrumentEditor::updateFromModel();
	if( ivSynthView )
	{
		ivSynthView->updateGui();
		ivSynthView->updateFromModel();
	}
	doLayout( false );
}

void MSynthEditor::UpdateTitle()
{
	setText( ivPtInstrument->getChannelName() );
}

void MSynthEditor::setInstrument( MInstrument* ptInstrument )
{
	MInstrumentEditor::setInstrument( ptInstrument );

	ivPtSynth = (Synth*) ptInstrument;

	if( ivSynthView )
		ivSynthView->setSynth( ivPtSynth );
	setProperties( ivPtSynth ? ivPtSynth->getProperties() : 0 );
}

Synth *MSynthEditor::getSynth()
{
	return ivPtSynth;
}

void MSynthEditor::OnEdit()
{
	if( ivPtEditor == 0 )
	{
		ivPtEditor = new MSynthStructureFrame( ivPtSynth );
		ivPtEditor->setActionOnClose( MFrameWnd::DESTROY );
		ivPtEditor->addListener( this );
		ivPtEditor->create( MRect( 0, 0, 500, 400 ), this );
	}
	ivPtEditor->setVisible( true );
}

void MSynthEditor::OnReset()
{
	ivPtSynth->processEvent( &MEvent( MEvent::RESET ) );
}

MInstrumentEditor* MSynthEditor::createInstance()
{
	return new MSynthEditor();
}

void MSynthEditor::OnOpen()
{
 	MFileDialog dialog;
	if( dialog.showOpenDialog(
		((MTopWnd*)getTopParent()),
		"Open synth",
		"Synths\0*.MSL\0All files\0*.*\0\0" ) )
		ivPtSynth->open( dialog.getSelectedFile() );
}

void MSynthEditor::OnSaveAs()
{
#ifndef __TESTVERSION
	MFileDialog dialog;
	if( dialog.showOpenDialog(
		((MTopWnd*)getTopParent()),
		"Save synth",
		"Synths\0*.MSL\0All files\0*.*\0\0" ) )
		ivPtSynth->save( dialog.getSelectedFile() );
#else
	MMessageBox::showError( "DISABLED_IN_DEMO" );
#endif
}

void MSynthEditor::OnCloseWindow()
{
	delete this;
}

	//IFrameWndListener
void MSynthEditor::onClose( MTopWnd* pWnd )
{
}
	//IFrameWndListener
void MSynthEditor::onDestroy( MTopWnd* pWnd )
{
	ivPtEditor = 0;
	pWnd->removeListener( this );
}

void MSynthEditor::onCommand( unsigned int id )
{
	switch( id )
	{
	case ID_EDIT_EDIT:
		OnEdit();
		break;
	case ID_EDIT_RESET:
		OnReset();
		break;
	case ID_OPEN_SYNTH:
		OnOpen();
		break;
	case ID_SAVE_SYNTH:
		OnSaveAs();
		break;
	case ID_FILE_CLOSE_WINDOW:
		OnCloseWindow();
		break;
	};
}