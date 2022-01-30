#include "MTreeNode.h"
#include "../io/MLogger.h"
#include "MTreeNodeIterator.h"

/** the runtime type info */
MRtti MTreeNode::gvRtti = MRtti( "MTreeNode", MTreeNode::createInstance );

/** constructor */
MTreeNode::MTreeNode() :
	MTreeLeaf()
{
}

/** constructor */
MTreeNode::MTreeNode( const String& name ) :
	MTreeLeaf( name )
{
}

/** destructor */
MTreeNode::~MTreeNode()
{
	ivAttributes.clear();
	ivChilds.deleteAll();
}

/** sets an attribute of this node */
void MTreeNode::setAttribute( const String &key, const String &value )
{
	ASSERT( key != "" );
	StrStrMapIter i = ivAttributes.find( key );
	if( i != ivAttributes.end() )
		ivAttributes.erase( i );
	ivAttributes.insert( StrStrMap::value_type( key, value ) );
}

/** returns a attribute of this node */
String MTreeNode::getAttribute( const String &key )
{
	StrStrMapIter i = ivAttributes.find( key );
	if( i != ivAttributes.end() )
		return i->second;
	else
		return "";
}

/** returns a attribute of this node, if not available, it returns the defaultvalue */
String MTreeNode::getAttribute( const String& key, const String& defaultValue )
{
	StrStrMapIter i = ivAttributes.find( key );
	if( i != ivAttributes.end() )
		return i->second;
	else
		return defaultValue;
}

/** returns the number of attributes */
unsigned int MTreeNode::getAttributeCount()
{
	return (unsigned int) ivAttributes.size();
}

/** returns the attribute name at the specified index !!! HACKED !!! */
String MTreeNode::getAttributeName( unsigned int index )
{
	ASSERT( index < getAttributeCount() );
	StrStrMap::iterator i = ivAttributes.begin();
	unsigned int count = 0;
	for( ;i!=ivAttributes.end();i++ )
		if( count++ == index )
			return i->first;
	return "";
}

/** returns true if the attribute is stored */
bool MTreeNode::hasAttribute( const String &key )
{
	return ivAttributes.find( key ) != ivAttributes.end();
}

/** removes the specified attribute from node */
void MTreeNode::removeAttribute( const String &attributeName )
{
	ASSERT( hasAttribute( attributeName ) );
	ivAttributes.erase( ivAttributes.find( attributeName ) );
}

/** adds a child to this node */
void MTreeNode::addChild( MTreeLeaf* ptNode )
{
	ptNode->setParent( this );
	ivChilds.add( ptNode );
}

/** removes the child at the specified index */
MTreeLeaf* MTreeNode::removeChild( unsigned int index )
{
	ASSERT( index < getChildCount() );
	return (MTreeNode*) ivChilds.remove( index );
}

/** deletes the child at the specified index */
void MTreeNode::deleteChild( unsigned int index )
{
	ASSERT( index < getChildCount() );
	ivChilds.deleteObj( index );
}

/** returns the number of childs */
unsigned int MTreeNode::getChildCount()
{
	return (unsigned int) ivChilds.getLength();
}

/** returns the child at the specified index */
MTreeLeaf* MTreeNode::getChild( unsigned int index )
{
	ASSERT( index < ivChilds.getLength() );
	return (MTreeLeaf*) ivChilds.get( index );
}

/** returns the first node with the given name */
MTreeNode* MTreeNode::getFirstNode( const String& nodeName )
{
	MTreeNode* pBack = 0;
	unsigned int count = getChildCount();
	for( unsigned int i=0;i<count&&pBack==0;i++ )
		if( getChild( i )->getName() == nodeName &&
			getChild( i )->getInterface( "MTreeNode" ) )
		{
			pBack = (MTreeNode*) getChild( i )->getInterface( "MTreeNode" );
		}
	return pBack;
}

/** generates xml */
void MTreeNode::toXml( IPrintStream* ptOut )
{
	if( ivName != "" )
	{
		ptOut->print( "<" );
		ptOut->print( ivName );
		if( ivAttributes.size() > 0 )
		{
			StrStrMap::iterator i = ivAttributes.begin();
			for( ;i!=ivAttributes.end();i++ )
			{
				ptOut->print( " " );
				ptOut->print( i->first );
				ptOut->print( "=\"" );
				ptOut->print( i->second );
				ptOut->print( "\"" );
			}
		}
		ptOut->print( ">" );

		if( ivChilds.getLength() > 0 )
		{
			unsigned int count = getChildCount();
			for( unsigned int i=0;i<count;i++ )
				getChild( i )->toXml( ptOut );
		}

		ptOut->print( "</" );
		ptOut->print( ivName );
		ptOut->print( ">" );
	}
}

/** queries for an interface */
void* MTreeNode::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return ((MTreeNode*)this);
	else
		return MTreeLeaf::getInterface( className );
}

/** returns the runtime type info */
IRtti* MTreeNode::getRtti() const
{
	return &gvRtti;
}

/** creates an instance of this class */
MObject* MTreeNode::createInstance()
{
	return new MTreeNode();
}

/** returns the leaf/node type */
MTreeLeaf::LeafType MTreeNode::getType() const
{
	return MTreeLeaf::NODE;
}

/** return a iterator */
MTreeNodeIterator MTreeNode::getIterator()
{
	return MTreeNodeIterator( &ivChilds );
}