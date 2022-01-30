/*
	(C)2001 MRoc hifiShock

	version 1.0
	version 2.0 -> replaced iostream by CFile
	version 3.0 -> waves saved in song file
*/

#include <framework/io/MFilePrinter.h>
#include <framework/io/MLogger.h>
#include <gui/dialog/MMessageBox.h>

#include "MFileHandling.h"

char *MFileHandling::FILE_VERSION = "1.5";
MFileHandling *MFileHandling::ivInstance = NULL;

MFileHandling::MFileHandling()
{
}

MFileHandling::~MFileHandling()
{
}

String MFileHandling::getVersion()
{
	return MFileHandling::FILE_VERSION;
}

bool MFileHandling::saveMXLFile( String fileName, MTreeNode* ptRoot )
{
	String pre =
		String( "<?xml version=\"1.0\" encoding=\"US-ASCII\" standalone=\"yes\"?>" ) +
		String( "<!-- MSynth XML file - DO NOT EDIT THIS FILE! -->\n" ) +
		String( "<MSynth version=\"" ) +
		String( MFileHandling::FILE_VERSION ) +
		String( "\">\n" );
	String post = "</MSynth>";

	MFilePrinter out;
	if( out.open( fileName ) )
	{
		out.print( pre );
		ptRoot->toXml( &out );
		out.print( post );
		return out.close();
	}
	else
		return false;
}

bool MFileHandling::openFile( String fileName, ISerializeable* callbackInit )
{
	bool back = true;
	MLogger::logMessage( "<mfilehandling::openfile filename=\"%s\">\n", fileName.getData() );

	String error( "" );
	MSaxTreeDocument doc;
	doc.parse( fileName );
	MTreeNode* ptRoot = doc.getRoot();
	if( ptRoot )
	{
		MTreeNode* parentElement = getRootNode( ptRoot );
		if( parentElement )
			callbackInit->load( parentElement );
		else
		{
			MMessageBox::showError( "ERROR_LOADING", fileName, "No root node found (1)" );
			back = false;
		}
	}
	else
	{
		MMessageBox::showError( "ERROR_LOADING", fileName, "No root node found (2)" );
		back = false;
	}

	return back;
};

MTreeNode* MFileHandling::getRootNode( MTreeNode* rootNode )
{
	MTreeNode* ptBack = 0;

	String rootName  = rootNode->getName();

	if( rootName == "MSynth" )
	{
		String versionTag = rootNode->getAttribute( "version" );
		if( versionTag == "1.0" )
		{
			TRACE( "<try to convert from 1.0 to 1.4>\n" );
			MXMLConverter* c = MXMLConverter::getInstance();
			c->formatXML( rootNode, 0 );
		}
		else if( versionTag == "1.1" )
		{
			TRACE( "<try to convert from 1.1 to 1.4>\n" );
			MXMLConverter* c = MXMLConverter::getInstance();
			c->formatXML( rootNode, 1 );
		}
		else if( versionTag == "1.2" )
		{
			TRACE( "<try to convert from 1.2 to 1.4>\n" );
			MXMLConverter* c = MXMLConverter::getInstance();
			c->formatXML( rootNode, 2 );
		}
		else if( versionTag == "1.3" )
		{
			TRACE( "<try to convert from 1.2 to 1.4>\n" );
			MXMLConverter* c = MXMLConverter::getInstance();
			c->formatXML( rootNode, 3 );
		}
		else if( versionTag == "1.4" )
		{
			TRACE( "<try to convert from 1.2 to 1.4>\n" );
			MXMLConverter* c = MXMLConverter::getInstance();
			c->formatXML( rootNode, 4 );
		}

		MTreeNodeIterator iter = rootNode->getIterator();
		while( iter.hasNext() )
		{
			MTreeNode* node = iter.nextTreeNode();
			if( node->getName() == "sequencer" )
				ptBack = node;
		}
	}
	else if( rootName == "sequencer" )
	{
		TRACE( "<try to convert from 1.0old to 1.3>\n" );
		MXMLConverter* c = MXMLConverter::getInstance();
		c->formatXML( rootNode, 0 );
		ptBack = rootNode;
	}

	return ptBack;
}

MFileHandling *MFileHandling::getInstance()
{
	if( ivInstance = NULL )
		ivInstance = new MFileHandling();

	return ivInstance;
}

void MFileHandling::release()
{
	SAFE_DELETE( ivInstance );
}