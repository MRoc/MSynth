#include "MTreeDocumentUtils.h"

#include "../io/MLogger.h"
#include "../MDefaultFactory.h"
#include "MSaxTreeDocument.h"
#include "../io/MFileOutputStream.h"
#include "../io/MStreamPrinter.h"


/** creates a mobject from the given node */
MObject* MTreeDocumentUtils::createObject( MTreeNode* pNode )
{
	String className = pNode->getAttribute( MObject::ATTR_CLASS );
	MObject* pObj = MDefaultFactory::getInstance()->createObject( className );
	if( pObj )
	{
		ISerializeable* pSer = (ISerializeable*) pObj->getInterface( "ISerializeable" );
		if( pSer )
			pSer->load( pNode );
		else
			MLogger::logWarning(
				"MTreeDocumentUtils::createObject: object of class \"%s\" does not support serialisation",
				className.getData() );
	}
	else
		MLogger::logWarning( "MTreeDocumentUtils::createObject: coult not create object of class \"%s\"", className.getData() );
	return pObj;
}

/** parses and creates the objects in the given xml file */
MObject* MTreeDocumentUtils::parse( String fileName )
{
	MSaxTreeDocument doc;
	try
	{
		doc.parse( fileName );
		return createObject( doc.getRoot() );
	}
	catch( MException ae )
	{
		MLogger::logWarning( ae.getExceptionDescripton() );
		return 0;
	}
}

/** recursive print the node */
void MTreeDocumentUtils::store( MTreeNode* pNode, String fileName )
{
	MFileOutputStream out;
	out.open( fileName );
	MStreamPrinter printer( &out );
	pNode->toXml( &printer );
	out.close();
}