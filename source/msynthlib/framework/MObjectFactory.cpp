#include "MObjectFactory.h"

MObjectFactory::MObjectFactory()
{
}

MObjectFactory::~MObjectFactory()
{
}

void MObjectFactory::store( MRtti* ptRtti )
{
	ASSERT( ptRtti );
	ASSERT( ptRtti->getClassName().getData() );
	ASSERT( ptRtti->canCreate() );

	insert( StrRttiMap::value_type( ptRtti->getClassName(), ptRtti ) );
}

MRtti* MObjectFactory::get( String className )
{
	MRtti* back = 0;
	StrRttiIter i = find( className );
	if( i != end() )
		back = i->second;
	else
		TRACE( "WARNING: key %s not found\n", className );
	return back;
}

MObject* MObjectFactory::createObject( String className )
{
	//TRACE( "<mobjectfactory::createobject classname=\"%s\"/>\n", className.getData() );
	MRtti* ptRtti = get( className );
	if( ptRtti )
		return ptRtti->createInstance();
	else
		return 0;
}

MObject* MObjectFactory::createObject( MTreeNode* ptNode )
{
	MObject* ptBack = 0;
	String nodeName = ptNode->getName();
	if( nodeName == MObject::ELEM_OBJECT )
	{
		String className = ptNode->getAttribute( MObject::ATTR_CLASS );
		if( className != "" )
		{
			ptBack = createObject( className );
			if( ptBack )
			{
				ISerializeable* ptSer = (ISerializeable*) ptBack->getInterface( "ISerializeable" );
				if( ptSer )
					ptSer->load( ptNode );
				else
					ASSERT( false );
			}
			else
				ASSERT( false );
		}
	}
	return ptBack;
}