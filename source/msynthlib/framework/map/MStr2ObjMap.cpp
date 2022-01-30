#include "MStr2ObjMap.h"

#include "../MDefaultFactory.h"
#include "../MException.h"

/**
 * runtime support
 */
MRtti MStr2ObjMap::gvRtti = MRtti( "MStr2ObjMap", MStr2ObjMap::createInstance );

/**
 * constructor
 */
MStr2ObjMap::MStr2ObjMap()
{
}

/**
 * destructor deleting all stored objects
 */
MStr2ObjMap::~MStr2ObjMap()
{
	deleteAll();
}

/**
 * stores a MObject under the specified key
 */
void MStr2ObjMap::store( String key, MObject* value )
{
	ASSERT( ivMap.find( key ) == ivMap.end() );
	ivMap[ key ] = value;
}

/**
 * returns the MObject stored under the key
 */
MObject* MStr2ObjMap::get( String key )
{
	MObject* back = 0;
	if( ivMap.find( key ) != ivMap.end() )
		back = (MObject*) ivMap.find( key )->second;
	return back;
}

/**
 * removes the MObject specified by key
 */
MObject* MStr2ObjMap::remove( String key )
{
	ASSERT( contains( key ) );

	MObject* back = 0;
	Str2ObjMapIter i = ivMap.find( key );
	if( i != ivMap.end() )
	{
		back = (MObject*) i->second;
		ivMap.erase( i );
	}
	return back;
}

/**
 * returns true if the key is found
 */
bool MStr2ObjMap::contains( String key )
{
	return ivMap.find( key ) != ivMap.end();
}

/**
 * deletes all stored objects
 */
void MStr2ObjMap::deleteAll()
{
	for(
		Str2ObjMapIter iter=ivMap.begin();
		iter!=ivMap.end();
		iter++ )
	{
		delete (iter)->second;
	}
	clear();
}

/**
 * clears the map
 */
void MStr2ObjMap::clear()
{
	ivMap.clear();
}

/**
 * returns the class name for runtime purpose
 */
IRtti* MStr2ObjMap::getRtti() const
{
	return &gvRtti;
}

/**
 * querries for a specified interface
 */
void* MStr2ObjMap::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MStr2ObjMap*)this);
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else
		return MObject::getInterface( className );
}

/**
 * stores a treenode
 */
MTreeNode* MStr2ObjMap::save()
{
	MTreeNode* ptBack = new MTreeNode( ELEM_OBJECT );
	ptBack->setAttribute( ATTR_CLASS, getRtti()->getClassName() );

	for(
		Str2ObjMapIter iter=ivMap.begin();
		iter!=ivMap.end();
		iter++ )
	{
		MTreeNode* ptItemNode = new MTreeNode( "item" );
		ptItemNode->setAttribute( "key", (iter)->first );
		MObject* ptNext = (iter)->second;
		ISerializeable* ptSer = (ISerializeable*) ptNext->getInterface( "ISerializeable" );
		if( ptSer )
			ptItemNode->addChild( ptSer->save() );
		else
		{
			delete ptItemNode;
			throw MException( "Could not query interface ISerializeable in MStr2ObjMap!" );
		}
		ptBack->addChild( ptItemNode );
	}

	return ptBack;
}

/**
 * loads from treenode
 */
void MStr2ObjMap::load( MTreeNode* parentNode )
{
	deleteAll();

	unsigned int count = parentNode->getChildCount();
	for( unsigned int i=0;i<count;i++ )
	{
		MTreeLeaf* pChild = parentNode->getChild( i );
		if( pChild->getType() == MTreeLeaf::NODE )
		{
			MTreeNode* node = (MTreeNode*)pChild;
			String nodeName = node->getName();
			if( strcmp(nodeName, "item") == 0 )
			{
				unsigned int count2 = node->getChildCount();
				for( unsigned int j=0;j<count2;j++ )
				{
					MTreeLeaf* objItemLeaf = node->getChild( j );
					if( objItemLeaf->getType() == MTreeLeaf::NODE )
					{
						MTreeNode* objItem = (MTreeNode*) objItemLeaf;
						String nodeName = objItem->getName();
						if( strcmp(nodeName, MObject::ELEM_OBJECT) == 0 )
						{
							MObject* newObject =
								MDefaultFactory::getInstance()->createObject( objItem );
							if( newObject != 0 )
							{
								String key = node->getAttribute( "key" );
								store( key, newObject );
							}
							else
								ASSERT( false );
						}
					}
				}
			}
		}
	}
}

/**
 * create an instance of this class
 */
MObject* MStr2ObjMap::createInstance()
{
	return new MStr2ObjMap();
}