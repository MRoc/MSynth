#include "MAttributeContentReplacer.h"

/**
 * runtime type info
 */
MRtti MAttributeContentReplacer::gvRtti = MRtti( "MAttributeContentReplacer", MAttributeContentReplacer::createInstance );

/**
 * constructor
 */
MAttributeContentReplacer::MAttributeContentReplacer()
{
}

/**
 * destructor
 */
MAttributeContentReplacer::~MAttributeContentReplacer()
{
	this->ivAttributeNames.clear();
}

/**
 * converts the given node
 */
void MAttributeContentReplacer::convert( MTreeNode* ptNode )
{
	unsigned int count = (unsigned int) this->ivAttributeNames.size();
	for( unsigned int i=0;i<count;i++ )
	{
		String attrName = this->ivAttributeNames[ i ];
		if( ptNode->hasAttribute( attrName ) )
			ptNode->setAttribute( attrName, this->ivAttributeContent );
	}
}

/**
 * queries for a interface
 */
void* MAttributeContentReplacer::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MAttributeContentReplacer*)this);
	else if( className == "INodeConverter" )
		return (void*) ((INodeConverter*)this);
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else
		return MObject::getInterface( className );
}

/**
 * returns the runtime info
 */
IRtti* MAttributeContentReplacer::getRtti() const
{
	return &gvRtti;
}

/**
 * loading
 */
void MAttributeContentReplacer::load( MTreeNode* ptNode )
{
	ASSERT( ptNode->getName() == MObject::ELEM_OBJECT );
	ASSERT( ptNode->getAttribute( MObject::ATTR_CLASS ) == getRtti()->getClassName() );

	this->ivAttributeContent = ptNode->getAttribute( "content" );

	MTreeNodeIterator iter = ptNode->getIterator();
	while( iter.hasNext() )
	{
		MTreeNode* ptChild = iter.nextTreeNode();
		ivAttributeNames.push_back( ptChild->getAttribute( "name" ) );
	}
}

/**
 * storing
 */
MTreeNode* MAttributeContentReplacer::save()
{
	MTreeNode* ptBack = new MTreeNode( MObject::ELEM_OBJECT );
	ptBack->setAttribute( MObject::ATTR_CLASS, getRtti()->getClassName() );
	ptBack->setAttribute( "content", this->ivAttributeContent );

	unsigned int count = (unsigned int) this->ivAttributeNames.size();
	for( unsigned int i=0;i<count;i++ )
	{
		MTreeNode* ptAttrNode = new MTreeNode( "attribute" );
		ptAttrNode->setAttribute( "name", this->ivAttributeNames[ i ] );
		ptBack->addChild( ptAttrNode );
	}
	return ptBack;
}

/**
 * creates a instance of this class
 */
MObject* MAttributeContentReplacer::createInstance()
{
	return new MAttributeContentReplacer();
}
