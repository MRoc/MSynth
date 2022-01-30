#include "msamplebankentry.h"

/**
 * runtime support
 */
MRtti MSampleBankEntry::gvRtti = MRtti( "MSampleBankEntry", MSampleBankEntry::createInstance );

/**
 * constructor
 */
MSampleBankEntry::MSampleBankEntry() :
	MObject(),
	ivPtWave( 0 )
{
}

/**
 * constructor
 */
MSampleBankEntry::MSampleBankEntry( MWave* ptWave ) :
	MObject(),
	ivPtWave( ptWave )
{
}

/**
 * destructor
 */
MSampleBankEntry::~MSampleBankEntry()
{
	SAFE_DELETE( ivPtWave );
}

/**
 * returns the runtime type information
 */
IRtti* MSampleBankEntry::getRtti() const
{
	return &gvRtti;
}

/**
 * querries for a specified interface
 */
void* MSampleBankEntry::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MSampleBankEntry*)this);
	else
		return MObject::getInterface( className );
}

/**
 * returns the stored wave
 */
MWave* MSampleBankEntry::getWave()
{
	return ivPtWave;
}

/**
 * sets the stored wave
 */
void MSampleBankEntry::setWave( MWave* ptWave )
{
	ivPtWave = ptWave;
}

/**
 * returns the sample name
 */
String MSampleBankEntry::getName()
{
	return ivName;
}

/**
 * sets the sample name
 */
void MSampleBankEntry::setName( String name )
{
	ivName = name;
}

/**
 * creates an instance of this class
 */
MObject* MSampleBankEntry::createInstance()
{
	return new MSampleBankEntry();
}

/**
 * returns a string representatin
 */
String MSampleBankEntry::toString()
{
	String back("");
	int length = 0;
	if( ivPtWave )
		length = ivPtWave->getDataLength();
	back.Format(
		"name: %s length: %i",
		getName().getData(),
		length );
	return back;
}