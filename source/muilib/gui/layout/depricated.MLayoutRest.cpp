#include "MLayoutRest.h"

MRtti MLayoutRest::gvRtti = MRtti( "MLayoutRest", MLayoutRest::createInstance );

MLayoutRest::MLayoutRest() :
	ivType( BOTH )
{
}

MLayoutRest::~MLayoutRest()
{
}

IRtti* MLayoutRest::getRtti() const
{
	return &gvRtti;
}

void* MLayoutRest::getInterface( const String &className ) const
{
	if( className == "MLayoutRect" )
		return (void*) ((MLayoutRest*)this);
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else
		return MLayoutLeaf::getInterface( className );
}

MSize MLayoutRest::getSize( MyWnd* ptWnd, MSize maxSize )
{
	return maxSize;
}

void MLayoutRest::doLayout( MyWnd* ptWnd, MRect rect )
{
}

String MLayoutRest::type2str( Type type )
{
}

Type MLayoutRest::str2Type( String type )
{
}

MObject* MLayoutRest::createInstance()
{
	return new MLayoutRest();
}