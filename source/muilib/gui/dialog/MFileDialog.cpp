#include "MFileDialog.h"
#include "cderr.h"
#include <framework/io/MLogger.h>
#include "../MApp.h"
#include "../win32impl/MAppWin32Impl.h"
#include "../win32impl/MTopWndWin32Impl.h"

MFileDialog::MFileDialog()
{
	memset( &ivDlgStruc, 0, sizeof(OPENFILENAME ) );
	ivDlgStruc.lStructSize = sizeof(OPENFILENAME);
}

MFileDialog::~MFileDialog()
{
}

bool MFileDialog::showOpenDialog( MTopWnd* parent, String title, char* filter )
{
	ASSERT( parent );
	ASSERT( filter );
	ASSERT( MApp::getInstance() );
	ASSERT( ((MAppWin32Impl*)MApp::getInstance()->getImpl())->getHandle() );

	ivBuffer[ 0 ] = 0;

	memset( &ivDlgStruc, 0, sizeof(OPENFILENAME) );
	ivDlgStruc.lStructSize = sizeof(OPENFILENAME); 
	ivDlgStruc.hwndOwner = ((MTopWndWin32Impl*)parent->getImpl())->getHandle();
	ivDlgStruc.hInstance = ((MAppWin32Impl*)MApp::getInstance()->getImpl())->getHandle();
	ivDlgStruc.lpstrFile = ivBuffer;
	ivDlgStruc.nMaxFile = MAX_PATH;
	ivDlgStruc.lpstrTitle = title;
	ivDlgStruc.lpstrFilter = filter;
	
	BOOL back = GetOpenFileName( &ivDlgStruc );
	if( back == FALSE )
	{
		switch( CommDlgExtendedError() )
		{
		case CDERR_DIALOGFAILURE: MLogger::logError( "CDERR_DIALOGFAILURE" ); break;
		case CDERR_FINDRESFAILURE: MLogger::logError( "CDERR_FINDRESFAILURE" ); break;
		case CDERR_NOHINSTANCE: MLogger::logError( "CDERR_NOHINSTANCE" ); break;
		case CDERR_INITIALIZATION: MLogger::logError( "CDERR_INITIALIZATION" ); break;
		case CDERR_NOHOOK: MLogger::logError( "CDERR_NOHOOK" ); break;
		case CDERR_LOCKRESFAILURE: MLogger::logError( "CDERR_LOCKRESFAILURE" ); break;
		case CDERR_NOTEMPLATE: MLogger::logError( "CDERR_NOTEMPLATE" ); break;
		case CDERR_LOADRESFAILURE: MLogger::logError( "CDERR_LOADRESFAILURE" ); break;
		case CDERR_STRUCTSIZE: MLogger::logError( "CDERR_STRUCTSIZE" ); break;
		case CDERR_LOADSTRFAILURE: MLogger::logError( "CDERR_LOADSTRFAILURE" ); break;
		case FNERR_BUFFERTOOSMALL: MLogger::logError( "FNERR_BUFFERTOOSMALL" ); break;
		case CDERR_MEMALLOCFAILURE: MLogger::logError( "CDERR_MEMALLOCFAILURE" ); break;
		case FNERR_INVALIDFILENAME: MLogger::logError( "FNERR_INVALIDFILENAME" ); break;
		case CDERR_MEMLOCKFAILURE: MLogger::logError( "CDERR_MEMLOCKFAILURE" ); break;
		case FNERR_SUBCLASSFAILURE: MLogger::logError( "FNERR_SUBCLASSFAILURE" ); break;
		default: break;
		}
	}
	return back == TRUE;
}

String MFileDialog::getSelectedFile()
{
	return String( ivBuffer );
}

