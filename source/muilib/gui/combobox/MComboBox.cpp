#include "MComboBox.h"
#include "../MWndUtils.h"
#include "../MGuiUtils.h"

MRtti MComboBox::gvRtti = MRtti( "MComboBox", MComboBox::createInstance );

INIT_RTTI( MComboBox::MSelectableObjectListListener, "MComboBox::MSelectableObjectListListener" );

MComboBox::MComboBox() :
	MyInteractiveWnd(),
	ivPtPopup( 0 ),
	ivPtListener( 0 )
{
	ivBkColor = MColorMap::BK_COLOR1;
	ivPtPopup = new MComboBoxPopup();
	ivPtListener = new MSelectableObjectListListener( this );
	getModel()->addSelectionListener( ivPtListener );
}

MComboBox::MComboBox( MSelectableObjectList* ptModel ) :
	MyInteractiveWnd(),
	ivPtPopup( 0 ),
	ivPtListener( 0 )
{
	ivBkColor = MColorMap::BK_COLOR1;
	ivPtPopup = new MComboBoxPopup();
	ivPtListener = new MSelectableObjectListListener( this );
	getModel()->addSelectionListener( ivPtListener );
}

MComboBox::~MComboBox()
{
	if( getModel() )
		getModel()->removeSelectionListener( ivPtListener );
	SAFE_DELETE( ivPtPopup );
	SAFE_DELETE( ivPtListener );
}

void MComboBox::paint( IGraphics* ptGraphics, const MRect &rect )
{
	MRect textRect( 1, 1, rect.getWidth() - 18, rect.getHeight() - 3 );
	MRect buttonRect( rect.getWidth() - 16, 1, 14, rect.getHeight() - 3 );

	// select color by state
	MColor bColor = MColorMap::BK_COLOR3;
	if( this->getMouseIn() && ! (getMouseDownLeft()||getMouseDownRight())  )
		bColor = MGuiUtils::multiplyColor( bColor, 1.1f );

	// paint background
	ptGraphics->fillRect( rect, MColorMap::BK_COLOR3 );

	// paint text+button border
	if( getMouseDownLeft()||getMouseDownRight() )
	{
		MGuiUtils::paintSunkenBorder( ptGraphics, textRect, bColor );
		MGuiUtils::paintSunkenBorder( ptGraphics, buttonRect, bColor );
	}
	else
	{
		MGuiUtils::paintRaisedBorder( ptGraphics, textRect, bColor );
		MGuiUtils::paintRaisedBorder( ptGraphics, buttonRect, bColor );
	}

	// paint button arrow
	MArrowRenderer::paintArrow(
		ptGraphics,
		buttonRect,
		MPoint( 0, 0 ),
		MArrowRenderer::DOWN );

	// paint text background
	MRect textClipRect( rect.getX() + 3, rect.getY() + 3, rect.getWidth() - 21, rect.getHeight() - 6 );
	ptGraphics->fillRect(
		textClipRect,
		MColorMap::FG_COLOR2 );

	// paint text
	if( getModel()->getSelectedObject() )
	{
		ptGraphics->drawText(
			getModel()->getSelectedObject()->toString(),
			textClipRect,
			MColorMap::BLACK );
	}

	/*if( this->getShowingTooltip() )
		this->paintHighLight( ptGraphics, rect, RGB( 255, 255, 0 ) );*/
}

void MComboBox::onMouseDown( MMouseEvent* ptEvent )
{
	MyInteractiveWnd::onMouseDown( ptEvent );

	if( ivPtPopup )
	{
		if( ! ivPtPopup->getImpl()->isCreated() )
			ivPtPopup->create( MRect( 0, 0, 100, 50 ), (MTopWnd*) getTopParent() );

		MRect rect = MWndUtils::getScreenOffsetRect( this );
		MSize prefSize = ivPtPopup->getPeferredSize();
		ivPtPopup->setRect(
			MRect(
				rect.getX(),
				rect.getBottom(),
				rect.getWidth(),
				prefSize.getHeight() ) );
		ivPtPopup->setVisible( true );
		repaint();
	}
}

MSelectableObjectList* MComboBox::getModel()
{
	return ivPtPopup->getModel();
}

void MComboBox::setSelection( int index )
{
	getModel()->setSelection( index );
}

int MComboBox::getSelection()
{
	return getModel()->getSelection();
}

MObject* MComboBox::getSelectedObject()
{
	return getModel()->getSelectedObject();
}

void MComboBox::setModel( MSelectableObjectList* ptModel )
{
	if( ivPtPopup->getModel() )
		ivPtPopup->getModel()->removeSelectionListener( ivPtListener );
	ivPtPopup->setModel( ptModel );
	if( ivPtPopup->getModel() )
		ivPtPopup->getModel()->addSelectionListener( ivPtListener );
}

MComboBox::MSelectableObjectListListener::MSelectableObjectListListener( MComboBox* ptCombo ) :
	ivPtCombo( ptCombo )
{
}

MComboBox::MSelectableObjectListListener::~MSelectableObjectListListener()
{
}

void MComboBox::MSelectableObjectListListener::selectionChanged( MObject* newSelection, MObject* ptSource )
{
	ivPtCombo->repaint();
}

String MComboBox::MSelectableObjectListListener::getClassName()
{
	return "MComboBox::MSelectableObjectListListener";
}

IRtti* MComboBox::getRtti() const
{
	return &gvRtti;
}

MObject* MComboBox::createInstance()
{
	return new MComboBox();
}