#include "MWndNamespace.h"
#include "MWndcollection.h"

/** constructor */
MWndNamespace::MWndNamespace( MWndCollection* pOwner ) :
	ivPtOwner( pOwner )
{
}

/** destructor */
MWndNamespace::~MWndNamespace()
{
	ivId2Wnd.clear();
	ivWnd2Id.clear();
	ivPtOwner = 0;
}

/** registers a object at the context */
void MWndNamespace::regObj( MObject* pObj, const String& id )
{
	ASSERT( pObj );
	ASSERT( id != "" );
	ASSERT( getObj( id ) == 0 );

	ivId2Wnd.insert( StrObjMap::value_type( id, pObj ) );
	ivWnd2Id.insert( ObjStrMap::value_type( pObj, id ) );
}

/** unregisters a object from the context */
void MWndNamespace::unregObj( MObject* pObj )
{
	ASSERT( pObj );

	StrObjMapIter i1 = ivId2Wnd.find( getId( pObj ) );
	if( i1 != ivId2Wnd.end() )
		ivId2Wnd.erase( ivId2Wnd.find( getId( pObj ) ) );

	ObjStrMapIter i2 = ivWnd2Id.find( pObj );
	if( i2 != ivWnd2Id.end() )
		ivWnd2Id.erase( ivWnd2Id.find( pObj ) );
}

/** returns a object from the context */
MObject* MWndNamespace::getObj( const String& id )
{
	ASSERT( id != "" );

	StrObjMapIter i = ivId2Wnd.find( id );
	if( i != ivId2Wnd.end() )
		return i->second;
	else
		return 0;
}

/** returns the id of a object */
String MWndNamespace::getId( MObject* pObj )
{
	ASSERT( pObj );

	ObjStrMapIter i = ivWnd2Id.find( pObj );
	if( i != ivWnd2Id.end() )
		return i->second;
	else
		return "";
}

/** returns a namespace for the given control if available */
MWndNamespace* MWndNamespace::getNamespace( MWnd* pControl )
{
	MWndNamespace* pBack = 0;
	MWndCollection* pt = (MWndCollection*) pControl->getParent();
	while( pt )
	{
		if( pt->getNamespace() )
		{
			pBack = pt->getNamespace();
			break;
		}
		pt = (MWndCollection*) pt->getParent();
	}
	return pBack;
}

/** registers a object in the next available namespace, returns false if failed */
bool MWndNamespace::regObjNS( MObject* pObj, MWnd* pControl, const String& id )
{
	MWndNamespace* pNameSpace = getNamespace( pControl );
	if( pNameSpace )
		pNameSpace->regObj( pObj, id );
	return pNameSpace != 0;
}

/** registers a object in the next available namespace, returns false if failed */
bool MWndNamespace::unregObjNS( MObject* pObj, MWnd* pControl )
{
	MWndNamespace* pNameSpace = getNamespace( pControl );
	if( pNameSpace )
		pNameSpace->unregObj( pObj );
	return pNameSpace != 0;
}

/** returns the object in the given namespace, if available */
MObject* MWndNamespace::getObjNS( MWndCollection* pCollection, const String& id )
{
	MObject* pBack = 0;
	MWndNamespace* pNS = pCollection->getNamespace();
	if( pNS )
		pBack = pNS->getObj( id );
	return pBack;
}

/** returns the object in the given namespace or in upper namespaces, if available */
MObject* MWndNamespace::getObjFullNS( MWndCollection* pCollection, const String& id )
{
	MObject* pBack = 0;
	while( pCollection && pBack == 0 )
	{
		MWndNamespace* pNS = pCollection->getNamespace();
		if( pNS )
			pBack = pNS->getObj( id );
		pCollection = (MWndCollection*) pCollection->getParent();
	}
	return pBack;
}