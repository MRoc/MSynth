/*

	MBeatBoxEditor (C)2000-2002 MRoc hifiShock

*/

#include "MBeatBoxEditor.h"
#include <gui/dialog/MFileDialog.h>
#include <gui/dialog/MMessageBox.h>

MBeatBoxEditor::MBeatBoxEditor() :
	MInstrumentEditor(),
	ivPtBeatBox( 0 ),
	ivChannelControl( 0 ),
	ivPtScrollPane( 0 )
{
	ivChannelControl	= new MChannelControl();
	ivPtScrollPane		= new MScrollPane( ivChannelControl );
}

MBeatBoxEditor::~MBeatBoxEditor()
{
	if( ivPtBeatBox )
		ivPtBeatBox = 0;
}

bool MBeatBoxEditor::create( MRect rect, MTopWnd *parentWnd )
{
	addChild( ivPtScrollPane );
	bool back = MInstrumentEditor::create( rect, parentWnd ) == TRUE;
	if( back )
	{
		MMenu* pMenu = new MMenu();
		if( pMenu->load( IDR_BEATBOX_EDITOR ) )
			setMenu( pMenu );
		else
			delete pMenu;
		doLayout( true );
		updateTitle();
		createControls();
	}
	return back;
}

void MBeatBoxEditor::doLayout( bool autoSize )
{	
	if( ivChannelControl )
	{
		ivPtScrollPane->setRect( getRect() );
		MSize prefSize = ivChannelControl->getPreferredSize();
		setMaximumSize( MSize( prefSize.getWidth() + 16, prefSize.getHeight() + 16 ) );
		if( autoSize )
		{
			setPreferredSize( MSize( prefSize.getWidth(), prefSize.getHeight() ) );
			doLayout();
			MWnd::repaint();
		}
	}
}

void MBeatBoxEditor::doLayout()
{
	MInstrumentEditor::doLayout();
	if( ivPtScrollPane )
	{
		ivPtScrollPane->setRect( MRect( MPoint(), getClientSize() ) );
		TRACE( "MBeatBoxEditor::doLayout size: %s\n", getClientSize().toString().getData() );
		MWnd::repaint();
	}
}

void MBeatBoxEditor::createControls()
{
	ivChannelControl->updateGui();
}

void MBeatBoxEditor::updateFromModel()
{
	MInstrumentEditor::updateFromModel();
	ivChannelControl->updateFromModel();
}

void MBeatBoxEditor::updateTitle()
{
	String sampleBankName = ivPtBeatBox->getPathName();
	String title;
	if( sampleBankName != "" )
		title = ivPtInstrument->getChannelName() + " (" + sampleBankName + ")";
	else
		title = ivPtInstrument->getChannelName();
	setText( title );
}

void MBeatBoxEditor::setInstrument( MInstrument* ptInstrument )
{
	MInstrumentEditor::setInstrument( ptInstrument );
	ivPtBeatBox = (MBeatBox*) ptInstrument;
	ivChannelControl->setBeatBox( ivPtBeatBox );
    	if( ivPtBeatBox )
		setProperties( ivPtBeatBox->getProperties() );
	else
		setProperties( 0 );
}

MBeatBox *MBeatBoxEditor::getBeatBox()
{
	return ivPtBeatBox;
}

void MBeatBoxEditor::onCommand( unsigned int id )
{
	switch( id )
	{
	case ID_SAMPLEBANK_NEW:
		{
			ivPtBeatBox->newSampleBank();
			ivPtBeatBox->setSampleBankName( "" );
			updateFromModel();
			updateTitle();
		}
		break;
	case ID_SAMPLEBANK_OPEN:
		{ 
			MFileDialog dialog;
			if( dialog.showOpenDialog(
				((MTopWnd*)this->getTopParent()),
				"Open samplebank",
				"Samplebank files\0*.MSB\0All files\0*.*\0\0" ) )
			{
				if( ! ivPtBeatBox->loadSampleBank( dialog.getSelectedFile() ) )
					MMessageBox::showError( "ERROR_LOADING_SAMPLEBANK" );
                createControls();
				doLayout( true );
				updateFromModel();
				updateTitle();
			}
		}
		break;
	case ID_SAMPLEBANK_SAVEAS:
	case ID_SAMPLEBANK_SAVE:
		{
			MFileDialog dialog;
			if(
				dialog.showSaveDialog(
					((MTopWnd*)this->getTopParent()),
					"Save samplebank",
					"Samplebank files\0*.MSB\0All files\0*.*\0\0" ) )
			{
				if( ! ivPtBeatBox->saveSampleBank( dialog.getSelectedFile() ) )
					MMessageBox::showError( "ERROR_SAVING_SAMPLEBANK" );
				else
				{
					ivPtBeatBox->setSampleBankName( dialog.getSelectedFile() );
					updateTitle();
				}
			}
		}
		break;
	case ID_CHANNEL_CREATE:
		{
			if( ivPtBeatBox->getChannelCount() < 32 )
			{
				ivPtBeatBox->createChannel();
				doLayout( true );
			}
		}
		break;
	case ID_CHANNEL_DELETE:
		{
			if( ivPtBeatBox->getWavePlayerCount() > 1 )
			{
				ivPtBeatBox->deleteChannel( ivPtBeatBox->getSelectedChannel() );
				doLayout( true );
			}
		}
		break;
	case ID_CHANNEL_SETNUMBEROFCHANNELS:
		{
			MBeatBoxDialog dialog;
			dialog.setCurrentChannelCount( ivPtBeatBox->getWavePlayerCount() );
			dialog.create( MRect( 0, 0, 300, 200 ), ((MTopWnd*)this->getTopParent()) );
			if( dialog.doModal() == MDialog::DlgResult::OK )
			{
				int channelCount = dialog.getCurrentChannelCount();
				TRACE( "<New channelcount: %i>\n", channelCount );

				ivPtBeatBox->setWavePlayerCount(channelCount);
				doLayout( true );
			}
		}
		break;
	case ID_SAMPLEBANK_CLOSEWINDOW:
		{
			delete this;
		}
		break;
	}
}
