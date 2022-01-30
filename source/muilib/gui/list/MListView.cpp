#include "mlistview.h"
#include <framework/io/MLogger.h>
#include "../event/MMouseEvent.h"
#include "../event/MKeyEvent.h"
#include "../MWndManager.h"
#include "../MTopWnd.h"

const int MListView::CY_SIZE = 14;

INIT_RTTI_CREATE( MListView, "MListView" );
INIT_RTTI( MListView::MListModelListener, "MListView::MListModelListener" );

MListView::MListView() :
	MWndCollection(),
	ivPtListener( 0 ),
	ivPtModel( 0 ),
	ivPreSelection( -1 ),
	ivPreSelectionEnabled( true )
{
	init();
}

MListView::MListView( MSelectableObjectList* ptList ) :
	MWndCollection(),
	ivPtListener( 0 ),
	ivPtModel( 0 ),
	ivPreSelection( -1 ),
	ivPreSelectionEnabled( true )
{
	ivPtListener = new MListModelListener( this );
	setModel( ptList );
}

void MListView::init()
{
	ivBkColor = MColorMap::FG_COLOR2;
	ivPtListener = new MListModelListener( this );
	setModel( new MSelectableObjectList() );
	ivPreSelection = getModel()->getSelection();
	//this->setRedrawNecessary( true );
}

MListView::~MListView()
{
	ivListeners.clear();

	if( ivPtModel )
	{
		ivPtModel->deleteAll();
		MSelectableObjectList* ptModelTmp = getModel();
		setModel( 0 );
		if( ptModelTmp )
			delete ptModelTmp;
	}

	SAFE_DELETE( ivPtListener );
}

void MListView::paint( IGraphics* ptGraphics, const MRect &rect )
{
	ptGraphics->fillRect( rect, ivBkColor );

	MPoint target = getScrollPos();
	int y = target.getY();
	unsigned int width = rect.getWidth();

	unsigned int count = ivPtModel->getLength();
	for( unsigned int i=0;i<count;i++ )
	{
		MRect r( 0, i*CY_SIZE - y, width, CY_SIZE );
		if( ivPreSelection == (int)i )
		{
			ptGraphics->fillRect( r, MColorMap::FG_COLOR3 );
			ptGraphics->drawText(
				ivPtModel->get( i )->toString(),
				r,
				MColorMap::FG_COLOR2 );
		}
		else
			ptGraphics->drawText(
				ivPtModel->get( i )->toString(),
				r,
				MColorMap::FG_COLOR3 );
	}
}

bool MListView::processEvent( IUiEvent* pEvent )
{
	switch( pEvent->getEventType() )
	{
	case IUiEvent::EVENT_MOUSE:
		{
			MMouseEvent* pMouseEvent = (MMouseEvent*) pEvent;
			switch( pMouseEvent->getType() )
			{
			case MMouseEvent::MMouseEventType::MOUSE_DOWN:
				if( pMouseEvent->getButton() == MMouseEvent::BUTTON1 )
				{
					MPoint point = pMouseEvent->getPoint();
					int index = point.getY() / CY_SIZE;
					if( index < (int) ivPtModel->getLength() )
						ivPtModel->setSelection( index );
					MWndManager::getInstance()->setKeyFocus( this );
				}
				break;
			case MMouseEvent::MMouseEventType::MOUSE_UP:
				if( pMouseEvent->getButton() == MMouseEvent::BUTTON1 )
				{
					MPoint point = pMouseEvent->getPoint();
					int index = point.getY() / CY_SIZE;
					if( index < (int) ivPtModel->getLength() )
						ivPtModel->setSelection( index );
					fireActionPerformed();
				}
				break;
			case MMouseEvent::MMouseEventType::MOUSE_MOVE:
				{
					if( ivPreSelectionEnabled )
					{
						MPoint point = pMouseEvent->getPoint();
						int index = point.getY() / CY_SIZE;
						if( index < (int) ivPtModel->getLength() && ivPreSelection != index )
							this->setPreSelection( index );
						MWndManager::getInstance()->setKeyFocus( this );
					}
				}
				break;
			}
		}
		break;
	case IUiEvent::EVENT_KEY:
		{
			MKeyEvent* pKeyEvent = (MKeyEvent*) pEvent;
			if( pKeyEvent->getType() == MKeyEvent::KEY_DOWN ||
				pKeyEvent->getType() == MKeyEvent::KEY_INTERMEDIATE )
			{
				if( pKeyEvent->getScanCode() == MKeyEvent::MVirtualKey::MVK_UP )
				{
					unsigned int newIndex = ivPreSelection - 1;
					if( newIndex < ivPtModel->getLength() )
						setPreSelection( newIndex );
				}
				else if( pKeyEvent->getScanCode() == MKeyEvent::MVirtualKey::MVK_DOWN )
				{
					unsigned int newIndex = ivPreSelection + 1;
					if( newIndex < ivPtModel->getLength() )
						setPreSelection( newIndex );
				}
				else if( pKeyEvent->getScanCode() == MKeyEvent::MVirtualKey::MVK_HOME )
				{
					if( ivPtModel->getLength() > 0 )
						setPreSelection( 0 );
				}
				else if( pKeyEvent->getScanCode() == MKeyEvent::MVirtualKey::MVK_END )
				{
					if( ivPtModel->getLength() > 0 )
						setPreSelection( ivPtModel->getLength() - 1 );
				}
			}
		}
		break;
	}
	return MWndCollection::processEvent( pEvent );
}

