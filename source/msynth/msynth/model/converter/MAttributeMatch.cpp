#include "mattributematch.h"

/**
 * runtime support
 */
MRtti MAttributeMatch::gvRtti = MRtti( "MAttributeMatch", MAttributeMatch::createInstance );

/**
 * constructor
 */
MAttributeMatch::MAttributeMatch() :
	ivMatchingType( EXACT )
{
}

/**
 * destructor
 */
MAttributeMatch::~MAttributeMatch()
{
	ivPossibleContent.clear();
}

/**
 * returns the runtime type info
 */
IRtti* MAttributeMatch::getRtti() const
{
	return &gvRtti;
}

/**
 * sets the name of the attribute to find
 */
void MAttributeMatch::setAttributeName( String name )
{
	ivAttributeName = name;
}

/**
 * returns the name of the attribute to find
 */
String MAttributeMatch::getAttributeName()
{
	return ivAttributeName;
}

/**
 * returns true if the attribute is found in
 * the given node and the value is valid
 */
bool MAttributeMatch::match( MTreeNode* ptNode )
{
	bool back = false;
	String content = ptNode->getAttribute( ivAttributeName );
	unsigned int count = this->ivPossibleContent.size();
	for( unsigned int i=0;i<count&&(!back);i++) 
	{
		switch( ivMatchingType )
		{
			case BEGIN:
				if( content.Find( ((String)ivPossibleContent[ i ]) ) == 0 )
					back = true;
				break;
			case END:
				if( content.Find( ((String)ivPossibleContent[ i ]) ) == ivPossibleContent[ i ].GetLength() - 1 )
					back = true;
				break;
			default:
				if( ivPossibleContent[ i ] == content )
					back = true;
				break;
		}
	}
	return back;
}

/**
 * store
 */
MTreeNode* MAttributeMatch::save()
{
	MTreeNode* ptBack = new MTreeNode( MObject::ELEM_OBJECT );
	ptBack->setAttribute( MObject::ATTR_CLASS, getRtti()->getClassName() );
	ptBack->setAttribute( "name", ivAttributeName );
	ptBack->setAttribute( "matchingtype", getMatchingTypeAsString( ivMatchingType ) );

	// conditions
	MTreeNode* ptConditionNode = new MTreeNode( "possiblecontent" );
	unsigned int count = (unsigned int)this->ivPossibleContent.size();
	for( unsigned int i=0;i<count;i++ )
	{
		MTreeNode* ptContentNode = new MTreeNode( "condition" );
		ptContentNode->setAttribute( "content", ivPossibleContent[ i ] );
		ptConditionNode->addChild( ptContentNode );
	}
	ptBack->addChild( ptConditionNode );

	// child matcher
	if( getChildCount() > 0 )
	{
		unsigned int count = getChildCount();
		for( unsigned int i=0;i<count;i++ )
			ptBack->addChild( getChild( i )->save() );
	}

	// converter
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
 * load
 */
void MAttributeMatch::load( MTreeNode* ptNode )
{
	ASSERT( ptNode->getAttribute( ATTR_CLASS ) == getRtti()->getClassName() );

	ivAttributeName = ptNode->getAttribute( "name" );
	ivMatchingType = this->getMatchingType( ptNode->getAttribute( "matchingtype" ) );

	MTreeNodeIterator iter = ptNode->getIterator();
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
			MTreeNodeIterator iter = ptChild->getIterator();
			while( iter.hasNext() )
			{
				MTreeNode* ptConverterNode = iter.nextTreeNode();
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
		else if( ptChild->getName() == "possiblecontent" )
		{
			MTreeNodeIterator iter = ptChild->getIterator();
			while( iter.hasNext() )
			{
				MTreeNode* ptContentNode = iter.nextTreeNode();
				this->ivPossibleContent.push_back( ptContentNode->getAttribute( "content" ) );
			}
		}
	}
}

MObject* MAttributeMatch::createInstance()
{
	return new MAttributeMatch();
}

/**
 * returns a string like the enum matchingtype
 */
String MAttributeMatch::getMatchingTypeAsString( MAttributeMatch::MatchingType t )
{
	switch( t )
	{
		case MatchingType::BEGIN:
			return "BEGIN";
		case MatchingType::END:
			return "END";
		default:
			return "EXACT";
	};
}

/**
 * returns the matchingtype extracted from the string
 */
MAttributeMatch::MatchingType MAttributeMatch::getMatchingType( String t )
{
	if( t == "BEGIN" )
		return MatchingType::BEGIN;
	else if( t == "END" )
		return MatchingType::END;
	else
		return MatchingType::EXACT;
}