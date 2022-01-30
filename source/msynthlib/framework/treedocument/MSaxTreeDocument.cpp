#include "msaxtreedocument.h"
#include "../io/MFileInputStream.h"
#include "../resource/MResourceManager.h"

#define MPARSEBUFLEN 65536

/**
 * the runtime type info
 */
MRtti MSaxTreeDocument::gvRtti = MRtti( "MSaxTreeDocument", MSaxTreeDocument::createInstance );

/**
 * constructor
 */
MSaxTreeDocument::MSaxTreeDocument() :
	ivPtCurrentNode( 0 )
{
}

/**
 * destructor
 */
MSaxTreeDocument::~MSaxTreeDocument()
{
}

/**
 * parses the specified xml file
 */
void MSaxTreeDocument::parse( String fileName )
{
	MFileInputStream in;
 	in.open( fileName );

	try
	{
		parse( &in );
	}
	catch( MException ae )
	{
		in.close();
		throw ae;
	}

	in.close();
}

/**
 * parse a resource xml file
 */
void MSaxTreeDocument::parseResource( String resourceName )
{
	ISeekableInputStream* pIn = MResourceManager::getInstance()->openReadFile( resourceName );
	if( pIn )
	{
		parse( pIn );
		MResourceManager::getInstance()->close( pIn );
	}
}

/**
 * parses the xml file given in the input stream
 */
void MSaxTreeDocument::parse( ISeekableInputStream* pIn )
{
	XML_Parser p = XML_ParserCreate( "US-ASCII" );
	XML_SetElementHandler( p, _startElementHandler, _endElementHandler );
	XML_SetCharacterDataHandler( p, _characterHandler );
	XML_SetUserData( p, (void*) this );

	int done;
	int len = MPARSEBUFLEN;
	char* buffer = new char[MPARSEBUFLEN];
	if( buffer == 0 )
		throw MException( "MSaxTreeDocument::parse: Could not allocate buffer mem!" );

	do
	{
		len = MPARSEBUFLEN;
		if( pIn->getFileLength() - pIn->getPosition() < MPARSEBUFLEN )
			len = pIn->getFileLength() - pIn->getPosition();
		if( len )
			pIn->read( (signed char*) buffer, len );

		done = len < MPARSEBUFLEN;
		if( XML_Parse( p, buffer, len, done ) == XML_STATUS_ERROR )
			throw MException( "MSaxTreeDocument::parse: could not parse stream" );
	}
	while( !done );
	XML_ParserFree( p );
	delete buffer;
}

/**
 * creates an instance of this class
 */
MObject* MSaxTreeDocument::createInstance()
{
	return new MSaxTreeDocument();
}

/**
 * returns the runtime type info
 */
IRtti* MSaxTreeDocument::getRtti() const
{
	return &gvRtti;
}

/**
 * queries for a specified interface
 */
void* MSaxTreeDocument::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return ((MSaxTreeDocument*)this);
	else
		return MObject::getInterface( className );
}

/*void MSaxTreeDocument::characters(
	const XMLCh* const chars,
	const unsigned int length )
{
	this->ivPtCurrentNode->setContent( _X( chars ) );
}*/

void MSaxTreeDocument::startElementHandler(
	void *userData,
	const XML_Char *name,
	const XML_Char **atts )
{
	if( this->ivPtCurrentNode == 0 )
	{
		ivPtCurrentNode = new MTreeNode();
		ivPtRoot = ivPtCurrentNode;
	}
	else
	{
		MTreeNode* ptNew = new MTreeNode();
		ivPtCurrentNode->addChild( ptNew );
		ivPtCurrentNode = ptNew;
	}

	ivPtCurrentNode->setName( name );

	const XML_Char** pt = atts;
	while( (*pt) )
	{
		ivPtCurrentNode->setAttribute(
			*pt,
			*(pt+1)	);
		pt +=2 ;
	}

}

void MSaxTreeDocument::endElementHandler(
	void *userData,
	const XML_Char *name )
{
	ivPtCurrentNode = (MTreeNode*) ivPtCurrentNode->getParent();
}

void MSaxTreeDocument::characterHandler( const XML_Char *s, int len )
{
	String tmp;
	tmp.setData( s, len );

	if( ivPtCurrentNode->getChildCount() > 0 &&
		ivPtCurrentNode->getChild( ivPtCurrentNode->getChildCount() - 1 )->getType() == MTreeLeaf::LEAF )
	{
		MTreeLeaf* ptLeaf = ivPtCurrentNode->getChild( ivPtCurrentNode->getChildCount() - 1 );
		ptLeaf->setName( ptLeaf->getName() + tmp );
	}
	else
	{
		MTreeLeaf* ptLeaf = new MTreeLeaf( tmp );
		ivPtCurrentNode->addChild( ptLeaf );
	}
}

void MSaxTreeDocument::_startElementHandler(
	void *userData,
	const XML_Char *name,
	const XML_Char **atts )
{
	((MSaxTreeDocument*)userData)->startElementHandler( userData, name, atts );
}

void MSaxTreeDocument::_endElementHandler(
	void *userData,
	const XML_Char *name )
{
	((MSaxTreeDocument*)userData)->endElementHandler( userData, name );
}

void MSaxTreeDocument::_characterHandler( void *userData, const XML_Char *s, int len )
{
	((MSaxTreeDocument*)userData)->characterHandler( s, len );
}