bool MFileDialog::showSaveDialog( MTopWnd* parent, String title, char* filter )
{
	ASSERT( parent );
	ASSERT( filter );
	ASSERT( MApp::getInstance() );
	ASSERT( ((MAppWin32Impl*)MApp::getInstance()->getImpl())->getHandle() );

	ivBuffer[ 0 ] = 0;

	memset( &ivDlgStruc, 0, sizeof(OPENFILENAME) );
	ivDlgStruc.lStructSize = sizeof(OPENFILENAME); 
	ivDlgStruc.hwndOwner = ((MTopWndWin32Impl*)parent->getImpl())->getHandle();
	ivDlgStruc.hInstance = ((MAppWin32Impl*)MApp::getInstance()->getImpl())->getHandle();
	ivDlgStruc.lpstrFile = ivBuffer;
	ivDlgStruc.nMaxFile = MAX_PATH;
	ivDlgStruc.lpstrTitle = title;
	ivDlgStruc.lpstrFilter = filter;
	
	BOOL back = GetSaveFileName( &ivDlgStruc );
	if( back == FALSE )
	{
		switch( CommDlgExtendedError() )
		{
		case CDERR_DIALOGFAILURE: MLogger::logError( "CDERR_DIALOGFAILURE" ); break;
		case CDERR_FINDRESFAILURE: MLogger::logError( "CDERR_FINDRESFAILURE" ); break;
		case CDERR_NOHINSTANCE: MLogger::logError( "CDERR_NOHINSTANCE" ); break;
		case CDERR_INITIALIZATION: MLogger::logError( "CDERR_INITIALIZATION" ); break;
		case CDERR_NOHOOK: MLogger::logError( "CDERR_NOHOOK" ); break;
		case CDERR_LOCKRESFAILURE: MLogger::logError( "CDERR_LOCKRESFAILURE" ); break;
		case CDERR_NOTEMPLATE: MLogger::logError( "CDERR_NOTEMPLATE" ); break;
		case CDERR_LOADRESFAILURE: MLogger::logError( "CDERR_LOADRESFAILURE" ); break;
		case CDERR_STRUCTSIZE: MLogger::logError( "CDERR_STRUCTSIZE" ); break;
		case CDERR_LOADSTRFAILURE: MLogger::logError( "CDERR_LOADSTRFAILURE" ); break;
		case FNERR_BUFFERTOOSMALL: MLogger::logError( "FNERR_BUFFERTOOSMALL" ); break;
		case CDERR_MEMALLOCFAILURE: MLogger::logError( "CDERR_MEMALLOCFAILURE" ); break;
		case FNERR_INVALIDFILENAME: MLogger::logError( "FNERR_INVALIDFILENAME" ); break;
		case CDERR_MEMLOCKFAILURE: MLogger::logError( "CDERR_MEMLOCKFAILURE" ); break;
		case FNERR_SUBCLASSFAILURE: MLogger::logError( "FNERR_SUBCLASSFAILURE" ); break;
		default: break;
		}
	}
	return back == TRUE;
}

String MFileDialog::getErrorText( DWORD err )
{
	String errText;
	switch( err )
	{
	case CDERR_DIALOGFAILURE: errText = "CDERR_DIALOGFAILURE"; break;
	case CDERR_FINDRESFAILURE: errText = "CDERR_FINDRESFAILURE"; break;
	case CDERR_NOHINSTANCE: errText = "CDERR_NOHINSTANCE"; break;
	case CDERR_INITIALIZATION: errText = "CDERR_INITIALIZATION"; break;
	case CDERR_NOHOOK: errText = "CDERR_NOHOOK"; break;
	case CDERR_LOCKRESFAILURE: errText = "CDERR_LOCKRESFAILURE"; break;
	case CDERR_NOTEMPLATE: errText = "CDERR_NOTEMPLATE"; break;
	case CDERR_LOADRESFAILURE: errText = "CDERR_LOADRESFAILURE"; break;
	case CDERR_STRUCTSIZE: errText = "CDERR_STRUCTSIZE"; break;
	case CDERR_LOADSTRFAILURE: errText = "CDERR_LOADSTRFAILURE"; break;
	case FNERR_BUFFERTOOSMALL: errText = "FNERR_BUFFERTOOSMALL"; break;
	case CDERR_MEMALLOCFAILURE: errText = "CDERR_MEMALLOCFAILURE"; break;
	case FNERR_INVALIDFILENAME: errText = "FNERR_INVALIDFILENAME"; break;
	case CDERR_MEMLOCKFAILURE: errText = "CDERR_MEMLOCKFAILURE"; break;
	case FNERR_SUBCLASSFAILURE: errText = "FNERR_SUBCLASSFAILURE"; break;
	default: errText = "UNKNOWN"; break;
	}
	return errText;
}