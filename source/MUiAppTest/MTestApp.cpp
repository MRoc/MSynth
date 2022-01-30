#include "MTestApp.h"
#include <framework/io/MIoException.h>
#include <framework/io/MLogger.h>
#include <framework/resource/MResourceManager.h>
#include <framework/treedocument/MTreeDocumentUtils.h>

MTestApp::MTestApp() :
	ivPtWindow( 0 )
{
}

MTestApp::~MTestApp()
{
}

MApp* createApp()
{
	return new MTestApp();
}


bool MTestApp::onInit()
{
#ifndef _DEBUG
	String appDir = getAppDir();
#else
	String appDir = "D:/code/msynth/msynth/";
#endif
	appDir.Replace( "\\", "/" );

	try
	{
		MResourceManager::getInstance()->loadArchive( appDir, "app" );
		MResourceManager::getInstance()->loadArchive( appDir + "resource", "resource" );
	}
	catch( MIoException ae )
	{
		MLogger::logError( ae.getExceptionDescripton() );
		return false;
	}

	ivPtWindow = new MTestWindow();
	ivPtWindow->create( MRect( 10,10, 640, 480 ), 0 );
	ivPtWindow->setText( "MRoc widget systems" );

	return true;
}

/** invoked when app exists */
bool MTestApp::onExit()
{
	try
	{
		MResourceManager::release();
	}
	catch( MIoException ae )
	{
		MLogger::logWarning( ae.getExceptionDescripton() );
		return false;
	}
	return true;
}
