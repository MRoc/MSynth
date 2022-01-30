#include "MDynamics.h"
#include <framework/resource/MResourceManager.h>
#include <framework/treedocument/MSaxTreeDocument.h>
#include <gui/dialog/MMessageBox.h>

/**
 * the gobal runtime type info
 */
MRtti MDynamics::gvRtti = MRtti( "MDynamics", 0 );



/**
 * constructor
 */
MDynamics::MDynamics()
{
}

/**
 * destructor
 */
MDynamics::~MDynamics()
{
}

/**
 * returns the dynamic's name
 */
String MDynamics::getName()
{
	return ivName;
}

/**
 * returns the runtime type information
 */
IRtti* MDynamics::getRtti() const
{
	return &gvRtti;
}

/**
 * querries for a specified interface
 * (inheritanced from IMUnknown)
 */
void* MDynamics::getInterface( const String &className ) const
{
	return MObject::getInterface( className );
}

/**
 * loads from the given tree node
 */
void MDynamics::load( MTreeNode* parentNode )
{
	ivName = parentNode->getAttribute( "name" );
	ivValues.setLength( parentNode->getChildCount() );
	MTreeNodeIterator iter = parentNode->getIterator();
	int counter = 0;
	while( iter.hasNext() )
	{
		String value = iter.nextTreeNode()->getAttribute( "value" );
		if( value.GetLength() != 0 )
			ivValues[ counter++ ] = MFloat::parse( value );
	}
}

/**
 * stores as tree node
 */
MTreeNode* MDynamics::save()
{
	return 0;
}

/**
 * applies the dynamics on the given notelist
 */
void MDynamics::apply( MDefaultNoteList* pList )
{
	unsigned int maxLen = pList->getMaxLength();
	unsigned int dynCount = ivValues.getDataLength();
	unsigned int divider = maxLen / dynCount;
	unsigned int rest = maxLen % dynCount;

	for( unsigned int i=0;i<divider;i++ )
		for( unsigned int j=0;j<dynCount;j++ )
			pList->applyDynamics( i*dynCount+j, (FP)ivValues[ j ] );
	for( unsigned int i=0;i<rest;i++ )
		pList->applyDynamics( divider*dynCount+i, (FP)ivValues[ i ] );
}

/**
 * returns the indexed value
 */
double MDynamics::get( unsigned int index )
{
	return ivValues[ index % ivValues.getDataLength() ];
}

/**
 * parses all dynamics files found in the given resource path
 */
MObjectList* MDynamics::loadFromResource( String path )
{
	MObjectList* pBack = new MObjectList();

	IFolder* folder = MResourceManager::getInstance()->getFolder( path );
	if( folder )
	{
		unsigned int fileCount = folder->getFileCount();
		for( unsigned int i=0;i<fileCount;i++ )
		{
			try
			{
				MSaxTreeDocument doc;
				doc.parseResource( path + "/" + folder->getFile( i ) );
				MDynamics* dyn = new MDynamics();
				dyn->load( doc.getRoot() );
				pBack->add( dyn );
			}
			catch( MException ae )
			{
				MMessageBox::showError(
					ae.getExceptionDescripton() );
			}
		}
	}
	
	return pBack;
}