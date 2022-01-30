#include "MAboutDialog.h"
#include <framework/treedocument/MSaxTreeDocument.h>

MAboutDialog::MAboutDialog() :
	MDialog()
{
	MSaxTreeDocument doc;
	doc.parseResource( "resource/xml/aboutdialog.xml" );
	load( doc.getRoot() );
	create( MRect( 0, 0, 310, 300 ), 0 );
}

MAboutDialog::~MAboutDialog()
{
}

void MAboutDialog::onInit()
{
}

MDialog::DlgResult MAboutDialog::onDlgCommand( unsigned int id )
{
	/*switch( id )
	{
	case IDOK:
		return DlgResult::OK;
		break;
	case IDC_STATIC_LINK:
		return DlgResult::NEUTRAL;
		break;
	default:
		return MDialog::onDlgCommand( id );
		break;
	}*/
	return DlgResult::OK;
}

void MAboutDialog::onDestroy()
{
}


