#include "MMessageBox.h"
#include "../MApp.h"
#include <framework/MNativeTypes.h>
#include <framework/resource/text/MTextResource.h>

void MMessageBox::showError( String msg )
{
	MessageBox(
		0,
		MTextResource::getInstance()->getString( msg ),
		MApp::getInstance()->getAppName(),
		MB_OK|MB_ICONERROR );
}

void MMessageBox::showError( String strId, String subString )
{
	MessageBox(
		0,
		MTextResource::getInstance()->getString( strId, LPCTSTR( subString ) ),
		MApp::getInstance()->getAppName(),
		MB_OK|MB_ICONERROR );
}

void MMessageBox::showError( String id, String subString1, String subString2 )
{
	MessageBox(
		0,
		MTextResource::getInstance()->getString( id, LPCTSTR( subString1 ), LPCTSTR( subString2 ) ),
		MApp::getInstance()->getAppName(),
		MB_OK|MB_ICONERROR );
}

/*int MMessageBox::showQuestion( CWnd* ptParent, String errorMessage )
{
	return MessageBox(
		ptParent->m_hWnd,
		errorMessage,
		APPNAME,
		MB_YESNO|MB_ICONQUESTION );
}*/

int MMessageBox::showQuestion( String id, String subString )
{
	return
		MessageBox(
			0,
			MTextResource::getInstance()->getString( id, LPCTSTR( subString ) ),
			MApp::getInstance()->getAppName(),
			MB_YESNO|MB_ICONQUESTION );
}

int MMessageBox::showQuestion( String id )
{
	return
		MessageBox(
			0,
			MTextResource::getInstance()->getString( id ),
			MApp::getInstance()->getAppName(),
			MB_YESNO|MB_ICONQUESTION );
}
