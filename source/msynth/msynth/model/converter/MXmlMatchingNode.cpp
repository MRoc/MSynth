#include "mxmlmatchingnode.h"

/**
 * runtime support
 */
MRtti MXmlMatchingNode::gvRtti = MRtti( "MXmlMatchingNode", 0 );

/**
 * constructor
 */
MXmlMatchingNode::MXmlMatchingNode() :
	ivPtParent( 0 )
{
}

/**
 * destructor
 */
MXmlMatchingNode::~MXmlMatchingNode()
{
	ivConverters.deleteAll();
	ivChilds.deleteAll();
}

/**
* if own match method was successfull,
* the converters are invoked, then the
* convert method of all childs are invoked.
*/
void MXmlMatchingNode::convert( MTreeNode* ptNode )
{
	if( this->match( ptNode ) )
	{
		// invoke childs nodes
		if( ivChilds.getLength() > 0 )
		{
			unsigned int count = this->ivChilds.getLength();
			for( unsigned int i=0;i<count;i++ )
				this->getChild( i )->convert( ptNode );
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
}

/**
 * this method returns a pointer to the implementation
 * of specified interface (className), like
 * queryInterface does in Microsoft COM.
 */
void *MXmlMatchingNode::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MXmlMatchingNode*)this);
	else if( className == "IXmlMatchingNode" )
		return (void*) ((IXmlMatchingNode*)this);
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else if( className == "IMUnknown" )
		return (void*) ((IMUnknown*) ((MObject*)this) );
	else
		return MObject::getInterface( className );
}

/**
 * returns the runtime type information
 */
IRtti* MXmlMatchingNode::getRtti() const
{
	return &gvRtti;
}

/**
 * returns the parent condition
 */
IXmlMatchingNode* MXmlMatchingNode::getParent()
{
	return ivPtParent;
}

/**
 * sets the parent condition
 */
void MXmlMatchingNode::setParent( IXmlMatchingNode* ptNode )
{
	ivPtParent = ptNode;
}

/**
 * returns the number of child conditions
 */ 
unsigned int MXmlMatchingNode::getChildCount()
{
	return ivChilds.getLength();
}

/**
 * returns the child condition at the specified index
 */
IXmlMatchingNode* MXmlMatchingNode::getChild( unsigned int index )
{
	MObject* ptObj = ivChilds.get( index );
	return (IXmlMatchingNode*) ptObj->getInterface( "IXmlMatchingNode" );
}

/**
 * adds a child condition
 */
void MXmlMatchingNode::addChild( IXmlMatchingNode* ptNode )
{
	MObject* ptObj = (MObject*) ptNode->getInterface( "MObject" );
	if( ptObj )
	{
		ptNode->setParent( this );
		ivChilds.add( ptObj );
	}
	else
		throw MException( "MXmlMatchingNode->addChild: not a MObject" );
}

/**
 * storing
 */
MTreeNode* MXmlMatchingNode::save()
{
	MTreeNode* ptBack = new MTreeNode( MObject::ELEM_OBJECT );
	ptBack->setAttribute( MObject::ATTR_CLASS, getRtti()->getClassName() );

	if( getChildCount() > 0 )
	{
		unsigned int count = getChildCount();
		for( unsigned int i=0;i<count;i++ )
			ptBack->addChild( getChild( i )->save() );
	}

	if( this->ivConverters.getLength() > 0 )
	{
		MTreeNode* ptConverterNode = new MTreeNode( "converter" );
		unsigned int count = ivConverters.getLength();
		for( unsigned int i=0;i<count;i++ )
		{
			MObject* ptConverter = ivConverters.get( i );
			ISerializeable* ptSer = (ISerializeable*) ptConverter->getInterface( "ISerializeable" );
			ptConverterNode->addChild( ptSer->save() );
		}
		ptBack->addChild( ptConverterNode );
	}

	return ptBack;
}

/**
 * loading
 */
void MXmlMatchingNode::load( MTreeNode* parentNode )
{
	ASSERT( parentNode->getAttribute( ATTR_CLASS ) == getRtti()->getClassName() );
	MTreeNodeIterator iter = parentNode->getIterator();
	while( iter.hasNext() )
	{
		MTreeNode* ptChild = iter.nextTreeNode();
		if( ptChild->getName() == MObject::ELEM_OBJECT )
		{
			MObject* ptNewChild = MDefaultFactory::getInstance()->createObject( ptChild );
			if( ptNewChild )
			{
				IXmlMatchingNode* ptMatch = (IXmlMatchingNode*) ptNewChild->getInterface( "IXmlMatchingNode" );
				if( ptMatch )
					addChild( ptMatch );
				else
					throw MException( "MXmlMatchingNode->load: loaded object is not a matching node" );
			}
		}
		else if( ptChild->getName() == "converter" )
		{
			MTreeNodeIterator iter2 = ptChild->getIterator();
			while( iter2.hasNext() )
			{
				MTreeNode* ptConverterNode = iter2.nextTreeNode();
				MObject* ptConverter = MDefaultFactory::getInstance()->createObject( ptConverterNode );
				if( ptConverter )
					this->ivConverters.add( ptConverter );
				else
					throw MException(
						"MXmlMatchingNode->load: converter " +
						ptConverterNode->getAttribute( MObject::ATTR_CLASS ) +
						" not found!" );
			}
		}
	}
}