#include "MTextResource.h"

#include <stdio.h>
#include <stdarg.h>

/**
 * runtime type info
 */
MRtti MTextResource::gvRtti = MRtti( "MTextResource", 0 );

/**
 * the singleton instance
 */
 MTextResource* MTextResource::gvInstance = 0;

/**
 * constructor
 */
MTextResource::MTextResource()
{
}

/**
 * destructor
 */
MTextResource::~MTextResource()
{
	ivStringTable.clear();
}

/**
 * return the runtime type info
 */
IRtti* MTextResource::getRtti() const
{
	return &gvRtti;
}

/**
 * query interface
 */
void* MTextResource::getInterface( const String &className ) const
{
	if( className == "MTextResource" )
		return (void*) ((MTextResource*)this);
	else if( className == "ITextResource" )
		return (void*) ((ITextResource*)this);
	else
		return MObject::getInterface( className );
}

/**
 * translates the given string and inserts the given values
 */
String MTextResource::getString( String id, ... )
{
	if( ivStringTable.find( id ) == ivStringTable.end() )
		return id;

	String format = ivStringTable.find( id )->second;

	va_list l;
	va_start( l, id );

	char buffer[ 4096 ];
	vsprintf(
		buffer,
		format,
		l ); 

	va_end( l );

	return String( buffer );
}

/**
 * loads the text resource from xml
 */
void MTextResource::load( String resourceName )
{
	MSaxTreeDocument doc;
	doc.parseResource( resourceName );
	if( doc.getRoot() )
		load( doc.getRoot() );
}

/**
 * loads the string table from tree
 */
void MTextResource::load( MTreeNode* pNode )
{
	unsigned int childCount = pNode->getChildCount();
	for( unsigned int i=0;i<childCount;i++ )
	{
		MTreeLeaf* pChild = pNode->getChild( i );
		if( pChild->getType() == MTreeLeaf::NODE )
		{
			MTreeNode* pChildNode = (MTreeNode*)pChild;
			String id = pChildNode->getAttribute( "id" );
			String text = pChildNode->getAttribute( "text" );
			text.Replace( "\\n", "\n" );
			ivStringTable.insert( StrStrMap::value_type( id, text ) );
		}
	}
}

/**
 * returns the singleton instance
 */
MTextResource* MTextResource::getInstance()
{
	if( gvInstance == 0 )
		gvInstance = new MTextResource();
	return gvInstance;
}

/**
 * frees the singleton instance
 */
void MTextResource::release()
{
	if( gvInstance )
	{
		delete gvInstance;
		gvInstance = 0;
	}
}