#include "MMenu.h"
#include "../MApp.h"
#include "../win32impl/MMenuWin32Impl.h"

/** constructor */
MMenu::MMenu() :
	ivImpl( 0 )
{
	ivImpl = new MMenuWin32Impl();
}

/** destructor */
MMenu::~MMenu()
{
	SAFE_DELETE( ivImpl );
}

/** loads the menu from native resource bundle */
bool MMenu::load( unsigned int id )
{
	return ivImpl->load( id );
}

/** returns the native impl */
MMenuImpl* MMenu::getImpl()
{
	return ivImpl;
}