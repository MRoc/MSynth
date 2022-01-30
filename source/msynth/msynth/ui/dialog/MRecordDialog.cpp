#include "MRecordDialog.h"

MRecordDialog::MRecordDialog()
{
	MSaxTreeDocument doc;
	doc.parseResource( "resource/xml/recorddialog.xml" );
	load( doc.getRoot() );

	ivMode = RENDER;
}

MRecordDialog::~MRecordDialog()
{
}

MRecordDialog::DlgResult MRecordDialog::onDlgCommand( unsigned int id )
{
	switch( id )
	{
	case IDC_RADIO1:
		{
			ivMode = RENDER;
			MToggleButton* lrToggle = (MToggleButton*) MWndNamespace::getObjNS( this, "left2right" );
			if( lrToggle )
				lrToggle->setValue( false );
		}
		break;
	case IDC_RADIO2:
		{
			ivMode = STREAMING;
			MToggleButton* lrSession = (MToggleButton*) MWndNamespace::getObjNS( this, "session" );
			if( lrSession )
				lrSession->setValue( true );
		}
		break;
	case IDOK: return MDialog::DlgResult::OK; break;
	case IDCANCEL: return MDialog::DlgResult::CANCEL; break;
	}
	return MDialog::DlgResult::CANCEL;
}

void MRecordDialog::onInit()
{
	MToggleButton* lrToggle = (MToggleButton*) MWndNamespace::getObjNS( this, "left2right" );
	if( lrToggle )
		lrToggle->setValue( true );
}

void MRecordDialog::onDestroy()
{
}