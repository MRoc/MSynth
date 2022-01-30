#include "MAttributeRenamer.h"

/**
 * runtime type info
 */
MRtti MAttributeRenamer::gvRtti = MRtti( "MAttributeRenamer", MAttributeRenamer::createInstance );

/**
 * constructor
 */
MAttributeRenamer::MAttributeRenamer()
{
}

/**
 * destructor
 */
MAttributeRenamer::~MAttributeRenamer()
{
	oldNames.clear(); 
}

/**
* returns the new attribute name
*/
String MAttributeRenamer::getAttributeName()
{
	return attrName;
}

/**
* deletes this instance
*/
void MAttributeRenamer::release()
{
	delete this;
}

/**
* does the renaming of a node, only renames one attribute.
* iterates the old attribute names and searches in the node
* if one is found. If it is found, the attribute name is replaced
* by the new name. After renaming one attribute, the function returns,
* or if no old one found, after renaming nothing.
*/
void MAttributeRenamer::convert( MTreeNode* nodeToBeConverted )
{
	for( unsigned int j=0;j<getOldNameCount();j++ )
	{
		String attribute = getOldName( j );
		if( nodeToBeConverted->hasAttribute( attribute ) )
		{
			String attrValue = nodeToBeConverted->getAttribute( attribute );
			nodeToBeConverted->removeAttribute( attribute );
			nodeToBeConverted->setAttribute( attrName, attrValue );
			break;
		}
	}
}

/**
* loads the attribute renamer
*/
void MAttributeRenamer::load( MTreeNode* node )
{
	ASSERT( node->getName() == MObject::ELEM_OBJECT );
	ASSERT( node->getAttribute( MObject::ATTR_CLASS ) == getRtti()->getClassName() );

	attrName = node->getAttribute( "name" );
	MTreeNodeIterator iter = node->getIterator();
	while( iter.hasNext() )
	{
		MTreeNode* child = iter.nextTreeNode();
		addOldName( child->getAttribute( "name" ) );
	}
}

/**
 * saving
 */
MTreeNode* MAttributeRenamer::save()
{
	MTreeNode* ptBack = new MTreeNode( MObject::ELEM_OBJECT );
	ptBack->setAttribute( MObject::ATTR_CLASS, getRtti()->getClassName() );
	ptBack->setAttribute( "name", attrName );
    
	for( unsigned int j=0;j<getOldNameCount();j++ )
	{
		MTreeNode* ptOldNode = new MTreeNode( "old" );
		ptOldNode->setAttribute( "name", getOldName( j ) );
		ptBack->addChild( ptOldNode );
	}

	return ptBack;
}

/**
* returns the old name at the specified index
*/
String MAttributeRenamer::getOldName( unsigned int index )
{
	return oldNames[ index ];
}

/**
* returns the number of old attribute names
*/
unsigned int MAttributeRenamer::getOldNameCount()
{
	return oldNames.size();
}

/**
* adds a old attribute name
*/
void MAttributeRenamer::addOldName( String oldName )
{
	oldNames.push_back( oldName );
}

/**
 * returns the runtime type info
 */
IRtti* MAttributeRenamer::getRtti() const
{
	return &gvRtti;
}

/**
 * query interface
 */
void* MAttributeRenamer::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MAttributeRenamer*)this);
	else if( className == "INodeConverter" )
		return (void*) ((INodeConverter*)this);
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else
		return MObject::getInterface( className );
}

/**
 * creates an instance of this class
 */
MObject* MAttributeRenamer::createInstance()
{
	return new MAttributeRenamer();
}