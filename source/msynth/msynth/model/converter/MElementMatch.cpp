#include "melementmatch.h"

/**
 * runtime support
 */
MRtti MElementMatch::gvRtti = MRtti( "MElementMatch", MElementMatch::createInstance );

/**
 * constructor
 */
MElementMatch::MElementMatch()
{
}

/**
 * destructor
 */
MElementMatch::~MElementMatch()
{
}

/**
 * returns a pointer to the as className
 * specified interface at runtime
 */
void *MElementMatch::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MElementMatch*)this);
	if( className == "IElementMatch" )
		return (void*) ((IElementMatch*)this);
	if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else
		return MXmlMatchingNode::getInterface( className );
}

/**
 * returns the runtime type info
 */
IRtti* MElementMatch::getRtti() const
{
	return &gvRtti;
}

/**
 * returns the name of the new element (can be the same as old one)
 */
String MElementMatch::getElementName()
{
	return ivName;
}

/**
 * sets the element name
 */
void MElementMatch::setElementName( String en )
{
	ivName = en;
}

/**
 * returns the number of old names this tag had
 */
unsigned int MElementMatch::getOldNameCount()
{
	return ivOldNames.size();
}

/**
 * returns the old name at the specified index
 */
String MElementMatch::getOldName( unsigned int index )
{
	return ivOldNames[ index ];
}

/**
 * adds a old name to the oldname list
 */
void MElementMatch::addOldName( String name )
{
	ivOldNames.push_back( name );
}

/**
 * returns true if the given name is in the name list
 */
bool MElementMatch::hasOldName( String name )
{
	bool back = false;
	for( StrList::iterator i=ivOldNames.begin();i!=ivOldNames.end();i++ )
		if( (*i) == name )
		{
			back = true;
			break;
		}
	return back;
}

/**
 * returns true after checking this and the child conditions
 */
bool MElementMatch::match( MTreeNode* ptNode )
{
	return ivName == ptNode->getName() || hasOldName( ptNode->getName() );
}

/**
 * creates a instance of this class
 */
MObject* MElementMatch::createInstance()
{
	return new MElementMatch();
}

/**
 * saving
 */
MTreeNode* MElementMatch::save()
{
	MTreeNode* ptBack = new MTreeNode( MObject::ELEM_OBJECT );
	ptBack->setAttribute( MObject::ATTR_CLASS, getRtti()->getClassName() );
	ptBack->setAttribute( "name", ivName );

	for( StrListIter si=ivOldNames.begin();si!=ivOldNames.end();si++ )
	{
		MTreeNode* ptChild = new MTreeNode( "old" );
		ptChild->setAttribute( "name", (*si) );
		ptBack->addChild( ptChild );
	}

	unsigned int count = getChildCount();
	for( unsigned int i=0;i<count;i++ )
		ptBack->addChild( ((ISerializeable*)getChild( i )->getInterface("ISerializeable"))->save() );

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
void MElementMatch::load( MTreeNode* ptNode )
{
	ASSERT( ptNode->getAttribute( ATTR_CLASS ) == getRtti()->getClassName() );
	this->ivName = ptNode->getAttribute( "name" );

	MTreeNodeIterator iter = ptNode->getIterator();
	while( iter.hasNext() )
	{
		MTreeNode* ptChild = iter.nextTreeNode();
		if( ptChild->getName() == ELEM_OBJECT )
		{
			MObject* ptNewChild = MDefaultFactory::getInstance()->createObject( ptChild );
			if( ptNewChild )
			{
				IXmlMatchingNode* ptMatch = (IXmlMatchingNode*) ptNewChild->getInterface( "IXmlMatchingNode" );
				if( ptMatch )
					addChild( ptMatch );
				else
					throw MException( "MElementMatch->load: loaded object is not a matching node" );
			}
			else
				ASSERT( false );
		}
		else if( ptChild->getName() == "old" )
		{
			addOldName( ptChild->getAttribute( "name" ) );
		}
		else if( ptChild->getName() == "converter" )
		{
			MTreeNodeIterator iter = ptChild->getIterator();
			while( iter.hasNext() )
			{
				MTreeNode* ptConverterNode = iter.nextTreeNode();
				if( ptConverterNode->getName() == MObject::ELEM_OBJECT )
				{
					MObject* ptConverter = MDefaultFactory::getInstance()->createObject( ptConverterNode );
					if( ptConverter )
						this->ivConverters.add( ptConverter );
					else
						throw MException(
							"MXmlMatchingNode->load: converter " +
							ptConverterNode->getAttribute( MObject::ATTR_CLASS ) +
							" not found!" );
				}
				else
					ASSERT( false );
			}
		}
	}
}

/**
* if own match method was successfull,
* the converters are invoked, then the
* convert method of all childs are invoked.
*/
void MElementMatch::convert( MTreeNode* ptNode )
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