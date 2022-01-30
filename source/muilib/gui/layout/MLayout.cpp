#include "MLayout.h"
#include <framework/treedocument/MSaxTreeDocument.h>
#include <framework/io/MFilePrinter.h>
#include <framework/io/MLogger.h>

MRtti MLayout::gvRtti = MRtti( "MLayout", MLayout::createInstance );

MLayout::MLayout() :
	MObject()
{
}

MLayout::~MLayout()
{
}

IRtti* MLayout::getRtti() const
{
	return &gvRtti;
}

void* MLayout::getInterface( const String &className ) const
{
	if( className == "MLayout" )
		return (void*) ((MLayout*)this);
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else
		return MObject::getInterface( className );
}

void MLayout::doLayout( MWndCollection* ptWnd )
{
	MSize size = ptWnd->getSize();
	ivRoot.setNamespace( ptWnd->getNamespace() );
	if( ivRoot.getChildCount() > 0 )
		ivRoot.getChild( 0 )->doLayout( ptWnd, MRect( MPoint(0,0), size ) );
	ptWnd->repaint();
}

void MLayout::load( MTreeNode* ptRoot )
{
	ASSERT( ptRoot->getName() == MObject::ELEM_OBJECT );
	ASSERT( ptRoot->getAttribute( MObject::ATTR_CLASS ) == getRtti()->getClassName() );

	MTreeNodeIterator iter = ptRoot->getIterator();
	if( iter.hasNext() )
	{
		MTreeNode* ptNode = (MTreeNode*) iter.nextTreeNode();
		MObject* ptObj = MDefaultFactory::getInstance()->createObject( ptNode );
		if( ptObj )
		{
			MLayoutLeaf* ptLeaf = (MLayoutLeaf*) ptObj->getInterface( "MLayoutLeaf" );
			if( ptLeaf )
				ivRoot.addChild( ptLeaf );
			else
				delete ptObj;
		}
	}
	else
	{
		MLogger::logError(
			"MLayout loading found no suitable kids" );
	}
}

MTreeNode* MLayout::save()
{
	MTreeNode* ptBack = new MTreeNode( MObject::ELEM_OBJECT );
	ptBack->setAttribute( MObject::ATTR_CLASS, getRtti()->getClassName() );
	ptBack->addChild( ((ISerializeable*)ivRoot.getInterface( "ISerializeable" ))->save() );
	return ptBack;
}

void MLayout::load( String fileName )
{
	MSaxTreeDocument doc;
	try
	{
		doc.parseResource( fileName );
		MTreeNode* ptRoot = doc.getRoot();
		if( ptRoot )
			load( ptRoot );
		else
			MLogger::logError( "MLayout could not load layout " + fileName );
	}
	catch( MException ae )
	{
		MLogger::logError( "%s\n", ae.getExceptionDescripton() );
	}
}

void MLayout::store( String fileName )
{
	MTreeNode* ptNode = this->save();
	MFilePrinter out;
	try
	{
		out.open( fileName );
		ptNode->toXml( &out );
		out.close();
	}
	catch( MException ae )
	{
		MLogger::logError( "%s\n", ae.getExceptionDescripton() );
	}
	delete ptNode;
}

MObject* MLayout::createInstance()
{
	return new MLayout();
}

MLayoutRoot* MLayout::getRoot()
{
	return &ivRoot;
}