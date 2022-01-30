#include "MInt2ObjMap.h"

#include "../MException.h"
#include "../primitive/MInt.h"

/**
 * runtime support
 */
MRtti MInt2ObjMap::gvRtti = MRtti( "MInt2ObjMap", MInt2ObjMap::createInstance );

/**
 * constructor
 */
MInt2ObjMap::MInt2ObjMap()
{
}

/**
 * destructor, deleting every stored item
 */
MInt2ObjMap::~MInt2ObjMap()
{
	deleteAll();
}

/**
 * stores a value under the specified key
 */
void MInt2ObjMap::store( int key, MObject* value )
{
	ASSERT( ivMap.find( key ) == ivMap.end() );
	ivMap[ key ] = value;
}

/**
 * returns the value at the specified key
 */
MObject* MInt2ObjMap::get( int key )
{
	MObject* back = 0;
	if( ivMap.find( key ) != ivMap.end() )
		back = (MObject*) ivMap.find( key )->second;
	return back;
}

/**
 * returns true if the key is found
 */
bool MInt2ObjMap::contains( int key )
{
	return ivMap.find( key ) != ivMap.end();
}

/**
 * deletes all stored objects
 */
void MInt2ObjMap::deleteAll()
{
	for(
		Int2ObjMapIter iter=ivMap.begin();
		iter!=ivMap.end();
		iter++ )
	{
		delete (iter)->second;
	}
	clear();
}

/**
 * clears all stored objects without deleting them
 */
void MInt2ObjMap::clear()
{
	ivMap.clear();
}

/**
 * returns a string representation
 */
String MInt2ObjMap::toString() const
{
	return getRtti()->getClassName();
}

/**
 * returns the runtime type information
 */
IRtti* MInt2ObjMap::getRtti() const
{
	return &gvRtti;
}

/**
 * querries for a specified interface
 */
void* MInt2ObjMap::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MInt2ObjMap*)this);
	if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else
		return MObject::getInterface( className );
}

/**
 * loads from the given tree node
 */
void MInt2ObjMap::load( MTreeNode* ptNode )
{
	ASSERT( false );
	ptNode = 0;
}

/**
 * stores as tree node
 */
MTreeNode* MInt2ObjMap::save()
{
	MTreeNode* ptBack = new MTreeNode( ELEM_OBJECT );
	ptBack->setAttribute( ATTR_CLASS, gvRtti.getClassName() );

	for(
		Int2ObjMapIter iter=ivMap.begin();
		iter!=ivMap.end();
		iter++ )
	{
		MTreeNode* ptKeyNode = new MTreeNode( "item" );
		ptKeyNode->setAttribute( "key", MInt::toString( (iter)->first ) );

		MObject* ptNext = (iter)->second;
		ISerializeable* ptSer = (ISerializeable*) ptNext->getInterface( "ISerializeable" );
		if( ptSer )
			ptKeyNode->addChild( ptSer->save() );
		else
		{
			delete ptKeyNode;
			throw MException( "Could not query interface ISerializeable in MStr2ObjMap!" );
		}

		ptBack->addChild( ptKeyNode );
	}

	return ptBack;
}

/**
 * creates an instance of this class
 */
MObject* MInt2ObjMap::createInstance()
{
	return new MInt2ObjMap();
}