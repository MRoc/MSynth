#include "MScrolledFrameWnd.h"
#include "..\scrollbar\MScrollPane.h"
#include <framework/io/MLogger.h>

MScrolledFrameWnd::MScrolledFrameWnd() :
	MFrameWnd()
{
	addChild( new MScrollPane() );
}

MScrolledFrameWnd::MScrolledFrameWnd( MWnd* ptRootPane ) :
	MFrameWnd()
{
	addChild( new MScrollPane() );
	setScrollWnd( ptRootPane );
}

MScrolledFrameWnd::~MScrolledFrameWnd()
{
}

void MScrolledFrameWnd::doLayout()
{
	if( getChildCount() > 0 )
		getChild( 0 )->setRect( MRect( MPoint(), getImpl()->getClientSize() ) );
	else
		MLogger::logWarning( "MScrolledFrameWnd::doLayout failed" );
}

void MScrolledFrameWnd::setScrollWnd( MWnd* pWnd )
{
	if( getChildCount() > 0 )
		((MScrollPane*)getChild( 0 ))->setScollWnd( pWnd );
	else
		MLogger::logWarning( "MScrolledFrameWnd::setScrollWnd failed" );
}