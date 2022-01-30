/**
 * MLabel (C)2000-2003 MRoc
 */

#include "MLabel.h"
#include <framework/primitive/MObjString.h>

/** runtime type info */
MRtti MLabel::gvRtti = MRtti( "MLabel", MLabel::createInstance ); 

/** constructor */
MLabel::MLabel()
	: MObjectLabel( new MObjString(), MColorMap::BLACK, MColorMap::WHITE )
{
}

/** constructor */
MLabel::MLabel( String text, MColor textColor, MColor backGroundColor )
	: MObjectLabel( new MObjString( text ), textColor, backGroundColor )
{
}

/** destructor */
MLabel::~MLabel()
{
	SAFE_DELETE( ivPtObject );
}

/** sets the text to be rendered */
void MLabel::setText( String text )
{
	((MObjString*)ivPtObject)->setData( text );
	repaint();
}

/** returns the displayed text */
String MLabel::getText()
{
	return ivPtObject->toString();
}

/** loads from the given tree node */
void MLabel::load( MTreeNode* ptNode )
{
	MObjectLabel::load( ptNode );
	setText( ptNode->getAttribute( "text" ) );
}

/** stores as tree node */
MTreeNode* MLabel::save()
{
	MTreeNode* pBack = MObjectLabel::save();
	pBack->setAttribute( "text", getText() );
	return pBack;
}

/** creates an instance of this class */
MObject* MLabel::createInstance()
{
	return new MLabel();
}

/** returns the runtime type info */
IRtti* MLabel::getRtti() const
{
	return &gvRtti;
}

/** query interface */
void* MLabel::getInterface( const String& className ) const
{
	if( className == "MLabel" )
		return (void*) ((MLabel*)this);
	else
		return MWnd::getInterface( className );
}