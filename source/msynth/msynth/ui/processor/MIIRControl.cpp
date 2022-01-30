/**
 * (C)2000-2003 MRoc
 */

#include "MIIRControl.h"

#define ID_LFO_CONTROL 3080
#define ID_ENV_CONTROL 3081

MPoint MIIRControl::COMBO_POINTS[] = { 
	MPoint( 15, 70 ),
	MPoint( 15, 88 ),
	MPoint( 15, 106 ) };
	
MIIRControl::MIIRControl() :
	MAbstractDescriptorView(),
	ivPtLfoControl( 0 ),
	ivPtEnvControl( 0 ),
	ivLabelName( 0 )
{
	for( unsigned int i=0;i<8;i++ )
		ivPtFloatGuis[ i ] = 0;
	for( i=0;i<3;i++ )
		ivPtCombo[ i ] = 0;

	setSize( MSize( 418, 135 ) );
}

MIIRControl::~MIIRControl()
{
}

void MIIRControl::setDescriptor( IDescriptor* pDescriptor )
{
	ASSERT( ivPtDescriptor == 0 );
	ASSERT( pDescriptor != 0 );

	MAbstractDescriptorView::setDescriptor( pDescriptor );

	ivLabelName = new MLabel( ivPtDescriptor->getShortName(), MColorMap::FG_COLOR2, MColorMap::BK_COLOR2 );
	ivLabelName->setRect( MRect( 1, 1, getWidth() - 2, 10 ) );
	addChild( ivLabelName );

	unsigned int start = MIIR::CUTOFF;
	unsigned int stop = MIIR::RESONANCE + 1;
	for( unsigned int i=start;i<stop;i++ )
	{
		int index = i - start;
		MFloatControl* ptControl = (MFloatControl*) ((MIIR*)ivPtDescriptor)->getControl( i );
		ivPtFloatGuis[ index ] = new MFloatGui( ptControl, ivBkColor );
		addChild( ivPtFloatGuis[ index ] );
	}

	for( i=0;i<3;i++ )
	{
		ivPtCombo[i] = new MComboGui( (MDiscreteComboControl*)((MIIR*)ivPtDescriptor)->getControl( i ), ivBkColor );
		ivPtCombo[i]->setRect(
			MRect(
				COMBO_POINTS[ i ].getX(),
				COMBO_POINTS[ i ].getY(),
				100,
				20 ) );
		addChild( ivPtCombo[ i ] );
	}

	int xx = 20;
	for( i=0;i<2;i++ )
	{
		ivPtFloatGuis[ i ]->setPoint( MPoint( xx, 18 ) );
		xx += ivPtFloatGuis[ i ]->getWidth() + 5;
	}

	ivPtEnvControl = new MControlSubView(
		((MIIR*)ivPtDescriptor)->getEnvelope(),
		getBkColor() );
	ivPtEnvControl->setPoint( MPoint( 130, 15 ) );
	addChild( ivPtEnvControl );

	ivPtLfoControl = new MControlSubView( ((MIIR*)ivPtDescriptor)->getLfo(), getBkColor() );
	ivPtLfoControl->setPoint( MPoint( 130, 75 ) );
	addChild( ivPtLfoControl );
}

void MIIRControl::paint( IGraphics* ptGraphics, const MRect &rect )
{
	ptGraphics->fillRect( rect, ivBkColor );
	ptGraphics->drawRect( rect, MColorMap::FG_COLOR3 );
}

MAbstractDescriptorView* MIIRControl::createInstance()
{
	return new MIIRControl();
}