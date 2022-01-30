/**
 * (C)2000-2003 MRoc hifiShock
 */

#include "MDescriptorView.h"
#include "../control/MComboGui.h"

#define NAME_LABEL_HEIGHT 10
#define DEFAULT_HORIZONTAL_SPACE 5
#define DEFAULT_VERTICAL_SPACE 5

MDescriptorView::MDescriptorView() :
	MAbstractDescriptorView(),
	ivLabelName( 0 )
{
}

MDescriptorView::MDescriptorView( IDescriptor *descriptor, MColor bkColor ) :
	MAbstractDescriptorView( descriptor, bkColor ),
	ivLabelName( 0 )
{
	setDescriptor( descriptor );
	setBkColor( bkColor );
}

MDescriptorView::~MDescriptorView()
{ 
	while( ivArPtComponents.size() > 0 )
	{
		MWnd* ptWnd = ivArPtComponents[ 0 ];
		removeChild( ptWnd );
		ivArPtComponents.erase( ivArPtComponents.begin() );
		delete ptWnd;
	}
	ivArPtComponents.clear();
	ivArIndex.clear();
}

void MDescriptorView::paint( IGraphics* ptGraphics, const MRect &rect )
{
	ptGraphics->fillRect( rect, ivBkColor );
	ptGraphics->drawRect( rect, MColorMap::FG_COLOR3 );
}

void MDescriptorView::setDescriptor( IDescriptor* descriptor )
{
	ASSERT( ivPtDescriptor == 0 );
	ASSERT( descriptor );

	MAbstractDescriptorView::setDescriptor( descriptor );

	unsigned int realAdded = 0;
	unsigned int count = ivPtDescriptor->getControlCount();
	for( unsigned int i=0;i<count;i++ )
	{
		if( ivPtDescriptor->getControl( i ) )
		{
 			if( ivPtDescriptor->getControl( i )->getViewID() == DESC_TYPE_SLIDER )
			{
				MFloatGui* ptFloatGui = new MFloatGui( ((MFloatControl*)ivPtDescriptor->getControl( i )), getBkColor() );
				ivArPtComponents.push_back( ptFloatGui );
				ivArIndex.push_back( realAdded );

				realAdded++;
			}
			else if( ivPtDescriptor->getControl( i )->getViewID() == DESC_TYPE_COMBO )
			{
				MComboGui* ptFloatGui = new MComboGui( ((MDiscreteComboControl*)ivPtDescriptor->getControl( i )), getBkColor() );
				ivArPtComponents.push_back( ptFloatGui );
				ivArIndex.push_back( realAdded );

				realAdded++;
			}
			else
				ivArIndex.push_back( -1 );
		}
		else
			ivArIndex.push_back( -1 );
	}

	count = ivArPtComponents.size();
	for( unsigned int i=0;i<count;i++ )
		addChild( ivArPtComponents[ i ] );

	ivLabelName = new MLabel(
		ivPtDescriptor->getShortName(),
		MColorMap::FG_COLOR2,
		MColorMap::BK_COLOR2 );
	ivLabelName->setPoint( MPoint( 1, 1 ) );
	addChild( ivLabelName );

	doLayout();

	setSize( MSize( 20 + 40 * ivArPtComponents.size(), 65 ) );
}

void MDescriptorView::updateFromModel()
{
	ASSERT( ivLabelName );
	ASSERT( ivPtDescriptor );
	ivLabelName->setText( ivPtDescriptor->getShortName() );
}

void MDescriptorView::doLayout()
{
	int xx = 5;
	unsigned int count = ivArPtComponents.size();
	for( unsigned int i=0;i<count;i++ )
	{
		if( ivArPtComponents[ i ] )
		{
			ivArPtComponents[ i ]->setPoint( MPoint( xx, 15 ) );
			xx += ivArPtComponents[ i ]->getWidth() + 5;
		}
	}

	if( ivLabelName )
		ivLabelName->setRect( MRect( 1, 1, getWidth() - 2, NAME_LABEL_HEIGHT ) );
}

MAbstractDescriptorView* MDescriptorView::createInstance()
{
	return new MDescriptorView();
}