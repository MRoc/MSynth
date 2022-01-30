#include "MFxSlotWnd.h"
#include "MFxRackFrame.h"
#include <gui/dialog/MMessageBox.h>

INIT_RTTI( MFxSlotWnd::MFxSlotListener, "MFxSlotWnd::MFxSlotListener" );

/**
 * constructor
 */
MFxSlotWnd::MFxSlotWnd( unsigned int index, MFxSlot* ptSlot ) :
	MWndCollection(),
	ivPtModel( ptSlot ),
	ivPtListener( 0 ),
	ivPtLabel( 0 ),
	ivPtCombo( 0 ),
	ivPtView( 0 )
{
	ivInsets = MInsets( 3,3,3,3 );
	ivBkColor = MColorMap::create( 255, 0, 0 );
	ivPtLabel =
		new MLabel(
			"Effect " + MInt::toString( index ),
			MColorMap::FG_COLOR2,
			MColorMap::BK_COLOR5 );
	ivPtLabel->setCenterText( false );
	addChild( ivPtLabel );

	ivPtCombo =
		new MComboBox(
			ptSlot->getTransformerRttis() );
	addChild( ivPtCombo );

	ivPtListener = new MFxSlotListener( this );

	setSize( MSize( 250, 18 + ivInsets.top() + ivInsets.right() ) );

	updateView( false );
}

/**
 * destructor
 */
MFxSlotWnd::~MFxSlotWnd()
{
	SAFE_DELETE( ivPtListener );
	ivPtCombo->setModel( 0 );

	SAFE_DELETE( ivPtView );
	SAFE_DELETE( ivPtCombo );
	SAFE_DELETE( ivPtLabel );
}

/**
 * paints the background
 */
void MFxSlotWnd::paint( IGraphics* ptGraphics, const MRect &rect )
{
	ptGraphics->drawRect( rect, MColorMap::BK_COLOR5 );
	ptGraphics->fillRect( 1, 1, rect.getWidth() - 1, ivInsets.top() + 18 + 2, MColorMap::BK_COLOR5 );
	ptGraphics->fillRect( 1, ivInsets.top() + 18 + 2, rect.getWidth() - 1, rect.getHeight() - 1, MColorMap::BK_COLOR2 );
}

/**
 * deletes (if nescessary) and
 * creates (if nescessary)
 * a new MDescriptorView
 */
void MFxSlotWnd::updateView( bool autosize )
{
	int rowheight = 18;
	int spacing = 2;
	int width = 250;
	int height = ivInsets.top() + rowheight + ivInsets.bottom();

	// delete old one ?
	if( ivPtView )
	{
		removeChild( ivPtView );
		delete ivPtView;
		ivPtView = 0;
	}

	// create new one ?
	IDescriptor* ptFx = (IDescriptor*) ivPtModel->getTransformer();
	if( ptFx )
	{
		ivPtView = (MDescriptorView*)
			MDescriptorViewFactory::getInstance()->getView( ptFx->getViewId() );
		if( ivPtView )
		{
			ivPtView->setBkColor( MColorMap::BK_COLOR3 );
			ivPtView->setDescriptor( ptFx );
			ivPtView->setRect( MRect( ivInsets.left(), ivInsets.top() + rowheight + spacing, ivPtView->getWidth(), ivPtView->getHeight() ) );
			addChild( ivPtView  );
			ivPtView->updateFromModel();

			// update size
			height += ivPtView->getHeight() + 1 + ivInsets.bottom();
			if( ivPtView->getWidth() > 250 - ( ivInsets.left() + ivInsets.right() ))
				width = ivPtView->getWidth();

			TRACE( "<mfxslotwnd::updateview/>\n" );
		}
		else
		{
			String msg("");
			msg.Format( "%i", ptFx->getViewId() );
			MMessageBox::showError( "UNKNOWN_VIEW_FOUND", msg );
		}
	}

	MSize newSize( width + ivInsets.left() + ivInsets.right(), height );
	setSize( newSize );
	doLayout();

	if( autosize )
	{
		((MWndCollection*)getParent())->doLayout();
		((MWndCollection*)getParent()->getParent())->doLayout();
		//((MWndCollection*)getParent()->getParent()->getParent())->autoSize();
	}
}

/**
 * returns the preferred size of this control
 */
MSize MFxSlotWnd::getPreferredSize()
{
	int width = 250;
	int rowHeight = 18;
	int spacing = 4;
	int height = ivInsets.top() + rowHeight + ivInsets.bottom();
	if( this->ivPtView )
	{
		MSize viewSize = ivPtView->getSize();
		height += viewSize.getHeight() + spacing;
		if( (int)viewSize.getWidth() > width - (ivInsets.left() + ivInsets.right() ) )
		{
			width = ivInsets.left() + viewSize.getWidth() +  + ivInsets.right();
		}
	}
	return MSize( width, height );
}

/**
 * layouts all components
 */
void MFxSlotWnd::doLayout()
{
	TRACE( "<mfxslotwnd::dolayout/>\n" );
	MWndCollection::doLayout();

	int labelWidth = 40;
	int comboWidth = 200;
	int rowHeight = 18;
	int spacing = 4;
	ivPtLabel->setRect( MRect( ivInsets.left(), ivInsets.top(), labelWidth, rowHeight ) );
	ivPtCombo->setRect( MRect( ivInsets.left() + labelWidth + spacing, ivInsets.top(), comboWidth, rowHeight ) );

	if( ivPtView )
		ivPtView->setRect(
			MRect(
				ivInsets.left(),
				ivInsets.top() + rowHeight + spacing,
				ivPtView->getWidth(),
				ivPtView->getHeight() ) );
}

/**
 * constructor
 */
MFxSlotWnd::MFxSlotListener::MFxSlotListener( MFxSlotWnd* ptView ) :
	ivPtView( ptView )
{
	ivPtView->ivPtModel->addListener( this );
}

/**
 * destructor
 */
MFxSlotWnd::MFxSlotListener::~MFxSlotListener()
{
	ivPtView->ivPtModel->removeListener( this );
}

/**
 * invoked when a processor changed
 * ptOld - the previous slected transformer, can be null will be deleted after
 * ptNew - the new selected transformer, can be null
 */
void MFxSlotWnd::MFxSlotListener::onProcessorChanged( MTransformer* ptOld, MTransformer* ptNew )
{
	ivPtView->updateView( true );
}