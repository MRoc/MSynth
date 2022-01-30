/*

	MDCGrid (C)2002-2003 MRoc hifiShock

	A ScrollPane displaying MDCControls,
	double buffered, controls can be created, dragged and edited.

*/

#include "MDCGrid.h"
#include <gui/MGuiUtils.h>
#include "../../../resource.h"
#include "../clipboard/MClipBoard.h"

MDCGrid::MDCGrid( MSize* ptPartSize ) :
	MyInteractiveWnd(),
	ivEditMode( SELECT_MODE ),
	ivPtEditControl( 0 ),
	ivMouseCaptured( false ),
	ivPtPartSize( ptPartSize ),
	ivMoveMode( false ),
	ivPtSelectionManager( 0 )
{
	ASSERT( ptPartSize );

	ivBkColor = MColorMap::BK_COLOR3;
	ivPtSelectionManager = new MDCControlSelectionManager();
}

MDCGrid::~MDCGrid()
{
	deleteControls();
	SAFE_DELETE( ivPtSelectionManager );
}

MSize MDCGrid::calculateScrollViewSize()
{
	ASSERT( ivPtPartSize );
	return
		MSize(
			getMaxCols() * ivPtPartSize->getWidth(),
			getMaxRows() * ivPtPartSize->getHeight() );
}

void MDCGrid::updateFromPartSize()
{
	unsigned int count = ivControls.size();
	for( unsigned int i=0;i<count;i++ )
		layoutControl( ivControls[ i ] );
	repaint();
}

void MDCGrid::setPartSize( MSize* ptSize )
{
	ivPtPartSize = ptSize;
}

MSize* MDCGrid::getPartSize()
{
	return ivPtPartSize;
}

void MDCGrid::setEditMode( MDCGrid::EditMode mode )
{
	ivEditMode = mode;
}

MDCGrid::EditMode MDCGrid::getEditMode()
{
	return ivEditMode;
}

int MDCGrid::GetIndexOfControl( MDCControl* ptControl )
{
	ASSERT( ptControl );
	int index = -1;
	unsigned int count = ivControls.size();
	for( unsigned int i=0;i<count;i++ )
		if( ivControls[ i ] == ptControl )
		{
			index = i;
			break;
		}

	return index;
}

void MDCGrid::startControlMoving( MDCControl *_ptControl )
{
	ASSERT( _ptControl );
 	ivPtEditControl = _ptControl;
	unsigned int count=ivPtSelectionManager->getSelectionCount();
	for( unsigned int i=0;i<count;i++ )
	{
		MDCControl* ptControl = ivPtSelectionManager->getSelectionAt( i );
		ptControl->setPoint(
			ptControl->getPosition().getPoint().sub( ivPtEditControl->getPosition().getPoint() ) );
	}
	ivMoveMode = true;
	ivMouseCaptured = true;
}

void MDCGrid::updateSelection( MRect rect )
{
	int count = ivControls.size();
	for( int i=0;i<count;i++ )
	{
		MDCControl* ptControl = ivControls[ i ];
		MRect pos = ptControl->getPosition();
		if( rect.contains( pos.getCenter() ) ||
			rect.contains( pos.getPoint() ) ||
			rect.contains( pos.getBottomRight() ) ||
			rect.contains( MPoint( pos.getBottomRight().getX(), pos.getY() ) ) ||
			rect.contains( MPoint( pos.getX(), pos.getBottomRight().getY() ) ) )
		{
			if( ! ivPtSelectionManager->isInSelection( ptControl ) )
				ivPtSelectionManager->addToSelection( ptControl );
		}
		else
		{
			int index;
			if( (index=ivPtSelectionManager->getIndexOfControl( ptControl )) != -1 )
				ivPtSelectionManager->removeFromSelection( index );
		}
	}
}

void MDCGrid::deleteControls()
{
	ivPtSelectionManager->resetSelection();

	unsigned int count = ivControls.size();
	for( unsigned int i=0;i<count;i++ )
		SAFE_DELETE( ivControls[ i ] );

	ivControls.clear();
}

void MDCGrid::onMouseDown( MMouseEvent* anEvent )
{
	ASSERT( anEvent );
	MyInteractiveWnd::onMouseDown( anEvent );
	if( anEvent->getButton() == MMouseEvent::BUTTON1 )
		ivPointMouseDown = anEvent->getPoint();
}

void MDCGrid::onMouseUp( MMouseEvent* anEvent )
{
	ASSERT( anEvent );
	MyInteractiveWnd::onMouseUp( anEvent );
	if( anEvent->getButton() == MMouseEvent::BUTTON1 )
		ivPointMouseUp = anEvent->getPoint();
}

void MDCGrid::onMouseMove( MMouseEvent* anEvent )
{
	MyInteractiveWnd::onMouseMove( anEvent );
}

void MDCGrid::onMouseDoubleClick( MMouseEvent* anEvent )
{
	MyInteractiveWnd::onMouseDoubleClick( anEvent );
}

int MDCGrid::getRow( MPoint point )
{
	return point.getY() / (int)ivPtPartSize->getHeight();
}

int MDCGrid::getCol( MPoint point )
{
	return point.getX() / (int)ivPtPartSize->getWidth();
}

void MDCGrid::layoutControl( MDCControl *_ptControl )
{
	ASSERT( _ptControl );
	int x = getCol( _ptControl ) * ivPtPartSize->getWidth();
	int y = getRow( _ptControl) * ivPtPartSize->getHeight();
	int width = getColSpan( _ptControl ) * ivPtPartSize->getWidth();
	int height = ivPtPartSize->getHeight();

	_ptControl->setPosition(
		MRect(
			x + 1,
			y + 1,
			width,
			height ) );
}

MDCControl* MDCGrid::getControlAt( const MPoint& point )
{
	MDCControl *control = 0;
	for( unsigned int i=0;i<ivControls.size();i++ )
		if( ivControls[i]->getPosition().contains( point ) )
		{
			control = ivControls[i];
			break;
		}

	return control;
}

MDCControlSelectionManager* MDCGrid::getSelectionManager()
{
	return ivPtSelectionManager;
}

void MDCGrid::addListener( IDCGridListener* pListener )
{
	ivListeners.addListener( pListener );
}

void MDCGrid::removeListener( IDCGridListener* pListener )
{
	ivListeners.removeListener( pListener );
}

void MDCGrid::fireContentChanged()
{
	unsigned int count = ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IDCGridListener*)ivListeners.getListener( i ))->onGridContentChanged();
}

bool MDCGrid::processEvent( IUiEvent* pEvent )
{
	/*switch( pEvent->getEventType() )
	{
	case IUiEvent::MEventType::EVENT_MOUSE:
		{
			MMouseEvent* pMouseEvent = (MMouseEvent*) pEvent;
			switch( pMouseEvent->getType() )
			{
			case MMouseEvent::MMouseEventType::MOUSE_MOVE:
				onMouseMove( pMouseEvent );
				break;
			case MMouseEvent::MMouseEventType::MOUSE_DOWN:
				onMouseDown( pMouseEvent );
				break;
			case MMouseEvent::MMouseEventType::MOUSE_UP:
				onMouseUp( pMouseEvent );
				break;
			case MMouseEvent::MMouseEventType::MOUSE_DOUBLECLICK:
				onMouseDoubleClick( pMouseEvent );
				break;
			}
		}
		break;
	}*/
	return MyInteractiveWnd::processEvent( pEvent );
}