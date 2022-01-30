#include "MDefaultFactory.h"
#include "primitive/MObjString.h"
#include "primitive/MBool.h"
#include "primitive/MInt.h"
#include "primitive/MPoint.h"
#include "primitive/MRect.h"

MDefaultFactory* MDefaultFactory::gvInstance = 0;

MDefaultFactory::MDefaultFactory()
{
	store( &(MObjString::gvRtti) );
	store( &(MInt::gvRtti) );
	store( &(MBool::gvRtti) );
	store( &(MRect::gvRtti) );
	store( &(MPoint::gvRtti) );
}

MDefaultFactory::~MDefaultFactory()
{
}

MDefaultFactory* MDefaultFactory::getInstance()
{
	if( gvInstance == 0 )
		gvInstance = new MDefaultFactory();
	return gvInstance;
};

void MDefaultFactory::release()
{
	if( gvInstance )
	{
		delete gvInstance;
		gvInstance = 0;
	}
}