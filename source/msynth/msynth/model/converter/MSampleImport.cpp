#include "MSampleImport.h"
#include <gui/dialog/MMessageBox.h>

/**
 * runtime support
 */
MRtti MSampleImport::gvRtti = MRtti( "MSampleImport", MSampleImport::createInstance );

/**
 * constructor
 */
MSampleImport::MSampleImport()
{
}

/**
 * destructor
 */
MSampleImport::~MSampleImport()
{
}

/**
 * query for interface
 */
void* MSampleImport::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MSampleImport*)this);
	if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	if( className == "INodeConverter" )
		return (void*) ((INodeConverter*)this);
	else
		return MObject::getInterface( className );
}

/**
 * storing
 */
MTreeNode* MSampleImport::save()
{
	MTreeNode* ptBack = new MTreeNode( ELEM_OBJECT );
	ptBack->setAttribute( ATTR_CLASS, getRtti()->getClassName() );
	return ptBack;
}

/**
 * loading
 */
void MSampleImport::load( MTreeNode* ptNode )
{
}

/**
 * returns the runtime type information
 */
IRtti* MSampleImport::getRtti() const
{
	return &gvRtti;
}

/**
 * does the xml conversion
 */
void MSampleImport::convert( MTreeNode* ptNode )
{
	if( ptNode->hasAttribute( "value" ) )
	{
		String sampleName = ptNode->getAttribute( "value" );
		MWave ptWave;
		try
		{
			ptWave.loadFile( sampleName );
			
			MTreeNode* ptSampleElement = new MTreeNode( MObject::ELEM_OBJECT );
			ptSampleElement->setAttribute( MObject::ATTR_CLASS, "MWave" );
			ptSampleElement->setAttribute( "samplingrate", "44100" );
			ptSampleElement->setAttribute( "bitspersample", "16" );
			ptSampleElement->setAttribute( "channelcount", "1" );
			String base64sample = MXmlUtil::getBase64String( ptWave.getData(), ptWave.getDataLength() );
			ptSampleElement->setAttribute( "data", base64sample );
			((MTreeNode*)ptNode->getParent())->addChild( ptSampleElement );
		}
		catch( ... )
		{
			MMessageBox::showError( "Could not import sample \"" + sampleName + "\"!" );
		}
	}
}

/**
 * returns a instance of this class
 */
MObject* MSampleImport::createInstance()
{
	return new MSampleImport();
}
