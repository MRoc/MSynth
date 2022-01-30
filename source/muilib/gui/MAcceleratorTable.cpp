#include "MAcceleratorTable.h"
#include <gui/MApp.h>
#include <framework/io/MLogger.h>
#include "win32impl/MAppWin32Impl.h"

MAcceleratorTable::MAcceleratorTable() :
	ivAccl( 0 )
{
}

MAcceleratorTable::~MAcceleratorTable()
{
	unload();
}

bool MAcceleratorTable::load( unsigned int resourceId )
{
	ivAccl =
		LoadAccelerators(
			((MAppWin32Impl*)MApp::getInstance()->getImpl())->getHandle(),
			MAKEINTRESOURCE( resourceId )
		);
	if( ivAccl == 0 )
		MLogger::logError( "MAcceleratorTable::load: LoadAccelerators failed" );

	return ivAccl != 0;
}

void MAcceleratorTable::unload()
{
	if( ivAccl )
	{
		if( DestroyAcceleratorTable( ivAccl ) != TRUE )
			MLogger::logError( "MAcceleratorTable::~MAcceleratorTable: DestroyAcceleratorTable failed" );
		ivAccl = 0;
	}
}

bool MAcceleratorTable::isLoaded()
{
	return ivAccl != 0;
}

HACCEL MAcceleratorTable::getHandle()
{
	return ivAccl;
}