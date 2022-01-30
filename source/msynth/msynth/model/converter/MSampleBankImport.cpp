#include "MSampleBankImport.h"

/**
 * runtime support
 */
MRtti MSampleBankImport::gvRtti = MRtti( "MSampleBankImport", MSampleBankImport::createInstance );

/**
 * constructor
 */
MSampleBankImport::MSampleBankImport()
{
}

/**
 * destructor
 */
MSampleBankImport::~MSampleBankImport()
{
}

/**
 * returns the runtime type info
 */
IRtti* MSampleBankImport::getRtti() const
{
	return &gvRtti;
}

/**
 * returns a string representation
 */
String MSampleBankImport::toString()
{
	return getRtti()->getClassName() + " is a class to import samplebanks and store them as base64";
}

/**
 * queries for a interface
 */
void *MSampleBankImport::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MSampleBankImport*)this);
	if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	if( className == "INodeConverter" )
		return (void*) ((INodeConverter*)this);
	else
		return MObject::getInterface( className );
}

/**
 * converts the given node
 */
void MSampleBankImport::convert( MTreeNode* ptNode )
{
	if( ptNode->getName() == "source" )
	{
		String sampleBankName = ptNode->getAttribute( "samplebank" );
		if( sampleBankName.GetLength() > 0 )
		{
			MSampleBank samplebank;
			samplebank.import( sampleBankName );
			
			ptNode->removeAttribute( "samplebank" );

			MTreeNodeIterator iter = ptNode->getIterator();
			unsigned int found = 0;
			while( iter.hasNext() )
			{
				MTreeNode* ptChild = iter.nextTreeNode();
				if( ptChild->getName() == "channel" )
				{
					if( found < samplebank.getSampleCount() )
					{
						MWave* ptWave = samplebank.getSample( found )->getWave();
						unsigned int dataLen = ptWave->getDataLength();
						if( dataLen && ptWave->getData() )
						{
							MTreeNode* ptSampleElement = new MTreeNode( MObject::ELEM_OBJECT );
							ptSampleElement->setAttribute( MObject::ATTR_CLASS, "MWave" );
							ptSampleElement->setAttribute( "samplingrate", "44100" );
							ptSampleElement->setAttribute( "bitspersample", "16" );
							ptSampleElement->setAttribute( "channelcount","1" );
							String base64sample = MXmlUtil::getBase64String( ptWave->getData(), dataLen );
							ptSampleElement->setAttribute( "data", base64sample );
							ptChild->addChild( ptSampleElement );
						}
					}
					found++;
				}
			}
		}
		else
		{
			TRACE( "WARNING: no \"samplebank\" attribute found!!" );
		}
	}
	else
	{
		TRACE( "WARNING: no \"source\" element found!!" );
	}
}

/**
 * storing
 */
MTreeNode* MSampleBankImport::save()
{
	MTreeNode* ptBack = new MTreeNode( ELEM_OBJECT );
	ptBack->setAttribute( ATTR_CLASS, getRtti()->getClassName() );
	return ptBack;
}

/**
 * loading
 */
void MSampleBankImport::load( MTreeNode* ptNode )
{
}

/**
 * create a instance of this class
 */
MObject* MSampleBankImport::createInstance()
{
	return new MSampleBankImport();
}