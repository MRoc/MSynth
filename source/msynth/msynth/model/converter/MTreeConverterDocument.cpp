#include "mtreeconverterdocument.h"

/**
 * runtime type info
 */
MRtti MTreeConverterDocument::gvRtti = MRtti( "MTreeConverterDocument", MTreeConverterDocument::createInstance );

/**
 * constructor
 */
MTreeConverterDocument::MTreeConverterDocument()
{
}

/**
 * destructor
 */
MTreeConverterDocument::~MTreeConverterDocument()
{
	ivElementMappings.clear();

	unsigned int count = this->ivElements.size();
	for( unsigned int i=0;i<count;i++ )
	{
		MObject* ptObj = (MObject*) ivElements[ i ]->getInterface( "MObject" );
		ASSERT( ptObj );
		delete ptObj;
	}
	ivElements.clear();
}

/**
 * returns the runtime type info
 */
IRtti* MTreeConverterDocument::getRtti() const
{
	return &gvRtti;
}

/**
 * query interface
 */
void* MTreeConverterDocument::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MTreeConverterDocument*)this);
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else
		return MObject::getInterface( className );
}

/**
 * create a instance of this class
 */
MObject* MTreeConverterDocument::createInstance()
{
	return new MTreeConverterDocument();
}

/**
 * this method adds a MTreeConverterEntry to this converter
 */
void MTreeConverterDocument::addEntry( MElementMatch* ptEntry )
{
	ivElements.push_back( ptEntry );
	ivElementMappings.insert(
		Str2NcMap::value_type( ptEntry->getElementName(), ptEntry ) );

	unsigned int count = ptEntry->getOldNameCount();
	for( unsigned int i=0;i<count;i++ )
		ivElementMappings.insert(
			Str2NcMap::value_type( ptEntry->getOldName( i ), ptEntry ) );
}

/**
 * this method performs the conversion of a complete tree
 */
void MTreeConverterDocument::convert( MTreeNode* ptNode )
{
	convertRecursive( ptNode );
}

/**
 * # used for recursive converting the tree
 */
void MTreeConverterDocument::convertRecursive( MTreeNode* ptNode )
{
	String nodeName = ptNode->getName();
	Str2NcMapIter is = ivElementMappings.find( nodeName );
	if( is != ivElementMappings.end() )
	{
		is->second->convert( ptNode );
	}

	MTreeNodeIterator iter = ptNode->getIterator();
	while( iter.hasNext() )
		convertRecursive( iter.nextTreeNode() );
}

/**
 * loading
 */
void MTreeConverterDocument::load( MTreeNode* ptNode )
{
	if( ptNode->getName() == MObject::ELEM_OBJECT )
	{
		if( ptNode->getAttribute( MObject::ATTR_CLASS ) == getRtti()->getClassName() )
		{
			MTreeNodeIterator iter = ptNode->getIterator();
			while( iter.hasNext() )
			{
				MTreeNode* ptChild = iter.nextTreeNode();
				MObject* ptObj = MDefaultFactory::getInstance()->createObject( ptChild );
				if( ptObj )
				{
					MElementMatch* ptElementMatch = (MElementMatch*) ptObj->getInterface( "MElementMatch" );
					if( ptElementMatch )
						this->addEntry( ptElementMatch );
					else
						throw MException( "MTreeConverterDocument->load: created object is not a MElementMatch!" );
				}
				else
					throw MException(
						"MTreeConverterDocument->load: could not create class " + ptChild->getAttribute( MObject::ATTR_CLASS ) );
			}
		}
		else
			throw MException( "MTreeConverterDocument->load: not a MTreeConverterDocument!" );
	}
	else
		throw MException( "MTreeConverterDocument->load: not a MObject!" );
}

/**
 * saving
 */
MTreeNode* MTreeConverterDocument::save()
{
	MTreeNode* ptBack = new MTreeNode( MObject::ELEM_OBJECT );
	ptBack->setAttribute( MObject::ATTR_CLASS, getRtti()->getClassName() );

	unsigned int count = (unsigned int) this->ivElements.size();
	for( unsigned int i=0;i<count;i++ )
	{
		ptBack->addChild( ivElements[ i ]->save() );
	}

	return ptBack;
}