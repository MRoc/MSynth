#include "MInt2FloatConverter.h"

/**
 * runtime type info
 */
MRtti MInt2FloatConverter::gvRtti = MRtti( "MInt2FloatConverter", MInt2FloatConverter::createInstance );

/**
 * constructor
 */
MInt2FloatConverter::MInt2FloatConverter() :
	attrName( "" ),
	a( 1.0f ),
	t( 0.0f ),
	ivMin( 0.0f ),
	ivMax( 1.0f ),
	quad( false )
{
}

/**
 * constructor
 */
MInt2FloatConverter::MInt2FloatConverter( String name ) :
	attrName( name ),
	a( 1.0f ),
	t( 0.0f ),
	ivMin( 0.0f ),
	ivMax( 1.0f ),
	quad( false )
{
}

/**
 * convstructor
 */
MInt2FloatConverter::MInt2FloatConverter( String name, float _a, float _t, float _ivMin, float _ivMax ) :
	attrName( name ),
	a( _a ),
	t( _t ),
	ivMin( _ivMin ),
	ivMax( _ivMax ),
	quad( false )
{
}

/**
 * destructor
 */
MInt2FloatConverter::~MInt2FloatConverter()
{
	oldNames.clear(); 
}

/**
 * returns the attribute name that should be converted
 */
String MInt2FloatConverter::getAttributeName()
{
	return attrName;
}

/**
 * does the conversion
 */
void MInt2FloatConverter::convert( MTreeNode* node )
{
	String attributeValue = "";
	if( node->hasAttribute( attrName ) )
	{
		attributeValue = node->getAttribute( attrName );
		node->removeAttribute( attrName );
	}
	else
	{
		for( unsigned int i=0;i<getOldNameCount();i++ )
		{
			String attribute = getOldName( i );
			if( node->hasAttribute( attribute ) )
			{
				attributeValue = node->getAttribute( attribute );
				node->removeAttribute( attribute );
				break;
			}
		}
	}

	int tmp = MInt::parse( attributeValue );
	float v = quad ? float(tmp*tmp) : float(tmp);
	v *= a;
	v += t;

	if( v > ivMax )
		v = ivMax;
	else if( v < ivMin )
		v = ivMin;
	node->setAttribute( attrName, MFloat::toString( v ) );
}

/**
 * loads from tree node
 */
void MInt2FloatConverter::load( MTreeNode* node )
{
	ASSERT( node->getName() == MObject::ELEM_OBJECT );
	ASSERT( node->getAttribute( MObject::ATTR_CLASS ) == getRtti()->getClassName() );
	ASSERT( node->hasAttribute( "name" ) );

	attrName = node->getAttribute( "name" );
	a = MFloat::parse( node->getAttribute( "a" ), -1000000.0f, 1000000.0f, 1.0f );
	t = MFloat::parse( node->getAttribute( "t" ), -1000000.0f, 1000000.0f, 0.0f );
	ivMin = MFloat::parse( node->getAttribute( "min" ), -1000000.0f, 1000000.0f, 0.0f );
	ivMax = MFloat::parse( node->getAttribute( "max" ), -1000000.0f, 1000000.0f, 0.0f );
	quad = MBool::parse( node->getAttribute( "quad" ), false );

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
MTreeNode* MInt2FloatConverter::save()
{
	MTreeNode* ptBack = new MTreeNode( MObject::ELEM_OBJECT );
	ptBack->setAttribute( MObject::ATTR_CLASS, getRtti()->getClassName() );

	// attributes
	ptBack->setAttribute( "a", MFloat::toString( a ) );
	ptBack->setAttribute( "t", MFloat::toString( t ) );
	ptBack->setAttribute( "min", MFloat::toString( ivMin ) );
	ptBack->setAttribute( "max", MFloat::toString( ivMax ) );
	ptBack->setAttribute( "quad", MBool::toString( quad ) );

	// old names
	unsigned int count = this->getOldNameCount();
	for( unsigned int i=0;i<count;i++ )
	{
		MTreeNode* ptOldNameNode = new MTreeNode( "old" );
		ptOldNameNode->setAttribute( "name", getOldName( i ) );
		ptBack->addChild( ptOldNameNode );
	}

	return ptBack;
}

/**
 * returns the old name at the specified index
 */
String MInt2FloatConverter::getOldName( unsigned int index )
{
	return oldNames[ index ];
}

/**
 * returns the number of old names
 */
unsigned int MInt2FloatConverter::getOldNameCount()
{
	return oldNames.size();
}

/**
 * adds a old name
 */
void MInt2FloatConverter::addOldName( String oldName )
{
	oldNames.push_back( oldName );
}


/**
 * returns the runtime type info
 */
IRtti* MInt2FloatConverter::getRtti() const
{
	return &gvRtti;
}

/**
 * query interface
 */
void* MInt2FloatConverter::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MInt2FloatConverter*)this);
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
MObject* MInt2FloatConverter::createInstance()
{
	return new MInt2FloatConverter();
}