#include "MMenuWin32Impl.h"
#include "MAppWin32Impl.h"
#include "../MApp.h"

/** constructor */
MMenuWin32Impl::MMenuWin32Impl() :
	ivMenu( 0 )
{
}

/** destructor */
MMenuWin32Impl::~MMenuWin32Impl()
{
	if( ivMenu )
		DestroyMenu( ivMenu );
}

/** loads a menu from native resource bundle */
bool MMenuWin32Impl::load( unsigned int id )
{
	if( ivMenu )
		DestroyMenu( ivMenu );
	ivMenu =
		LoadMenu(
			((MAppWin32Impl*)MApp::getInstance()->getImpl())->getHandle(),
			MAKEINTRESOURCE( id ) );
	return ivMenu != 0;
}

/** returns the native win32 menu handle */
HMENU MMenuWin32Impl::getHandle()
{
	return ivMenu;
}