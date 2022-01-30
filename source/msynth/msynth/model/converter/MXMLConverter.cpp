/**
 * (C) 2003 MRoc
 */

#include "MXMLConverter.h"
#include <framework/io/MFilePrinter.h>
#include <gui/dialog/MMessageBox.h>

/**
 * singleton instance
 */
MXMLConverter* MXMLConverter::gvInstance = 0;

/**
 * # constructor
 */
MXMLConverter::MXMLConverter()
{
}

/**
 * # destructor
 */
MXMLConverter::~MXMLConverter()
{
	this->ivConverterDocuments.deleteAll();
}

/**
 * returns the singleton instance
 */
MXMLConverter* MXMLConverter::getInstance()
{
	if( gvInstance == 0 )
	{
		gvInstance = new MXMLConverter();
		try
		{
			gvInstance->loadImportDocuments();
		}
		catch( MException ae )
		{
			MMessageBox::showError(
				"MXMLConverter loading failed: " + ae.getExceptionDescripton()
				);
		}
	}	
	return gvInstance;
};

/**
 * releases the singleton instance
 */
void MXMLConverter::release()
{
	if( gvInstance != 0 )
	{
		delete gvInstance;
		gvInstance = 0;
	}
}

/**
 * loads all import maps
 */
void MXMLConverter::loadImportDocuments()
{
	loadImportDocument( "resource/converter/importmap1.0to1.1.xml" );
	loadImportDocument( "resource/converter/importmap1.1to1.2.xml" );
	loadImportDocument( "resource/converter/importmap1.2to1.3.xml" );
	loadImportDocument( "resource/converter/importmap1.3to1.4.xml" );
	loadImportDocument( "resource/converter/importmap1.4to1.5.xml" );
}

/**
 * loads a specific import map
 */
void MXMLConverter::loadImportDocument( String filename )
{
	MSaxTreeDocument doc;
	doc.parseResource( filename );
	MTreeNode* ptRoot = doc.getRoot();
	if( ptRoot )
	{
		MObject* ptObj = MDefaultFactory::getInstance()->createObject( ptRoot );
		if( ptObj )
		{
			MTreeConverterDocument* ptDoc = (MTreeConverterDocument*) ptObj->getInterface( "MTreeConverterDocument" );
			if( ptDoc )
			{
				this->ivConverterDocuments.add( ptDoc );
			}
			else
			{
				delete ptObj;
				throw MException( "MXMLConverter->loadImportDocument: loaded file is not a MTreeConverterdocument" );
			}
		}
		else
			throw MException( "MXMLConverter->loadImportDocument: loaded file is not a MObject" );
	}
	else
		throw MException( "MXMLConverter->loadImportDocument: no root node found" );
}

/**
 * does the conversion depending on the given version tag
 */
void MXMLConverter::formatXML( MTreeNode* ptNode, unsigned int versionTag )
{
	if( versionTag < 1 ) // convert from 1.0 to 1.1
		((MTreeConverterDocument*)ivConverterDocuments.get( 0 ))->convert( ptNode );

	if( versionTag < 2 ) // convert from 1.1 to 1.2
		((MTreeConverterDocument*)ivConverterDocuments.get( 1 ))->convert( ptNode );

	if( versionTag < 3 ) // convert from 1.2 to 1.3
		((MTreeConverterDocument*)ivConverterDocuments.get( 2 ))->convert( ptNode );

	if( versionTag < 4 ) // convert from 1.3 to 1.4
		((MTreeConverterDocument*)ivConverterDocuments.get( 3 ))->convert( ptNode );

	if( versionTag < 5 ) // convert from 1.3 to 1.4
		((MTreeConverterDocument*)ivConverterDocuments.get( 4 ))->convert( ptNode );

	/*MFilePrinter out;
	out.open( "testConv4.xml" );
	ptNode->toXml( &out );
	out.close();*/
}