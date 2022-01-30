#include "MAudioChannelEditor.h"
#include <gui/dialog/MFileDialog.h>
#include <gui/dialog/MMessageBox.h>

MAudioChannelEditor::MAudioChannelEditor() :
	ivPtListView( 0 ),
	ivPtMenu( 0 )
{
}

MAudioChannelEditor::~MAudioChannelEditor()
{
	if( ivPtListView )
		ivPtListView->setModel( 0 );
	SAFE_DELETE( ivPtListView );
	SAFE_DELETE( ivPtMenu );
}

bool MAudioChannelEditor::create( MRect rect, MTopWnd *parentWnd )
{
	bool back = MInstrumentEditor::create( rect, parentWnd );
	if( back )
	{
		ivPtMenu = new MMenu();
		ivPtMenu->load( IDR_AUDIOCHANNEL_EDITOR );
		setMenu( ivPtMenu );
		ivPtListView = new MScrolledListView( ((MAudioChannel*)ivPtInstrument)->getFileList() );
		addChild( ivPtListView );
	}
	return back;
}

void MAudioChannelEditor::doLayout( bool autoSize )
{
	MRect rect = getRect();
	if( ivPtListView )
		ivPtListView->doLayout();

	if( autoSize )
		setPreferredSize( MSize( 400, 200 ) );
}

MInstrumentEditor* MAudioChannelEditor::createInstance()
{
	return new MAudioChannelEditor();
}

void MAudioChannelEditor::setInstrument( MInstrument* ptInstrument )
{
	MInstrumentEditor::setInstrument( ptInstrument );
	if( ivPtListView )
		ivPtListView->setModel( ((MAudioChannel*)ivPtInstrument)->getFileList() );
}

void MAudioChannelEditor::onCommand( unsigned int id )
{
	if( id == ID_FILE_IMPORTFILE454 )
	{
		MFileDialog dlg;
		if( dlg.showOpenDialog(
			((MTopWnd*)getTopParent()),
			"Open wave",
			"Windows PCM Wave\0*.WAV\0All files\0*.*\0\0" ) )
		{
			String fileName = dlg.getSelectedFile();
			fileName.Replace( "\\", "/" );
			try
			{
				((MAudioChannel*)ivPtInstrument)->importFile( fileName );
			}
			catch(MException e)
			{	
				MMessageBox::showError(
					"ERROR_LOADING", fileName, e.getExceptionDescripton() );
			}
		}
	}
	else
		MInstrumentEditor::onCommand( id );
}