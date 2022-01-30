#include "MAutomationEventList.h"


MAutomationEventList::MAutomationEventList() :
	ivLength( 16 ),
	ivData( 0 ),
	ivPtTarget( 0 ),
	ivSerId( 0 )
{
	init();
}

MAutomationEventList::MAutomationEventList( unsigned int length ) :
	ivLength( length ),
	ivData( 0 ),
	ivPtTarget( 0 ),
	ivSerId( 0 )
{
	init();
}

MAutomationEventList::MAutomationEventList( unsigned int length, IEventTarget* ptTarget ) :
	ivLength( length ),
	ivData( 0 ),
	ivPtTarget( ptTarget ),
	ivSerId( 0 )
{
	init();
}

MAutomationEventList::MAutomationEventList( MAutomationEventList* toClone ) : 
	ivLength( toClone->getLength() ),
	ivData( 0 ),
	ivPtTarget( toClone->getTarget() ),
	ivSerId( 0 )
{
	ASSERT( toClone );
	init();
	unsigned int count = toClone->getLength();
	for( unsigned int i=0;i<count;i++ )
		if( toClone->getItem( i ) )
			setItem( toClone->getItem( i )->clone(), i );
}

MAutomationEventList::MAutomationEventList( unsigned int length, IAutomationEvent** data,
	IEventTarget* ptTarget ) :
	ivLength( length ),
	ivData( data ),
	ivPtTarget( ptTarget ),
	ivSerId( 0 )
{
}

MAutomationEventList::~MAutomationEventList()
{
	clear();
	delete ivData;
}

void MAutomationEventList::setTarget( IEventTarget* ptTarget )
{
	ivPtTarget = ptTarget;
}

IEventTarget* MAutomationEventList::getTarget()
{
	return ivPtTarget;
}

// updates the length of a automation event list, if the new length
// is less than ther old one, all events not used anymore are released,
void MAutomationEventList::setLength( unsigned int newLength )
{
	IAutomationEvent** newArray = new IAutomationEvent*[ newLength ];
	if( newLength < ivLength )
	{
		for( unsigned int i=0;i<newLength;i++ )
			newArray[ i ] = ivData[ i ];
		for( i=newLength;i<ivLength;i++ )
			if( ivData[ i ] )
				ivData[ i ]->release();
	}
	else
	{
		unsigned int count = ivLength;
		for( unsigned int i=0;i<ivLength;i++ )
			newArray[ i ] = ivData[ i ];
		for( i=ivLength;i<newLength;i++ )
			newArray[ i ] = 0;
	}
	ivLength = newLength;
	delete ivData;
	ivData = newArray;
}

unsigned int MAutomationEventList::getLength()
{
	return ivLength;
}

// rescales the data field, if scaled down, events can be released
void MAutomationEventList::scale( unsigned int newLength )
{
	IAutomationEvent** newArray = new IAutomationEvent*[ newLength ];

	// reset array
	for( unsigned int i=0;i<newLength;i++ )
		newArray[ i ] = 0;

	if( newLength < ivLength )
	{
		int div = int(ivLength) / int(newLength);
		for( unsigned int i=0;i<ivLength;i++ )
			if( ivData[ i ] )
			{
				if( i%div==0 )
					newArray[ i/div ] = ivData[ i ];
				else
					ivData[ i ]->release();
			}
	}
	else
	{
		float div = float(newLength) / float(ivLength);
		for( unsigned int i=0;i<newLength;i++ )
		{
			int index = i/int(div);
			if( i%int(div)==0 && ivData[ index ] )
				newArray[ i ] = ivData[ index ];
		}
	}
	ivLength = newLength;
	delete ivData;
	ivData = newArray;
}

void MAutomationEventList::setItem( IAutomationEvent* ptEvent, unsigned int index )
{
	//TRACE( "<mautomationeventlist::setitem step=\"%i\" length=\"%i\"/>\n", index, ivLength );
	ASSERT( index < ivLength );
	if( ivData[ index ] )
		ivData[ index ]->release();
	ivData[ index ] = ptEvent;
}

IAutomationEvent* MAutomationEventList::getItem( unsigned int index )
{
	ASSERT( index < ivLength );
	return ivData[ index ];
}

