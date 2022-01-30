#include "MProgressDialog.h"

MProgressDialog::MProgressDialog( MRenderThread* ptThread ) :
	MDialog(),
	ivPtThread( ptThread ),
	ivPtProgress( 0 )
{
	MSaxTreeDocument doc;
	doc.parseResource( "resource/xml/progressdialog.xml" );
	load( doc.getRoot() );
}

MProgressDialog::~MProgressDialog()
{
}

void MProgressDialog::onInit()
{
	ivPtThread->setProgressCallback( (MProgressControl*) MWndNamespace::getObjNS( this, "progress" ) );
	ivPtThread->start();
}

MProgressDialog::DlgResult MProgressDialog::onDlgCommand( unsigned int id )
{
	switch( id )
	{
	case IDCANCEL:
		return CANCEL;
		break;
	}
	return CANCEL;//MDialog::onDlgCommand( id );
}

void MProgressDialog::onDestroy()
{
	ivPtThread->setProgressCallback( 0 );
	delete ivPtProgress;
}