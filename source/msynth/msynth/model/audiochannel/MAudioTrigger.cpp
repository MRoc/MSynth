#include "maudiotrigger.h"

MAudioTrigger::MAudioTrigger() :
	MDefaultNote(),
	ivPtFile( 0 )
{
}

MAudioTrigger::MAudioTrigger( MAudioFile* ptFile ) :
	MDefaultNote(),
	ivPtFile( ptFile )
{
}

MAudioTrigger::MAudioTrigger( MAudioTrigger* ptTrigger ) :
	MDefaultNote( ptTrigger ),
	ivPtFile( ptTrigger->getFile() )
{
}

MAudioTrigger::~MAudioTrigger()
{
}

void MAudioTrigger::setFile( MAudioFile* ptFile )
{
	this->ivPtFile = ptFile;
}

MAudioFile* MAudioTrigger::getFile()
{
	return ivPtFile;
}

MDefaultNote* MAudioTrigger::getCopy()
{
	return new MAudioTrigger( this );
}

MDefaultNote* MAudioTrigger::createNewInstance()
{
	return new MAudioTrigger();
}

bool MAudioTrigger::isOverlapping( IAbstractNote *note )
{
	return wouldOverlapp( note->getStartsAt(), note->getDuration() );
}

bool MAudioTrigger::wouldOverlapp( int index, unsigned int length )
{
	return !
		(( index + ((int)(length)) <= ivIndex ) ||
		( index > getStopsAt() ));
}

MTreeNode* MAudioTrigger::save()
{
	MTreeNode* ptBack = new MTreeNode( "at" );
	ptBack->setAttribute( "startsAt", MInt::toString( (int)MListItem::getIndex() ) );
	ptBack->setAttribute( "duration", MInt::toString( (int)getDuration() ) );
	ptBack->setAttribute( "fileid", MInt::toString( (int)ivPtFile->getSerId() ) );
	return ptBack;
}

void MAudioTrigger::load( MTreeNode* node )
{
	ASSERT( node->getName() == "at" );

	MListItem::setIndex(
		MInt::parse(
			node->getAttribute( "startsAt" ),
			0,
			32000,
			0 ) );

	setDuration(
		MInt::parse( 
			node->getAttribute( "duration" ),
			1,
			32000,
			1 ) );

	int id = MInt::parse( node->getAttribute( "fileid" ), -1 );
	ivPtFile = (MAudioFile*) MIdMap::getInstance()->get( id );
	ASSERT( ivPtFile );
}
