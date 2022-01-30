#include "MWndCollection.h"
#include <framework/MDefaultFactory.h>
#include <framework/io/MLogger.h>
#include <framework/MNativeTypes.h>
#include <framework/treedocument/MTreeNodeIterator.h>

/** runtime type info */
MRtti MWndCollection::gvRtti = MRtti( "MWndCollection", MWndCollection::createInstance );

/** constuctor */
MWndCollection::MWndCollection() :
	ivPtNamespace( 0 )
{
}

/** destructor */
MWndCollection::~MWndCollection()
{
	SAFE_DELETE( ivPtNamespace );

	size_t count = ivChilds.size();
	for( size_t i=0;i<count;i++ )
		delete ivChilds[ i ];
	ivChilds.clear();
}

/** adds a child to the window */
void MWndCollection::addChild( MWnd* pWnd )
{
	ASSERT( pWnd );
	ASSERT( getIndex( pWnd ) == -1 );

	if( pWnd->ivPtParent )
		((MWndCollection*)pWnd->ivPtParent)->removeChild( pWnd );

	ivChilds.push_back( pWnd );
	//pWnd->ivPtParent = this;
	pWnd->setParent( this ); // to be able to override setParent
}

/** removes a child to the window */
void MWndCollection::removeChild( MWnd* pWnd )
{
	ASSERT( pWnd );
	ASSERT( getIndex( pWnd ) != -1 );

	int index = getIndex( pWnd );
	if( index != -1 )
	{
		pWnd->ivPtParent = 0;
		ivChilds.erase( ivChilds.begin() + index );
	}
	else
		MLogger::logError( "MWndCollection::removeChild: child does not exist" );
}


/** returns the number of childs */
unsigned int MWndCollection::getChildCount()
{
	return (unsigned int) ivChilds.size();
}

/** returns the number of childs */
MWnd* MWndCollection::getChild( unsigned int index )
{
	ASSERT( index < getChildCount() );
	return ivChilds[ index ];
}

/** returns the index of the given window */
int MWndCollection::getIndex( MWnd* pWnd )
{
	ASSERT( pWnd );

	int back = -1;
	size_t count = ivChilds.size();
	for( size_t i=0;i<count&&back==-1;i++ )
		if( ivChilds[ i ] == pWnd )
			back = (int) i;
	return back;
}


/** returns the runtime type info */
IRtti* MWndCollection::getRtti() const
{
	return &gvRtti;
}

/** query interface */
void* MWndCollection::getInterface( const String& className ) const
{
	if( className == "MWndCollection" ) return (void*) ((MWndCollection*)this);
	else return MWnd::getInterface( className );
}

/** sets size and does layout */
void MWndCollection::setSize( MSize size )
{
	MWnd::setSize( size );
	doLayout();
}

/** sets rect and does layout */
void MWndCollection::setRect( MRect rect )
{
	MWnd::setRect( rect );
	doLayout();
}

/** paint the window. the rect should be in window coordinates */
void MWndCollection::paint( IGraphics* pGraphics, const MRect& rect )
{
	MWnd::paint( pGraphics, rect );

	/*unsigned int gridX = 10;
	unsigned int gridY = 10;
	int stepsX = rect.getWidth() / gridX;
	int stepsY = rect.getHeight() / gridY;

	for( int i=0;i<stepsX;i++ )
		pGraphics->drawLine( i*gridX, 0, i*gridX, rect.getHeight(), RGB( 50, 50, 50 ) );

	for( int i=0;i<stepsY;i++ )
		pGraphics->drawLine( 0, i*gridY, rect.getWidth(), i*gridY, RGB( 50, 50, 50 ) );

	pGraphics->drawRect( MRect( 0, 0, rect.getWidth(), rect.getHeight() ), RGB( 255, 0, 0 ) );*/
}

/** loads from the given tree node */
void MWndCollection::load( MTreeNode* ptNode )
{
	if( ptNode )
	{
		MWnd::load( ptNode );
		if( ptNode->getAttribute( "namespace" ) == "true" )
			ivPtNamespace = new MWndNamespace( this );
		MTreeNode* childNode = ptNode->getFirstNode( "children" );
		if( childNode )
		{
			MTreeNodeIterator iter = childNode->getIterator();
			while( iter.hasNext() )
			{
				MTreeNode* pWndNode = iter.nextTreeNode();
				MObject* pObj = MDefaultFactory::getInstance()->createObject( pWndNode->getAttribute( ATTR_CLASS ) );
				if( pObj )
				{
					MWnd* pWnd = (MWnd*) pObj->getInterface( "MWnd" );
					if( pWnd )
					{
						addChild( pWnd );
						pWnd->load( pWndNode );
					}
					else
					{
						delete pObj;
						MLogger::logWarning( "Instantiated object is not of class \"MWnd\"" );	
					}
				}
				else
					MLogger::logWarning( "Could not instantiate object of class \"%s\"",
					pWndNode->getAttribute( ATTR_CLASS ).getData() );
			}
		}
	}
	else
		MLogger::logWarning( "MWndCollection::load failed: no node provided\n" );
}

/** stores as tree node */
MTreeNode* MWndCollection::save()
{
	MTreeNode* pBack = MWnd::save();
	if( pBack && getChildCount() > 0 )
	{
		MTreeNode* pChildNode = new MTreeNode( "children" );
		pBack->addChild( pChildNode );
		unsigned int count = getChildCount();
		for( unsigned int i=0;i<count;i++ )
		{
			ISerializeable* pSer = (ISerializeable*) getChild( i )->getInterface( "ISerializeable" );
			if( pSer )
				pChildNode->addChild( pSer->save() );
		}
	}
	return pBack;
}

/** creates an instance of this class */
MObject* MWndCollection::createInstance()
{
	return new MWndCollection();
}

/** returns, if available, the container's namespace */
MWndNamespace* MWndCollection::getNamespace()
{
	return ivPtNamespace;
}