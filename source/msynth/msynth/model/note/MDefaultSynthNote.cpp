#include "MDefaultSynthNote.h"

MDefaultSynthNote::MDefaultSynthNote() :
	MDefaultNote(),
	ivVolume( 0.8f ),
	ivValue( 0 )
{
}

MDefaultSynthNote::MDefaultSynthNote( MDefaultSynthNote* ptNote ) :
	MDefaultNote( ptNote )
{
	ivVolume = ptNote->getVolume();
	ivValue = ptNote->getValue();
}

MDefaultSynthNote::~MDefaultSynthNote()
{
}

void MDefaultSynthNote::setValue( int value )
{
	ivValue = value;
}

int MDefaultSynthNote::getValue()
{
	return ivValue;
}

FP MDefaultSynthNote::getVolume()
{
	return ivVolume;
}

void MDefaultSynthNote::setVolume( FP volume )
{
	ivVolume = volume;
}

bool MDefaultSynthNote::isOverlapping( IAbstractNote *note )
{
	bool back = false;

	int start = getIndex();
	int stop = getStopsAt();
	int startRef = ((MDefaultSynthNote*)note)->getIndex();
	int stopRef =  ((MDefaultSynthNote*)note)->getStopsAt();

	if( (start <= startRef && stop >= startRef) ||
		(start <= stopRef && stop >= stopRef) )
	{
		back = true;
	}
	
	return back;
}

bool MDefaultSynthNote::wouldOverlapp( int index, unsigned int length )
{
	bool back = false;

	unsigned int startRef = getIndex();
	unsigned int stopRef = getStopsAt();
	unsigned int start = index;
	unsigned int stop =  index + length - 1;

	if( (start <= startRef && stop >= startRef) ||
		(start <= stopRef && stop >= stopRef) ||
		(start >= startRef && stop <= stopRef ) )
	{
		back = true;
	}
	
	return back;
}

MTreeNode* MDefaultSynthNote::save()
{
	MTreeNode* ptBack = new MTreeNode( "dsn" );
	ptBack->setAttribute( "startsAt", MInt::toString( int( getIndex() ) ) );
	ptBack->setAttribute( "duration", MInt::toString( int( getDuration() ) ) );
	ptBack->setAttribute( "value", MInt::toString( int( getValue() ) ) );
	ptBack->setAttribute( "volume", MFloat::toString( getVolume() ) );
	return ptBack;
}

void MDefaultSynthNote::load( MTreeNode* parentNode )
{
	setIndex(
		MInt::parse(
			parentNode->getAttribute( "startsAt" ),
			0,
			MInt::MAX_INT,
			0 ) );

	setDuration(
		MInt::parse(
			parentNode->getAttribute( "duration" ),
			1,
			MInt::MAX_INT,
			1 ) );

	setValue(
		MInt::parse(
			parentNode->getAttribute( "value" ),
			0,
			MInt::MAX_INT,
			0 ) );

	setVolume(
		MFloat::parse(
			parentNode->getAttribute( "volume" ),
			0.0f,
			1.0f,
			0.8f ) );
}

bool MDefaultSynthNote::needsFalldown()
{
	bool back = true;

	MDefaultNote* ptNote = (MDefaultNote*) getPrev();

	while( ptNote != 0 )
	{
		if( ptNote->isOverlapping( this ) )
			back = false;
		ptNote = (MDefaultNote*) ptNote->getPrev();
	}

	return back;
}

bool MDefaultSynthNote::existsOverlappingAtEnd()
{
	MDefaultNote* ptNote = (MDefaultNote*) getNext();
	int stopsAt = getStopsAt();
	while( ptNote != 0 )
	{
		if( ptNote->getIndex() <= stopsAt &&
			ptNote->getStopsAt() > stopsAt )
		{
			return true;
		}
		ptNote = (MDefaultNote*) ptNote->getNext();
	}

	return false;
}

MDefaultNote* MDefaultSynthNote::getCopy()
{
	return 	new MDefaultSynthNote( this );
}

MDefaultNote* MDefaultSynthNote::createNewInstance()
{
	return new MDefaultSynthNote();
}