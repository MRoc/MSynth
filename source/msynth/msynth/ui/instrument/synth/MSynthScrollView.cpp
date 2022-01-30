/**
 * (C) 2000-2003 MRoc
 */
#include "MSynthScrollView.h"
#include "MSynthEditor.h"
#include <gui/dialog/MMessageBox.h>
#include <gui/scrollbar/MScrollPane.h>

INIT_RTTI( MSynthScrollView::MSynthListener, "MSynthScrollView::MSynthListener" );

MSynthScrollView::MSynthScrollView() :
	MWndCollection(),
	ivPtListener( 0 ),
	ivPtSynth( 0 ),
	ivSpaceX( 2 ),
	ivSpaceY( 2 ),
	ivWidth( 0 ),
	ivHeight( 0 )
{
	ivInsets = MInsets( 1,1,1,1 );
	ivBkColor = MColorMap::BK_COLOR5;
}

MSynthScrollView::~MSynthScrollView()
{
	SAFE_DELETE( ivPtListener );
	deleteDescriptorViews();
}

void MSynthScrollView::setSynth( Synth* pSynth )
{
	if( ivPtSynth )
	{
		delete ivPtListener;
		ivPtListener = 0;
		deleteDescriptorViews();
	}

	ivPtSynth = pSynth;

	if( ivPtSynth )
	{
		updateGui();
		doLayout();
		ivPtListener = new MSynthListener( this );
	}
}

Synth* MSynthScrollView::getSynth()
{
	return ivPtSynth;
}

void MSynthScrollView::updateFromModel()
{
	unsigned int count = ivArPtDescriptorViews.size();
	for( unsigned int i=0;i<count;i++ )
		ivArPtDescriptorViews[i]->updateFromModel();
}

void MSynthScrollView::updateFromModel( unsigned int index )
{
	ASSERT( index >= 0 && index < ivArPtDescriptorViews.size() );
	ivArPtDescriptorViews[ index ]->updateFromModel();
}

void MSynthScrollView::doLayout()
{
	TRACE( "MSynthScrollView::doLayout()\n");
	MSize size = getSize();

	int x = ivInsets.left() ;//- scrollPos.getX();
	int y = ivInsets.top() ;//- scrollPos.getY();
	int oldViewsHeight = 0;
	int maxWidth = 0;

	unsigned int count = ivArPtDescriptorViews.size();
	for( unsigned int i=0;i<count;i++ )
	{
		int controlWidth =	ivInsets.left() + ivArPtDescriptorViews[ i ]->getWidth() + ivInsets.right();
		if( controlWidth > maxWidth )
			maxWidth = controlWidth;
	}

	for( i=0;i<count;i++ )
	{
		int currentViewsWidth = ivArPtDescriptorViews[ i ]->getWidth();
		int currentViewsHeight = ivArPtDescriptorViews[ i ]->getHeight();
		if( x + ivSpaceX + currentViewsWidth + ivInsets.right() > maxWidth &&
			x != ivInsets.left() )
		{ // line wrap
			x = ivInsets.left();
			y += ivSpaceY + oldViewsHeight;
			oldViewsHeight = currentViewsHeight;
		}

		ivArPtDescriptorViews[i]->setRect(
			MRect(
				x,
				y,
				currentViewsWidth,
				currentViewsHeight ) );
	
		x += ivSpaceX + currentViewsWidth;

		if( oldViewsHeight < currentViewsHeight )
			oldViewsHeight = currentViewsHeight;
	}

	ivWidth = maxWidth;
	ivHeight = y + oldViewsHeight + ivSpaceY;

	repaint();
}

MSize MSynthScrollView::getPreferredSize()
{
	return MSize( ivWidth + 1, ivHeight );
}

MAbstractDescriptorView* MSynthScrollView::createDescriptorView( IDescriptor* ptDesc )
{
	ASSERT( ptDesc );

	MAbstractDescriptorView* ptBack =
		MDescriptorViewFactory::getInstance()->getView(
			ptDesc->getViewId() );

	if( ptBack )
	{
		ptBack->setBkColor( MColorMap::BK_COLOR3 );
		ptBack->setDescriptor( ptDesc );
		ptBack->updateFromModel();
		addChild( ptBack );
	}
	else
		MMessageBox::showError( "UNKNOWN_VIEW_FOUND", MUtil::toString( ptDesc->getViewId() ) );

	return ptBack;
}

