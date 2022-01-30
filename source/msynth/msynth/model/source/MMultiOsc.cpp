/*

	MMultiOsc (C)2001 MRoc hifiShock

	a synchronized oscilator containing other oscilators.

*/

#include "MMultiOsc.h"

/**
 * the runtime type info
 */
MRtti MMultiOsc::gvRtti = MRtti( "MMultiOsc", MMultiOsc::createInstance );

/**
 * constructor
 */
MMultiOsc::MMultiOsc() : MOsc(),
	ivPtTracker( 0 ),
	ivOscCount( 0 ),
	ivOsc( 0 )
{
}

/**
 * destructor
 */
MMultiOsc::~MMultiOsc()
{
	deleteContent();
}

/**
 * deletes all osc and frees reserved memory
 */
void MMultiOsc::deleteContent()
{
	MSynchronize cls( &ivCriticalSection );

	if( ivOsc )
	{
		for( unsigned int i=0;i<ivOscCount;i++ )
			SAFE_DELETE( ivOsc[ i ] );
		delete ivOsc;
		ivOsc = 0;
	}
}

/**
 * performs all oscs goNext and adds/subbs the result
 */
void MMultiOsc::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt )
{
	MSynchronize cls( &ivCriticalSection );

	MSoundBuffer *oscBuffer = new MSoundBuffer( 1, stopAt - startFrom );
	for( unsigned int i=0;i<ivOscCount;i++ )
	{
		//oscBuffer->zeroBuffer();
		ivOsc[ i ]->goNext( oscBuffer, 0, stopAt - startFrom );

		FP *pt = buffer->getData(0) + startFrom;
		FP *ptStop = buffer->getData(0) + stopAt;
		FP *oscData = oscBuffer->getData(0);
		for( ;pt<ptStop; pt++,oscData++ )
			(*pt) += (*oscData);
	}
	delete oscBuffer;
}				

/**
 * sets the frequence in all osc
 */
void MMultiOsc::setFreq( FP freq )
{
	MSynchronize cls( &ivCriticalSection );

	ivFreq = freq;
	for( unsigned int i=0;i<ivOscCount;i++ )
		ivOsc[ i ]->setFreq( freq );
}

/**
 * adds a pitch to all oscis
 */
void MMultiOsc::addPitch( FP delta )
{
	MSynchronize cls( &ivCriticalSection );

	ivFreq += delta;

	for( unsigned int i=0;i<ivOscCount;i++ )
		ivOsc[ i ]->addPitch( delta );
}

/**
 * resets all oscis
 */
void MMultiOsc::reset()
{
	MSynchronize cls( &ivCriticalSection );

	for( unsigned int i=0;i<ivOscCount;i++ )
		ivOsc[ i ]->reset( );
}

/**
 * triggers portamento in all oscis
 */
void MMultiOsc::triggerPortamento( FP targetFrequence, unsigned int portamentoLength )
{
	MSynchronize cls( &ivCriticalSection );

	MOsc::triggerPortamento( targetFrequence, portamentoLength );

	for( unsigned int i=0;i<ivOscCount;i++ )
		ivOsc[ i ]->triggerPortamento( targetFrequence, portamentoLength );
}

/**
 * stops portamento in all oscis
 */
void MMultiOsc::stopPortamento()
{
	MSynchronize cls( &ivCriticalSection );

	MOsc::stopPortamento();

	for( unsigned int i=0;i<ivOscCount;i++ )
		ivOsc[ i ]->stopPortamento();
}

/**
 * deletes all oscis and reserves mem for new one
 */
void MMultiOsc::setOscCount( int oscCount )
{
	MSynchronize cls( &ivCriticalSection );

	deleteContent();

	ivOscCount = oscCount;
	ivOsc = new MOsc*[ ivOscCount ];
	for( unsigned int i=0;i<ivOscCount;i++ )
	{
		ivOsc[ i ] = new MOscSaw();	
		if( ivPtTracker )
			ivOsc[ i ]->registerTargets( ivPtTracker );
	}
}

/**
 * returns the number of oscis this multiosci uses
 */
unsigned int MMultiOsc::getOscCount()
{
	return ivOscCount;
}

/**
 * sets the osci at the given index
 */
void MMultiOsc::setOscAt( MOsc* ptOsc, int index )
{
	MSynchronize cls( &ivCriticalSection );

	if( ivOsc[ index ] )
		delete ivOsc[ index ];

	ivOsc[ index ] = ptOsc;

	if( ivOsc[ index ] && ivPtTracker )
		ivOsc[ index ]->registerTargets( ivPtTracker );
}

/**
 * returns the osci at the given index
 */
MOsc* MMultiOsc::getOscAt( int index )
{
	return ivOsc[ index ];
}

/**
 * returns the classname
 */
String MMultiOsc::getName()
{
	return gvRtti.getClassName();
}

/**
 * returns the short userfriendly name
 */
String MMultiOsc::getShortName()
{
	return String( "MultiOsc" );
}

/**
 * saving
 */
MTreeNode* MMultiOsc::save()
{
	MTreeNode* ptBack = new MTreeNode( getType() );
	ptBack->setAttribute( "value", gvRtti.getClassName() );
	ptBack->setAttribute( "oscCount", MInt::toString( (int)getOscCount() ) );
	
	for( unsigned int i=0;i<ivOscCount;i++ )
		ptBack->addChild( ivOsc[ i ]->save() );

	return ptBack;
}

/**
 * loading
 */
void MMultiOsc::load( MTreeNode* parentNode )
{
	setOscCount(
		MInt::parse(
			parentNode->getAttribute( "oscCount" ),
			1,
			3,
			3 ) );

	int oscisFound = 0;
	MTreeNodeIterator iter = parentNode->getIterator();
	while( iter.hasNext() )
	{
		MTreeNode* node = iter.nextTreeNode();
		String nodeName = node->getName();
		if(	strcmp( nodeName, "sourceprocessor" ) == 0 )
		{
			String oscName = node->getAttribute( "value" );
			MObject* ptObject = MDefaultFactory::getInstance()->createObject( oscName );
			if( ptObject )
			{
				MOsc* ptSource = (MOsc*) ptObject->getInterface( "MOsc" );
				if( ptSource != NULL )
				{
					ptSource->load( node );
					setOscAt( ptSource, oscisFound++ );
				}
				else
					throw MException( "MMultiOsc: loaded object was not a osci!" );
			}
			else
				throw MException( "MMultiOsc: unknown classname found!" );
		}
		else
			TRACE( "<Unknown node found: %s>\n", nodeName );
	}
}

/**
 * returna the view id
 */
int MMultiOsc::getViewId()
{
	return DESC_VIEW_MULTIOSC;
}

/**
 * registers all automation targets available in this osci
 */
void MMultiOsc::registerTargets( IAutomationTracker* ptTracker )
{
	if( ! ivPtTracker )
		ivPtTracker = ptTracker;
	else
		ASSERT( ivPtTracker == ptTracker );

	MOsc::registerTargets( ivPtTracker );
	for( unsigned int i=0;i<ivOscCount;i++ )
		if( ivOsc[ i ] )
			ivOsc[ i ]->registerTargets( ivPtTracker );
}

/**
 * returns the runtime type info
 */
IRtti* MMultiOsc::getRtti() const
{
	return &gvRtti;
}

/**
 * create a instance of this class
 */
MObject* MMultiOsc::createInstance()
{
	MMultiOsc* newOsc = new MMultiOsc();
	newOsc->setOscCount( 3 );
	return newOsc;
}