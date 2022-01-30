/**
 * MProperties (C)2001-2002 MRoc hifiShock
 */

#include "MProperties.h"
#include "../resource/MResourceManager.h"
#include "../io/MStreamPrinter.h"
#include "../primitive/MInt.h"
#include "../primitive/MBool.h"
#include "../primitive/MObjString.h"
#include "../MDefaultFactory.h"
#include "../io/MLogger.h"
#include "../treedocument/MSaxTreeDocument.h"

MProperties* MProperties::gvInstance	= 0;

/**
 * runtime support
 */
MRtti MProperties::gvRtti = MRtti( "MProperties", MProperties::createInstance );

MProperties::MProperties()
{
}

MProperties::~MProperties()
{
	deleteAll();
}

void MProperties::store( String key, MObject* value )
{
	// remove and delete if nescessary
	MObject* ptObj = get( key );
	if( ptObj != 0 )
	{
		MObject* ptObj = remove( key );
		delete ptObj;
	}

	// invoke super->store
	MStr2ObjMap::store( key, value );
}

int MProperties::_getInt( String key, int defaultValue )
{
	MInt* myInt = (MInt*) get( key );
	if( myInt != 0 )
		return myInt->getValue();
	else
	{
		storeInt( key, defaultValue );
		return defaultValue;
	}
}

bool MProperties::_getBool( String key, bool defaultValue )
{
	MBool* myBool = (MBool*) get( key );
	if( myBool != 0 )
		return myBool->getValue();
	else
	{
		storeBool( key, defaultValue );
		return defaultValue;
	}
}

/**
 * returns the class name for runtime purpose
 */
IRtti* MProperties::getRtti() const
{
	return &gvRtti;
}

/**
 * querries for a specified interface
 */
void* MProperties::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MProperties*)this);
	else
		return MStr2ObjMap::getInterface( className );
}

String MProperties::toString() const
{
	return getRtti()->getClassName();
}

void MProperties::save( String fileName )
{
	ISeekableOutputStream* pOut = MResourceManager::getInstance()->openWriteFile( fileName );
	if( pOut )
	{
		MStreamPrinter printer( pOut );
		MTreeNode* ptNode = MStr2ObjMap::save();
		ptNode->toXml( &printer );
		delete ptNode;
		MResourceManager::getInstance()->close( pOut );
	}
}

void MProperties::load( String resourceName )
{
	MLogger::logMessage( "<mproperties::load resource=\"%s\"/>\n", resourceName.getData() );

	MSaxTreeDocument doc;
	doc.parseResource( resourceName );
	MTreeNode* ptNode = doc.getRoot();

	if( ptNode )
	{
		if( strcmp( ptNode->getName(), ELEM_OBJECT ) == 0 )
			MStr2ObjMap::load( ptNode );
		else
		{
			String msg("");
			msg.Format(
				"Could not load properties. Document element: \"%s\"",
				ptNode->getName().getData() );
			MLogger::logWarning( "Error loading \"" + resourceName + "\"" );
		}
	}
	else
		MLogger::logWarning( "Error loading properties. Could not find root node." );
}

MProperties* MProperties::getInstance()
{
	if( gvInstance == 0 )
		gvInstance = new MProperties();

	return gvInstance;
}

void MProperties::release()
{
	if( gvInstance )
	{
		delete gvInstance;
		gvInstance = 0;
	}
}

void MProperties::loadProperties( String resourceName )
{
	getInstance()->load( resourceName );
}

void MProperties::saveProperties( String resourceName )
{
	getInstance()->save( resourceName );
}

void MProperties::storeInt( String key, int value )
{
	getInstance()->store( key, new MInt( value ) );
}

int MProperties::getInt( String key, int defaultValue )
{
	return getInstance()->_getInt( key, defaultValue );
}

void MProperties::storeBool( String key, bool value )
{
	getInstance()->store( key, new MBool( value ) );
}

int MProperties::getInt( String key, int defaultValue, int min, int max )
{
	MInt* myInt = (MInt*) getInstance()->get( key );
	if( myInt != 0 )
	{
		int back = myInt->getValue();
		if( back < min )
			back = min;
		else if( back > max )
			back = max;
		return back;
	}
	else
	{
		storeInt( key, defaultValue );
		return defaultValue;
	}
}

void MProperties::storeString( String key, String value )
{
	getInstance()->store( key, new MObjString( value ) );
}

String MProperties::getString( String key, String defaultValue )
{
	MObjString* myString = (MObjString*) getInstance()->get( key );
	if( myString != 0 )
		return myString->toString();
	else
	{
		storeString( key, defaultValue );
		return defaultValue;
	}
}

MObject* MProperties::createInstance()
{
	return getInstance();
}