unsigned int MAutomationEventList::getItemCount()
{
	unsigned int count = 0;
	for( unsigned int i=0;i<ivLength;i++ )
		if( ivData[ i ] )
			count++;
	return count;
}

void MAutomationEventList::removeItem( unsigned int index )
{
	if( ivData[ index ] )
		ivData[ index ]->release();
	ivData[ index ] = 0;
}

void MAutomationEventList::removeItem( IAutomationEvent* ptEvent )
{
	int index = getIndex( ptEvent );
	if( index != -1 )
		removeItem( index );
}

void MAutomationEventList::clear()
{
	for( unsigned int i=0;i<ivLength;i++ )
		if( ivData[ i ] )
		{
			ivData[ i ]->release();
			ivData[ i ] = 0;
		}
}

void MAutomationEventList::glow( IAutomationEventList* ptEventList )
{
	unsigned int newLength = ivLength + ptEventList->getLength();
	IAutomationEvent** newArray = new IAutomationEvent*[ newLength ];
	for( unsigned int i=0;i<ivLength;i++ )
		newArray[ i ] = ivData[ i ];
	for( i=ivLength;i<newLength;i++ )
	{
		unsigned int index = i - ivLength;
		IAutomationEvent* ptEvent = ptEventList->getItem( index );
		if( ptEvent )
			newArray[ i ] = ptEvent->clone();
		else
			newArray[ i ] = 0;
	}
	delete ivData;
	ivData = newArray;
	ivLength = newLength;
}

IAutomationEventList* MAutomationEventList::cut( unsigned int index )
{
	IAutomationEventList* newList =
		new MAutomationEventList( ivLength - index, ivPtTarget );

	IAutomationEvent** newArray = new IAutomationEvent*[ index ];
	for( unsigned int i=0;i<index;i++ )
		newArray[ i ] = ivData[ i ];
	for( i=index;i<ivLength;i++ )
		if( ivData[ i ] )
			newList->setItem( ivData[ i ], i - index );

	delete ivData;
	ivData = newArray;
	ivLength = index;
	return newList;
}

void MAutomationEventList::release()
{
	delete this;
}

IAutomationEventList* MAutomationEventList::clone()
{
	return new MAutomationEventList( this );
}

MTreeNode* MAutomationEventList::save()
{
	MTreeNode* ptBack = new MTreeNode( "automationeventlist" );
	ptBack->setAttribute( "target", MInt::toString( (int)ivPtTarget ) );

	unsigned int count = getLength();
	for( unsigned int i=0;i<count;i++ )
		if( ivData[ i ] )
			ptBack->addChild( ivData[ i ]->save( i ) );
	
	return ptBack;
}

void MAutomationEventList::load( MTreeNode* ptNode )
{
	int id =
		MInt::parse(
			ptNode->getAttribute( "target" ),
			-1 );
	if( id != -1 )
	{
		ivPtTarget =
			MTargetRegistry::getInstance()->getIdRegistry()->getTarget( id );
	}
	else
		ivSerId = 0;

	MTreeNodeIterator iter = ptNode->getIterator();
	while( iter.hasNext() )
	{
		MTreeNode* node = iter.nextTreeNode();
		if( strcmp( node->getName() , "automationevent" ) == 0 )
		{
			String eventTypeStr = node->getAttribute( "class" );
			if( strcmp( eventTypeStr, "MFloatEvent" ) == 0 )
			{
				IAutomationEvent *event = new MFloatEvent();
				event->load( node );
				int index =
					MInt::parse(
						node->getAttribute( "index" ),
						0,
						999999999,
						0 );
				setItem( event, index );
			}
		}
	}
}


void MAutomationEventList::init()
{
	if( ivData )
		delete ivData;
	ivData = new IAutomationEvent*[ ivLength ];
	for( unsigned int i=0;i<ivLength;i++ )
		ivData[ i ] = 0;
}

// returns the index of the given event, -1 if not found
int MAutomationEventList::getIndex( IAutomationEvent* ptEvent )
{
	int back = -1;
	for( unsigned int i=0;i<ivLength;i++ )
		if( ivData[ i ] == ptEvent )
		{
			back = i;
			break;
		}
	return back;
}
