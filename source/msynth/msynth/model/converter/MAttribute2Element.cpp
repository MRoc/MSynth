#include "mattribute2element.h"

/**
 * runtime type info
 */
MRtti MAttribute2Element::gvRtti = MRtti( "MAttribute2Element", MAttribute2Element::createInstance );
 
/**
 * constructor
 */
MAttribute2Element::MAttribute2Element()
{
}

/**
 * destructor
 */
MAttribute2Element::~MAttribute2Element()
{
	this->ivExcludeAttributes.clear();
}

/**
 * converts the given node
 */
void MAttribute2Element::convert( MTreeNode* ptNode )
{
	for( unsigned int i=0;i<ptNode->getAttributeCount();i++ )
	{
		String attributeName = ptNode->getAttributeName( i );
		if( ! this->excludeAttribute( attributeName ) )
		{
			String attributeValue = ptNode->getAttribute( attributeName );
			MTreeNode* ptNewChild = new MTreeNode( this->getNewElementName() );
			ptNewChild->setAttribute( this->getNewAttributeOldName(), attributeName );
			ptNewChild->setAttribute( this->getNewAttributeOldValue(), attributeValue );

			ptNode->removeAttribute( attributeName );
			ptNode->addChild( ptNewChild );
			i--;
		}
	}
}

/**
 * returns the name of the new element
 */
String MAttribute2Element::getNewElementName()
{
	return this->ivNewElementName;
}

/**
 * returns name of the new attribute that will store the old value
 */
String MAttribute2Element::getNewAttributeOldValue()
{
	return this->ivNewAttributeOldValue;
}

/**
 * returns the name of the new attribute that will store the old attribute name
 */
String MAttribute2Element::getNewAttributeOldName()
{
	return this->ivNewAttributeOldName;
}

/**
 * returns the number of attributes to be excluded
 */
unsigned int MAttribute2Element::getExcludeCount()
{
	return (unsigned int) this->ivExcludeAttributes.size();
}

/**
 * returns the attribute that should be excluded at the specified index
 */
String MAttribute2Element::getExcludeAttribute( unsigned int index )
{
	ASSERT( index < getExcludeCount() );
	return this->ivExcludeAttributes[ index ];
}

/**
 * returns true if the given attribute name should be excluded
 */
bool MAttribute2Element::excludeAttribute( String attributeName )
{
	bool back = false;
	unsigned int count = ivExcludeAttributes.size();
	for( unsigned int i=0;i<count;i++ )
		if( ivExcludeAttributes[ i ] == attributeName )
		{
			back = true;
			break;
		}
	return back;
}

/**
 * loading
 */
void MAttribute2Element::load( MTreeNode* node )
{
	ASSERT( node->getName() == MObject::ELEM_OBJECT );
	ASSERT( node->getAttribute( MObject::ATTR_CLASS ) == getRtti()->getClassName() );

	this->ivNewElementName = node->getAttribute( "newelementname" );
	this->ivNewAttributeOldName = node->getAttribute( "newattributeoldname" );
	this->ivNewAttributeOldValue = node->getAttribute( "newattributeoldvalue" );

	MTreeNodeIterator iter = node->getIterator();
	while( iter.hasNext() )
	{
		MTreeNode* ptChild = iter.nextTreeNode();
		this->ivExcludeAttributes.push_back( ptChild->getAttribute( "name" ) );
	}
}

/**
 * saving
 */
MTreeNode* MAttribute2Element::save()
{
	MTreeNode* ptBack = new MTreeNode( MObject::ELEM_OBJECT );
	ptBack->setAttribute( MObject::ATTR_CLASS, getRtti()->getClassName() );

	ptBack->setAttribute( "newelementname", this->getNewElementName() );
	ptBack->setAttribute( "newattributeoldname", this->getNewAttributeOldName() );
	ptBack->setAttribute( "newattributeoldvalue", this->getNewAttributeOldValue() );

	unsigned int count = this->getExcludeCount();
	for( unsigned int i=0;i<count;i++ )
	{
		MTreeNode* ptNode = new MTreeNode( "exclude" );
		ptNode->setAttribute( "name", this->getExcludeAttribute( i ) );
		ptBack->addChild( ptNode );
	}
	return ptBack;
}

/**
 * returns the runtime type info
 */
IRtti* MAttribute2Element::getRtti() const
{
	return &gvRtti;
}

/**
 * query for a interface
 */
void* MAttribute2Element::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MAttribute2Element*)this);
	else if( className == "INodeConverter" )
		return (void*) ((INodeConverter*)this);
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else
		return MObject::getInterface( className );
}

/**
 * creates a new instance of this class
 */
MObject* MAttribute2Element::createInstance()
{
	return new MAttribute2Element();
}