void MSynthScrollView::updateGui()
{
	TRACE("<synthscrollview::updategui/>\n");

	setScrollPos( MPoint(0,0) );

	deleteDescriptorViews();

	ivArPtDescriptorViews.push_back( 
		createDescriptorView( ivPtSynth->getOsc() ) );
	
	unsigned int count = ivPtSynth->getTransformerCount(); 
	for( unsigned int i=0;i<count;i++ )
		ivArPtDescriptorViews.push_back( 
			createDescriptorView( ivPtSynth->getTransformer( i ) ) );

	doLayout();
}

MSize MSynthScrollView::calculateSize()
{
	return MSize( ivWidth, ivHeight );
}

void MSynthScrollView::deleteDescriptorViews()
{
	unsigned int count = ivArPtDescriptorViews.size();
	for( unsigned int i=0;i<count;i++ )
	{
		if( ivArPtDescriptorViews[i] )
		{
			MAbstractDescriptorView* view = ivArPtDescriptorViews[i];
			ivArPtDescriptorViews[i] = 0;
			removeChild( view );
			delete( view );
		}
	}
	ivArPtDescriptorViews.clear();
}

MSynthScrollView::MSynthListener::MSynthListener( MSynthScrollView* ptScrollView ) :
	ivPtScrollView( ptScrollView )
{
	ivPtScrollView->ivPtSynth->addInstrumentListener( this );
}

MSynthScrollView::MSynthListener::~MSynthListener()
{
	ivPtScrollView->ivPtSynth->removeInstrumentListener( this );
}

void MSynthScrollView::MSynthListener::stateChanged( MInstrumentEvent anEvent )
{
	if( anEvent.getMessageType() == Synth::MSG_OSC_CHANGED )
	{
		IDescriptor *ptDescriptor = ivPtScrollView->ivPtSynth->getOsc();
		MAbstractDescriptorView* ptTemp = ivPtScrollView->ivArPtDescriptorViews[0];
		ivPtScrollView->ivArPtDescriptorViews[0] = 0;
		if( ptTemp )
		{
			ivPtScrollView->removeChild( ptTemp );
			delete ptTemp;
		}
		ivPtScrollView->ivArPtDescriptorViews[0] = ivPtScrollView->createDescriptorView( ptDescriptor );
		layoutControls();
	}
	else if( anEvent.getMessageType() == Synth::MSG_TRANSFORMER_ADDED )
	{
		ivPtScrollView->ivArPtDescriptorViews.push_back(
			ivPtScrollView->createDescriptorView(
				ivPtScrollView->ivPtSynth->getTransformer(
					anEvent.getValue() ) ) );
		layoutControls();
	}
	else if( anEvent.getMessageType() == Synth::MSG_TRANSFORMER_REMOVED )
	{
		int descriptorIndex = anEvent.getValue() + 1;
		ASSERT( descriptorIndex >= 0 );
		ASSERT( descriptorIndex < (int)ivPtScrollView->ivArPtDescriptorViews.size() );
		MAbstractDescriptorView* ptTemp = ivPtScrollView->ivArPtDescriptorViews[descriptorIndex];
		ivPtScrollView->ivArPtDescriptorViews.erase( ivPtScrollView->ivArPtDescriptorViews.begin() + descriptorIndex );
		if( ptTemp )
		{
			ivPtScrollView->removeChild( ptTemp );
			delete ptTemp;
		}
		layoutControls();
	}
	else if( anEvent.getMessageType() == Synth::MSG_TRANSFORMER_SWAPPED )
	{
		int index1 = (int)anEvent.getValue() + 1;
		int index2 = (int)anEvent.getValue2() + 1;

		ASSERT( index1 >= 0 );
		ASSERT( index1 < (int) ivPtScrollView->ivArPtDescriptorViews.size() );
		ASSERT( index2 >= 0 );
		ASSERT( index2 < (int) ivPtScrollView->ivArPtDescriptorViews.size() );

		MAbstractDescriptorView* ptTemp = ivPtScrollView->ivArPtDescriptorViews[ index1 ];
		ivPtScrollView->ivArPtDescriptorViews[ index1 ] = ivPtScrollView->ivArPtDescriptorViews[ index2 ];
		ivPtScrollView->ivArPtDescriptorViews[ index2 ] = ptTemp;
		layoutControls();
	}
}

void MSynthScrollView::MSynthListener::layoutControls()
{
	ivPtScrollView->doLayout();
	((MSynthEditor*)ivPtScrollView->getTopParent())->doLayout( true );
}

String MSynthScrollView::MSynthListener::getInstrumentListenerName()
{
	return "MSynthScrollView::MSynthListener";
}