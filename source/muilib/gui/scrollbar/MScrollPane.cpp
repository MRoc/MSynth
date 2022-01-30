#include "MScrollPane.h"
#include <framework/treedocument/MTreeDocumentUtils.h>
#include <framework/io/MLogger.h>

INIT_RTTI_CREATE( MScrollPane, "MScrollPane" );

MScrollPane::MScrollPane() :
	MWndCollection(),
	ivPtScrollWnd( 0 ),
	ivPtScrollbarHorizontal( 0 ),
	ivPtScrollbarVertical( 0 ),
	ivPtController( 0 )
{
	ivBkColor = MColorMap::BK_COLOR1;
	ivPtScrollbarHorizontal = new MScrollBar( MScrollBar::HORIZONTAL );
	ivPtScrollbarVertical = new MScrollBar( MScrollBar::VERTICAL );
	ivPtController = new MScrollBarController();
	ivPtController->setHorizontalScrollbar( ivPtScrollbarHorizontal );
	ivPtController->setVerticalScrollbar( ivPtScrollbarVertical );
	ivPtController->setAdditionalSize( MSize( 16, 16 ) );
	addChild( ivPtScrollbarHorizontal );
	addChild( ivPtScrollbarVertical );
}

MScrollPane::MScrollPane( MWnd* ptScrollWnd ) :
	MWndCollection(),
	ivPtScrollWnd( 0 ),
	ivPtScrollbarHorizontal( 0 ),
	ivPtScrollbarVertical( 0 ),
	ivPtController( 0 )
{
	ivBkColor = MColorMap::BK_COLOR1;
	ivPtScrollbarHorizontal = new MScrollBar( MScrollBar::HORIZONTAL );
	ivPtScrollbarVertical = new MScrollBar( MScrollBar::VERTICAL );
	ivPtController = new MScrollBarController();
	ivPtController->setHorizontalScrollbar( ivPtScrollbarHorizontal );
	ivPtController->setVerticalScrollbar( ivPtScrollbarVertical );
	ivPtController->setAdditionalSize( MSize( 16, 16 ) );
	addChild( ivPtScrollbarHorizontal );
	addChild( ivPtScrollbarVertical );
	setScollWnd( ptScrollWnd );
}

MScrollPane::~MScrollPane()
{
	SAFE_DELETE( ivPtController );

	removeChild( ivPtScrollbarHorizontal );
	removeChild( ivPtScrollbarVertical );

	SAFE_DELETE( ivPtScrollbarHorizontal );
	SAFE_DELETE( ivPtScrollbarVertical );
}

void MScrollPane::setScrollSize( MSize size )
{
	ivPtController->setScrollSizes( size, getSize() );
	doLayout();
}

void MScrollPane::doLayout()
{
	MRect rect = getRect();

	int width = (int) rect.getWidth();
	int height = (int) rect.getHeight();

	if( ivPtScrollbarHorizontal &&
		ivPtScrollbarVertical &&
		ivPtScrollWnd )
	{
		int vMax = ivPtScrollbarHorizontal->getModel()->getMax();
		int hMax = ivPtScrollbarVertical->getModel()->getMax();
		bool vert = true;
		bool horz = true;
		if( width >= vMax &&
			height >= hMax )
		{
			// disable both
			vert = horz = false;
		}
		else if(
			width < vMax &&
			height - 16 >= hMax )
		{
			// disable vertical
			vert = false;
		}
		else if(
			width - 16 >= vMax &&
			height < hMax )
		{
			// disable horizontal
			horz = false;
		}

		if( ivPtScrollbarVertical->getVisible() != vert )
			ivPtScrollbarVertical->setVisible( vert );
		if( ivPtScrollbarHorizontal->getVisible() != horz )
			ivPtScrollbarHorizontal->setVisible( horz );

		int xWidth = vert ? 16 : 0;
		int yWidth = horz ? 16 : 0;

		ivPtScrollbarHorizontal->setRect(
			MRect(
				0,
				height - yWidth,
				width - xWidth,
				yWidth ) );

		ivPtScrollbarVertical->setRect(
			MRect(
				width-xWidth,
				0,
				xWidth,
				height - yWidth ) );

		ivPtScrollWnd->setRect(
			MRect(
				0,
				0,
				width - xWidth,
				height - yWidth ) );

		ivPtScrollbarHorizontal->getModel()->setInner( width - xWidth );
		ivPtScrollbarVertical->getModel()->setInner( height - yWidth );
	}
}

MScrollBar* MScrollPane::getVerticalScrollbar()
{
	return ivPtScrollbarVertical;
}

MScrollBar* MScrollPane::getHorizontalScrollbar()
{
	return ivPtScrollbarHorizontal;
}

MScrollBarController* MScrollPane::getController()
{
	return ivPtController;
}

/** store ui state into treenode */
MTreeNode* MScrollPane::save()
{
	MTreeNode* pBack = MWnd::save();
	if( ivPtScrollWnd )
		pBack->addChild( ivPtScrollWnd->save() );
	return pBack;
}

/** load ui state from treenode */
void MScrollPane::load( MTreeNode* pNode )
{
	MWnd::load( pNode );
	MTreeNode* pViewNode = pNode->getFirstNode( MObject::ELEM_OBJECT );
	if( pViewNode )
	{
		MObject* pObj = MTreeDocumentUtils::createObject( pViewNode );
		if( pObj )
		{
			if( pObj->getInterface( "MWnd" ) )
				addChild( (MWnd*) pObj->getInterface( "MWnd" ) );
			else
			{
				MLogger::logWarning( "MScrollPane::load: loaded object is not a MWnd!" );
				delete pObj;
			}
		}
	}
	doLayout();
}

void MScrollPane::setScollWnd( MWnd* pWnd )
{
	if( ivPtScrollWnd )
	{
		ivPtController->removeScrollWnd( pWnd );
		removeChild( ivPtScrollWnd );
	}
	ivPtScrollWnd = pWnd;
	if( ivPtScrollWnd )
	{
		ivPtController->addScrollWnd( pWnd );
		addChild( ivPtScrollWnd );
	}
}

MWnd* MScrollPane::getScrollWnd()
{
	return ivPtScrollWnd;
}

void MScrollPane::paint( IGraphics* pG, const MRect& rect )
{
	pG->fillRect( rect, ivBkColor );
}