#include "MComboBoxPopup.h"
#include "../MGuiUtils.h"

INIT_RTTI( MComboBoxPopup::MComboPopupController, "MComboBoxPopup::MComboPopupController" );

/** constructor */
MComboBoxPopup::MComboBoxPopup() :
	MPopupWnd(),
	ivPtView( 0 ),
	ivPtScrollBar( 0 ),
	ivPtListener( 0 )
{
	ivPtView = new MListView();
	ivPtScrollBar = new MScrollBar( MScrollBar::VERTICAL );
	ivPtListener = new MComboPopupController( this );

	addChild( ivPtView );
	addChild( ivPtScrollBar );
}

/** constructor */
MComboBoxPopup::MComboBoxPopup( MSelectableObjectList* ptList ) :
	MPopupWnd(),
	ivPtView( 0 ),
	ivPtScrollBar( 0 )
{
	ivPtView = new MListView( ptList );
	ivPtScrollBar = new MScrollBar( MScrollBar::VERTICAL );
	ivPtListener = new MComboPopupController( this );

	addChild( ivPtView );
	addChild( ivPtScrollBar );
}

/** destructor */
MComboBoxPopup::~MComboBoxPopup()
{
	SAFE_DELETE( ivPtListener );

	removeChild( ivPtView );
	removeChild( ivPtScrollBar );
	SAFE_DELETE( ivPtView );
	SAFE_DELETE( ivPtScrollBar );
}

/** creates the popup */
bool MComboBoxPopup::create( MRect rect, MTopWnd* ptParent )
{
	bool back = MPopupWnd::create( rect, ptParent );
	if( back )
		doLayout();
	return back;
}

/** returns the preferred size of this window */
MSize MComboBoxPopup::getPeferredSize()
{
	MSize listViewSize = ivPtView->getPreferredSize();
	return
		MSize( 
			listViewSize.getWidth() + 4,
			listViewSize.getHeight() + 4 );
}

/** paints a border arround the listview */
void MComboBoxPopup::paint( IGraphics* pGraphics, const MRect &rect )
{
	pGraphics->fillRect( rect, MColorMap::BK_COLOR3 );
	MGuiUtils::paintRaisedBorder(
		pGraphics,
		rect,
		MColorMap::BK_COLOR3 );
}

/** layouts the window */
void MComboBoxPopup::doLayout()
{
	MSize size = getSize();
	int xOff = 16;
	MSize prefSize = ivPtView->getPreferredSize();
	if( size.getHeight() - 4 >= prefSize.getHeight() )
	{
		xOff = 0;
		if( ivPtScrollBar )
			ivPtScrollBar->setVisible( false );	
	}
	else
		if( ivPtScrollBar )
			ivPtScrollBar->setVisible( true );


	if( ivPtView )
	{
		ivPtView->setRect(
			MRect(
				2,
				2,
				size.getWidth() - 4 - xOff,
				size.getHeight() - 4 ) );
		ivPtView->repaint();
	}

	if( ivPtScrollBar )
	{
    	ivPtScrollBar->setRect(
			MRect(
				size.getWidth() - 18,
				2,
				16,
				size.getHeight()-4 ) );

		ivPtScrollBar->getModel()->setData(
				0,
				size.getHeight() - 4,
				ivPtView->getPreferredSize().getHeight() );
	}
}

/** returns the model of this popup */
MSelectableObjectList* MComboBoxPopup::getModel()
{
	return ivPtView->getModel();
}

/** sets the model of this popup */
void MComboBoxPopup::setModel( MSelectableObjectList* ivPtModel )
{
	ivPtView->setModel( ivPtModel );
}

MComboBoxPopup::MComboPopupController::MComboPopupController( MComboBoxPopup* ptPopup ) :
	ivPtPopup( ptPopup )
{
	ivPtPopup->ivPtView->addListViewListener( this );
	ivPtPopup->ivPtScrollBar->getModel()->addActionListener( this );
}

MComboBoxPopup::MComboPopupController::~MComboPopupController()
{
	ivPtPopup->ivPtView->removeListViewListener( this );
	ivPtPopup->ivPtScrollBar->getModel()->removeActionListener( this );
}

void MComboBoxPopup::MComboPopupController::onSelection()
{
	ivPtPopup->setVisible( false );
}

void MComboBoxPopup::MComboPopupController::onPreSelection()
{
	int top = ivPtPopup->ivPtView->getPreSelection() * MListView::CY_SIZE;
	int bottom = top + MListView::CY_SIZE;
	int scrollPos = ivPtPopup->ivPtView->getScrollPos().getY();
	if( top < scrollPos )
	{
		ivPtPopup->ivPtScrollBar->getModel()->setValue( top );
	}
	else if( bottom > scrollPos + ivPtPopup->ivPtScrollBar->getModel()->getInner() )
	{
		ivPtPopup->ivPtScrollBar->getModel()->setValue(
			bottom - ivPtPopup->ivPtScrollBar->getModel()->getInner() );
	}
}

void MComboBoxPopup::MComboPopupController::valueChanged( void* source )
{
	if( ivPtPopup && ivPtPopup->ivImpl->isCreated() )
	{
		int scrollOffset = ivPtPopup->ivPtScrollBar->getModel()->getValue();
		ivPtPopup->ivPtView->setScrollPos( MPoint( 0, scrollOffset ) );
	}
}