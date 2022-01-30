#include "MLayoutRoot.h"
#include <framework/io/MLogger.h>


/**
 * runtime typeinfo
 */
MRtti MLayoutRoot::gvRtti = MRtti( "MLayoutRoot", MLayoutRoot::createInstance );

MLayoutRoot::MLayoutRoot() :
	ivNamespace( 0 )
{
}

MLayoutRoot::~MLayoutRoot()
{
}

IRtti* MLayoutRoot::getRtti() const
{
	return &gvRtti;
}

void* MLayoutRoot::getInterface( const String &className ) const
{
	if( className == "MLayoutRoot" )
		return (void*) ((MLayoutRoot*)this);
	else
		return MLayoutNode::getInterface( className );
}

MObject* MLayoutRoot::createInstance()
{
	return new MLayoutRoot();
}

void MLayoutRoot::setNamespace( MWndNamespace* pNamespace )
{
	ivNamespace = pNamespace;
}

MWnd* MLayoutRoot::getWnd( const String &id )
{
	return (MWnd*) ivNamespace->getObj( id );
}

void MLayoutRoot::registerWnd( MWnd* pWnd, const String &id )
{
	ASSERT( ivNamespace );
	if( ivNamespace->getObj( id ) == 0 )
		ivNamespace->regObj( pWnd, id );
	else
		MLogger::logWarning( "MLayoutRoot::registerWnd failed" );
}