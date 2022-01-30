/**
 * (C)2001-2003 MRoc
 */
#include "MNoteInspector.h"

#define DEFAULT_NOTE_WIDTH 20

MNoteInspector::MNoteInspector( MBar* ptBar, MSize* ptPartSize ) :
	MyInteractiveWnd(),
	ivPtBar( ptBar ),
	ivPtSize( ptPartSize ),
	ivPtSelectedInspector( 0 )
{
	ASSERT( ptBar );
	ASSERT( ptPartSize );

	setBkColor( MColorMap::BK_COLOR3 );
}

MNoteInspector::~MNoteInspector()
{
	MToolTipCtrl::getInstance()->hideToolTipInfo();
	for( InspectorListIter i=ivPtInspectors.begin();
		i!= ivPtInspectors.end() ;i++ )
		(*i)->setContainingWnd( 0 );
}

void MNoteInspector::paint( IGraphics* pG, const MRect &rect )
{
	pG->fillRect( rect, ivBkColor );

	unsigned int scale = 10;
	unsigned int height = rect.getHeight() / scale;
	for( unsigned int i=0;i<scale;i++ )
	{
		int y = height * i;
		pG->drawLine( 0, y, rect.getWidth(), y, MColorMap::BK_COLOR4 );
	}

	pG->drawLine( 0, 0, rect.getWidth(), 0, MColorMap::FG_COLOR3 );
	if( ivPtSelectedInspector )
	{
		ivPtSelectedInspector->setSize( rect.getSize() );

		MPoint target = getScrollPos();
		pG->setOffset( pG->getOffset() - target );
		ivPtSelectedInspector->paint( pG, rect );
		pG->setOffset( pG->getOffset() + target );
	}
}

void MNoteInspector::setBar( MBar* ptBar )
{

	ivPtBar = ptBar;
	for( InspectorListIter i=ivPtInspectors.begin();
		i!= ivPtInspectors.end() ;i++ )
		(*i)->setBar( ptBar );	
}

MBar *MNoteInspector::getBar()
{
	return ivPtBar;
}

void MNoteInspector::setPartSize( MSize* ptSize )
{
	ivPtSize = ptSize;
	for( InspectorListIter i=ivPtInspectors.begin();
		i!= ivPtInspectors.end() ;i++ )
		(*i)->setPartSize( ivPtSize );
}

MSize* MNoteInspector::getPartSize()
{
	return ivPtSize;
}

void MNoteInspector::onMouseDown( MMouseEvent* anEvent )
{
	MyInteractiveWnd::onMouseDown( anEvent );
	if(
		anEvent->getButton() == MMouseEvent::BUTTON1 &&
		ivPtSelectedInspector &&
		ivPtSelectedInspector->onMouseDown( anEvent ) )
	{
		repaint();
		if( ivPtSelectedInspector->getForwardEvents() )
			fireChange();
	}
}

void MNoteInspector::onMouseUp( MMouseEvent* anEvent )
{
	if( anEvent->getButton() == MMouseEvent::BUTTON1 && ivPtSelectedInspector )
	{
		if( ivPtSelectedInspector->onMouseUp( anEvent ) )
		{
			repaint();
			if( ivPtSelectedInspector->getForwardEvents() )
				fireChange();
		}
		MToolTipCtrl::getInstance()->hideToolTipInfo();
	}
	MyInteractiveWnd::onMouseUp( anEvent );
}

void MNoteInspector::onMouseMove( MMouseEvent* anEvent )
{
	MyInteractiveWnd::onMouseMove( anEvent );

	if( getMouseDownLeft() && ivPtSelectedInspector && getEnabled() )
	{
		if( getMouseIn() ) // is mouse in ?
		{
			if( ivPtSelectedInspector->onMouseMoved( anEvent ) ) // redraw ?
			{
				repaint();
				if( ivPtSelectedInspector->getForwardEvents() ) // forward changes ?
					fireChange();
			}

			// update tooltip
			ToolTipInfo toolTip = ivPtSelectedInspector->getToolTip( anEvent->getPoint() );
			if( toolTip.show )
			{
				MToolTipCtrl::getInstance()->showToolTipInfo(
					this,
					toolTip.toolTipText,
					toolTip.toolTipPoint.getX(),
					toolTip.toolTipPoint.getY() );
			}
			else
				MToolTipCtrl::getInstance()->hideToolTipInfo();
		}
		else
		{
			MToolTipCtrl::getInstance()->hideToolTipInfo();
		}
	}
}

void MNoteInspector::onMouseHover( MHoverEvent* anEvent )
{
	MyInteractiveWnd::onMouseHover( anEvent );
	repaint();
}

void MNoteInspector::onMouseLeave( MHoverEvent* anEvent )
{
	MyInteractiveWnd::onMouseLeave( anEvent );
	repaint();
	MToolTipCtrl::getInstance()->hideToolTipInfo();
}

void MNoteInspector::addInspector( IBarInspector* ptInspector )
{
	ASSERT( ptInspector );

	ivPtInspectors.push_back( ptInspector );
	ptInspector->setPartSize( ivPtSize );
	ptInspector->setContainingWnd( this );
	ptInspector->setBar( ivPtBar );
}

void MNoteInspector::removeInspector( IBarInspector* ptInspector )
{
	for( InspectorListIter i=ivPtInspectors.begin();
		i!= ivPtInspectors.end() ;i++ )
		if( (*i) == ptInspector )
		{
			ivPtInspectors.erase( i );
			ptInspector->setPartSize( ivPtSize );
			ptInspector->setContainingWnd( 0 );
			break;
		}
}

IBarInspector* MNoteInspector::getInspectorByName( String name )
{
	IBarInspector* back = 0;
	for( InspectorListIter i=ivPtInspectors.begin();
		i!= ivPtInspectors.end() ;i++ )
		if( (*i)->getName() == name )
		{
			back = (*i);
			break;
		}
	ASSERT( back );
	return back;
}

void MNoteInspector::selectInspectorByName( String name )
{
	setSelectedInspector( getInspectorByName( name ) );
}

void MNoteInspector::setSelectedInspector( IBarInspector* ivPtInspector )
{
	ivPtSelectedInspector = ivPtInspector;
	repaint();
}

void MNoteInspector::addListener( INoteInspectorListener* pListener )
{
	ivListeners.addListener( pListener );
}

void MNoteInspector::removeListener( INoteInspectorListener* pListener )
{
	ivListeners.removeListener( pListener );
}

void MNoteInspector::fireChange()
{
	unsigned int count = ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((INoteInspectorListener*)ivListeners.getListener( i ))->onInspectorChanged();
}
