#include "MIconWin32Impl.h"
#include "../MApp.h"
#include "MAppWin32Impl.h"

/** constructor */
MIconWin32Impl::MIconWin32Impl() :
	ivIconHandle( 0 ),
	ivRefCount( 1 )
{
}


/** constructor */
MIconWin32Impl::MIconWin32Impl( unsigned int id ) :
	ivIconHandle( 0 ),
	ivRefCount( 1 )
{
	load( id );
}

/** destructor */
MIconWin32Impl::~MIconWin32Impl()
{
	ASSERT( ivRefCount == 0 );
}

/** returns the native windows icon handle */
HICON MIconWin32Impl::getIcon()
{
	return ivIconHandle;
}

/** loads a icon from native resource bundle */
bool MIconWin32Impl::load( unsigned int id )
{
	ivIconHandle =
		LoadIcon(
			((MAppWin32Impl*)MApp::getInstance()->getImpl())->getHandle(),
			MAKEINTRESOURCE( id ) );
	return ivIconHandle != 0;
}

/** increment reference counter */
void MIconWin32Impl::incRef()
{
	ivRefCount++;
}

/** decrement reference counter, if zero returns true */
bool MIconWin32Impl::decRef()
{
	ASSERT( ivRefCount > 0 );
	return ((--ivRefCount) == 0);
}