#include "MChildElementEnumeration.h"

/**
 * runtime support
 */
MRtti MChildElementEnumeration::gvRtti = MRtti( "MChildElementEnumeration", MChildElementEnumeration::createInstance );

/**
 * constructor
 */
MChildElementEnumeration::MChildElementEnumeration()
{
}

/**
 * destructor
 */
MChildElementEnumeration::~MChildElementEnumeration()
{
}

/**
 * enumerates child matchings and invokes 
 * the convert method for every element child
 * found in the treenode node. then the own
 * converters are invoked
 */
void MChildElementEnumeration::convert( MTreeNode* ptNode )
{
	unsigned int childCount = this->getChildCount();
	for( unsigned int i=0;i<childCount;i++ )
	{
		IXmlMatchingNode* ptChild = this->getChild( i );

		MTreeNodeIterator iter = ptNode->getIterator();
		while( iter.hasNext() )
			ptChild->convert( iter.nextTreeNode() );
	}

	// invoke own converters
	if( ivConverters.getLength() > 0 )
	{
		unsigned int count = ivConverters.getLength();
		for( unsigned int i=0;i<count;i++ )
		{
			MObject* ptConverter = ivConverters.get( i );
			INodeConverter* ptConv = (INodeConverter*) ptConverter->getInterface( "INodeConverter" );
			ptConv->convert( ptNode );
		}
	}
}

/**
 * returns allways true
 */
bool MChildElementEnumeration::match( MTreeNode* ptNode )
{
	return true;
}

/**
 * returns the runtime type info
 */
IRtti* MChildElementEnumeration::getRtti() const
{
	return &gvRtti;
}

/**
 * returns an instance of a MAttributeMatch
 */
MObject* MChildElementEnumeration::createInstance()
{
	return new MChildElementEnumeration();
}