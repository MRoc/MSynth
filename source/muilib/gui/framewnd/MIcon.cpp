#include "MIcon.h"
#include "../win32impl/MIconWin32Impl.h"

/** default constructor */
MIcon::MIcon() :
	ivImpl( 0 )
{
	ivImpl = new MIconWin32Impl();
}

/** default constructor */
MIcon::MIcon( const MIcon& icon ) :
	ivImpl( 0 )
{
	ivImpl = icon.ivImpl;
	icon.ivImpl->incRef();
}

/** constructor */
MIcon::MIcon( unsigned int id ) :
	ivImpl( 0 )
{
	ivImpl = new MIconWin32Impl( id );
}

/** destructor */
MIcon::~MIcon()
{
	if( ivImpl && ivImpl->decRef() )
	{
		delete ivImpl;
		ivImpl = 0;
	}
}

/** loads a icon from native resource bundle */
bool MIcon::load( unsigned int id )
{
	return ivImpl->load( id );
}

/** returns the native impl */
MIconImpl* MIcon::getImpl()
{
	return ivImpl;
}