void MListView::onModelChanged()
{
	ivPreSelection = ivPtModel->getSelection();
	repaint();
}

void MListView::onselectionChanged()
{
	ivPreSelection = ivPtModel->getSelection();
	firePreselect();
	repaint();
}

MSelectableObjectList* MListView::getModel()
{
	return this->ivPtModel;
}

void MListView::setModel( MSelectableObjectList* ptList )
{
	if( ivPtModel )
	{
		ivPtModel->removeObjectListListener( ivPtListener );
		ivPtModel->removeSelectionListener( ivPtListener );
	}

	this->ivPtModel = ptList;

	if( ivPtModel )
	{
		ivPtModel->addObjectListListener( ivPtListener );
		ivPtModel->addSelectionListener( ivPtListener );
		this->ivPreSelection = ivPtModel->getSelection();
	}
}

MSize MListView::getPreferredSize()
{
	MSize back;
	back.setWidth( 30 );
	back.setHeight( ivPtModel->getLength() * CY_SIZE );
	/*if( getVisible() )
	{
		HDC dc = GetDC( ((MOsWindow*)getTopParent())->getHwnd() );
		if( dc )
		{*/
/*
	MSize back;

	HDC hdc = GetDC( wnd );
	HGDIOBJ oldObj = SelectObject( hdc, ptFont->m_hObject );
	SIZE size;

	if( 
		GetTextExtentPoint32(
			hdc,					// handle to DC
			text,					// text string
			text.GetLength(),		// characters in string
			&size					// string size
			) == TRUE )
	{
		back = MSize( size.cx, size.cy );
	}
	else
		MLogger::logError(
			"MGuiUtils::getTextExtent failed" );

	SelectObject( hdc, oldObj );
	ReleaseDC( wnd, hdc );
*/
			/*int max = 0;
			unsigned int count = ivPtModel->getLength();
			for( unsigned int i=0;i<count;i++ )
			{
				String text = ivPtModel->get( i )->toString();
				SIZE size;
				if( GetTextExtentPoint32(
					dc,					// handle to DC
					text,					// text string
					text.GetLength(),		// characters in string
					&size					// string size
					) == FALSE )
				{
					MLogger::logError(
						"MListView::getPreferredSize: GetTextExtentPoint32 failed" );
				}
				if( size.cx > max )
					max = size.cx;
			}
			ReleaseDC( ((MOsWindow*)getTopParent())->getHwnd(), dc );
			back.setWidth( max );
		}
		else
			MLogger::logError( 
				"MListView::getPreferredSize: GetDC failed" );
	}*/
	return back;
}

void MListView::setPreSelection( int preSelection )
{
	ivPreSelection = preSelection;
	repaint();
	firePreselect();
}

int MListView::getPreSelection()
{
	return ivPreSelection;
}

void MListView::fireActionPerformed()
{
	unsigned int count = this->ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IListViewListener*)ivListeners.getListener( i ))->onSelection();
}

void MListView::firePreselect()
{
	unsigned int count = this->ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IListViewListener*)ivListeners.getListener( i ))->onPreSelection();
}

void MListView::addListViewListener( IListViewListener* ptListener )
{
	ivListeners.addListener( ptListener );
}

void MListView::removeListViewListener( IListViewListener* ptListener )
{
	ivListeners.removeListener( ptListener );
}

void MListView::setPreSelectionEnabled( bool enable )
{
	ivPreSelectionEnabled = enable;
}

bool MListView::getPreSelectionEnabled()
{
	return ivPreSelectionEnabled;
}

MListView::MListModelListener::MListModelListener( MListView* ptView )
{
	ivPtView = ptView;
}

MListView::MListModelListener::~MListModelListener()
{
}

void MListView::MListModelListener::objectAdded( MObject* obj )
{
	ivPtView->onModelChanged();
}

void MListView::MListModelListener::selectionChanged( MObject* newSelection, MObject* ptSource )
{
	ivPtView->onselectionChanged();
}

void MListView::MListModelListener::objectRemoved( MObject* obj )
{
	ivPtView->onModelChanged();
}

void MListView::MListModelListener::allObjectsRemoved()
{
	ivPtView->onModelChanged();
}