#include "MTreeLeaf.h"
#include "../io/MLogger.h"

/** the runtime type info */
MRtti MTreeLeaf::gvRtti = MRtti( "MTreeLeaf", MTreeLeaf::createInstance );

/** constructor */
MTreeLeaf::MTreeLeaf() :
	ivPtParent( 0 )
{
}

/** constructor */
MTreeLeaf::MTreeLeaf( const String& name ) :
	ivPtParent( 0 ),
	ivName( name )
{
	ASSERT( name != "" );
}

/** destructor */
MTreeLeaf::~MTreeLeaf()
{
}

/** sets the name of this element */
void MTreeLeaf::setName( const String& name )
{
	ivName = name;
	ASSERT( name != "" );
}

/** returns the name of this element */
const String &MTreeLeaf::getName() const
{
	return ivName;
}

/** returns the parent node */
MTreeLeaf* MTreeLeaf::getParent() const
{
	return this->ivPtParent;
}

/** sets the parent node */
void MTreeLeaf::setParent( MTreeLeaf* ptParent )
{
	this->ivPtParent = ptParent;
}

/** generates xml */
void MTreeLeaf::toXml( IPrintStream* ptOut )
{
	if( ivName != "" )
		ptOut->print( ivName );
}

/** queries for an interface */
void* MTreeLeaf::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return ((MTreeLeaf*)this);
	else
		return MObject::getInterface( className );
}

/** returns the runtime type info */
IRtti* MTreeLeaf::getRtti() const
{
	return &gvRtti;
}

/** creates an instance of this class */
MObject* MTreeLeaf::createInstance()
{
	return new MTreeLeaf();
}

/** returns the leaf/node type */
MTreeLeaf::LeafType MTreeLeaf::getType() const
{
	return LEAF;
}