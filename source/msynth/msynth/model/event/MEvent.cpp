#include "MEvent.h"

/**
 * the runtime type info
 */
MRtti MEvent::gvRtti = MRtti( "MEvent", 0 );

/**
 * constructor
 */
MEvent::MEvent( MEvent::Type t ) :
	ivType( t )
{
}

/**
 * destructor
 */
MEvent::~MEvent()
{
}

/**
 * returns the type of the event
 */
MEvent::Type MEvent::getType()
{
	return ivType;
}

/**
 * returns the runtime type info
 */
IRtti* MEvent::getRtti() const
{
	return &gvRtti;
}

/**
 * returns a pointer to the specified interface if available
 */
void* MEvent::getInterface( const String &className ) const
{
	if( className == MEvent::gvRtti.getClassName() )
		return (void*) ((MEvent*) this );
	if( className == "ISerializeable" )
		return (void*) ((ISerializeable*) this );
	else
		return MObject::getInterface( className );
}

/**
 * saves the event
 */
MTreeNode* MEvent::save()
{
	MTreeNode* ptBack = new MTreeNode( MObject::ELEM_OBJECT );
	ptBack->setAttribute( MObject::ATTR_CLASS, getRtti()->getClassName() );
	ptBack->setAttribute( "type", MInt::toString( (int)ivType ) );
	return ptBack;
}

/**
 * loads the event
 */
void MEvent::load( MTreeNode* ptNode )
{
}

/**
 * returns a string representation of the note event
 */
String MEvent::toString()
{
	String back = "<MEvent type=\"";

	switch( this->ivType )
	{
	case NOTE_ON: back += "NOTE_ON"; break;
	case NOTE_OFF: back += "NOTE_OFF"; break;
	case RESET: back += "RESET"; break;
	case PORTAMENTO: back += "PORTAMENTO"; break;
	case FALLDOWN: back += "FALLDOWN"; break;
	}
	back += "\"/>";
	
	return back;
}