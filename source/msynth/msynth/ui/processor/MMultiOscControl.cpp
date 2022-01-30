/**
 * (C)2001-2003 MRoc
 */

#include "MMultiOscControl.h"

INIT_RTTI( MMultiOscControl, "MMultiOscControl" );
INIT_RTTI( MMultiOscControl::MOscWrapper, "MMultiOscControl::MOscWrapper" );

MMultiOscControl::MMultiOscControl() :
	MAbstractDescriptorView(),
	ivLabelName(0),
	ivPtControls(0)
{
	setSize( MSize( 511, 226 ) );
}

MMultiOscControl::MMultiOscControl( IDescriptor *descriptor, MColor bkColor ) :
	MAbstractDescriptorView( descriptor, bkColor),
	ivLabelName(0),
	ivPtControls(0)
{
	setSize( MSize( 511, 226 ) );
	setDescriptor(descriptor);
}

MMultiOscControl::~MMultiOscControl()
{
}

void MMultiOscControl::createView( int index )
{
	if( ivPtControls[ index ] )
	{
		removeChild( ivPtControls[ index ] );
		delete ivPtControls[ index ];
		ivPtControls[ index ] = 0;
	}

	ivPtControls[ index ] = new MOscWrapper( (MMultiOsc*)ivPtDescriptor, index );

	if( ivPtControls[ index ] )
	{
		ivPtControls[ index ]->setRect( MRect( 10, 10+index*100, 300, 90 ) );
		addChild( ivPtControls[ index ] );
	}
}

void MMultiOscControl::updateFromModel()
{
	MMultiOsc* osc = (MMultiOsc*) ivPtDescriptor;
	for( unsigned int i=0;i<ivOscCount;i++ )
		ivPtControls[ i ]->updateFromModel();
}

void MMultiOscControl::setDescriptor( IDescriptor *descriptor )
{
	for( unsigned int i=0;i<ivOscCount;i++ )
		SAFE_DELETE( ivPtControls[ i ] );
	SAFE_DELETE( ivPtControls );

	MAbstractDescriptorView::setDescriptor( descriptor );
	ivOscCount = ((MMultiOsc*)ivPtDescriptor)->getOscCount();

	ivPtControls = new MOscWrapper*[ ivOscCount ];
	for( unsigned int i=0;i<ivOscCount;i++ )
		ivPtControls[ i ] = 0;

	// name label
	if( ivLabelName == 0 )
	{
		ivLabelName = new MLabel( ivPtDescriptor->getShortName(), MColorMap::FG_COLOR2, MColorMap::BK_COLOR2 );
		ivLabelName->setRect( MRect( 1, 1, getWidth() - 2, 10 ) );
		addChild( ivLabelName );
	}

	int yOsc = 15;
	int yCounter = 40;
	for( i=0;i<ivOscCount;i++ )
	{
		ivPtControls[ i ] = new MOscWrapper( (MMultiOsc*) ivPtDescriptor, i );
		ivPtControls[ i ]->setRect( MRect( 10, 10+i*120, 300, 100 ) );
		addChild( ivPtControls[ i ] );
	}
}

void MMultiOscControl::paint( IGraphics* ptGraphics, MRect &rect )
{
	ptGraphics->fillRect( rect, MColorMap::BK_COLOR3 );
	ptGraphics->drawRect( rect, MColorMap::FG_COLOR3 );
}

MAbstractDescriptorView* MMultiOscControl::createInstance()
{
	return new MMultiOscControl();
}

MMultiOscControl::MOscWrapper::MOscWrapper(MMultiOsc* pModel, unsigned int index) :
	ivPtCombo( 0 ),
	ivPtView( 0 ),
	ivPtCounter( 0 ),
	ivPtModel( pModel ),
	ivIndex( index )
{
	ASSERT( pModel );

	MDefaultFactory* ptFactory = MDefaultFactory::getInstance();
	MSourceRegistry* ptRegistry = MSourceRegistry::getInstance();

	// filter type combo
	int yOsc = 15;
	int yCounter = 40;

	ivPtCombo = new MComboBox();
	ivPtCombo->setRect( MRect( 5, 15, 95, 18 ) );
	addChild( ivPtCombo );
		
	unsigned int sourceCount = ptRegistry->getRegistrationCount();
	for( unsigned int j=0;j<sourceCount;j++ )
	{
		MObjectDescription* next = ptRegistry->getRegistration( j );
		if( next->getDescriptedRtti() != &MMultiOsc::gvRtti )
		{
			ivPtCombo->getModel()->add( next );
			if( ivPtModel->getOscAt( ivIndex )->getRtti() == next->getDescriptedRtti() )
				ivPtCombo->getModel()->setSelection( next );
		}
	}

	ivPtCombo->getModel()->addSelectionListener( this );

	ivPtCounter =
		new MCountControl(
			1,
			1,
			8,
			ivBkColor,
			MColorMap::create( 250,250,250 ),
			"Octave",
			52 );

	ivPtCounter->setRect( MRect( 5, 40, 100, 20 ) );
	addChild( ivPtCounter );

	int v = ((MIntControl*)ivPtModel->getOscAt( ivIndex )->getControl(MOsc::OCTAVE) )->getValue();
	((MCountControl*)ivPtCounter)->setPos( v );
	ivPtCounter->getModel()->addListener( this );

	createView();
}

MMultiOscControl::MOscWrapper::~MOscWrapper()
{
	ivPtCombo->getModel()->removeSelectionListener( this );
}

MComboBox* MMultiOscControl::MOscWrapper::getCombo()
{
	return ivPtCombo;
}

MAbstractDescriptorView* MMultiOscControl::MOscWrapper::getView()
{
	return ivPtView;
}

MCountControl* MMultiOscControl::MOscWrapper::getCounter()
{
	return ivPtCounter;
}

void MMultiOscControl::MOscWrapper::createView()
{
	if( ivPtView )
	{
		removeChild( ivPtView );
		delete ivPtView;
		ivPtView = 0;
	}

	MOsc* pOsc = ivPtModel->getOscAt( ivIndex );
	ASSERT( pOsc );

	ivPtView = MDescriptorViewFactory::getInstance()->getView( pOsc->getViewId() );

	if( ivPtView )
	{
		ivPtView->setBkColor( ivBkColor );
		ivPtView->setDescriptor( pOsc );
		ivPtView->setRect( MRect( 105, 15, 100, 20 ) );
		addChild( ivPtView );
		ivPtView->updateFromModel();
	}
	else
		MLogger::logWarning( "MMultiOscControl::MOscWrapper::createView failed" );
}

void MMultiOscControl::MOscWrapper::updateFromModel()
{
	ivPtCombo->getModel()->setSelection( ivPtModel->getOscAt( ivIndex )->getName() );
	int octaveMulti = ((MIntControl*)ivPtModel->getOscAt( ivIndex )->getControl( MOsc::OCTAVE) )->getValue();
	ivPtCounter->setPos( octaveMulti );
}

void MMultiOscControl::MOscWrapper::valueChanged( MSpinButtonModel* pModel )
{
	MOsc* ptOsc = ivPtModel->getOscAt( ivIndex );
	((MIntControl*)ptOsc->getControl( MOsc::OCTAVE ) )->setValue( ivPtCounter->getPos() );
}

void MMultiOscControl::MOscWrapper::selectionChanged( MObject* newSelection, MObject* ptSource )
{
	MOsc* ptCurrentOsc = ivPtModel->getOscAt( ivIndex );
	int octave = ((MIntControl*)ptCurrentOsc->getControl( MOsc::OCTAVE ))->getValue();
	if( ivPtView )
	{
		removeChild( ivPtView );
		delete ivPtView;
		ivPtView = 0;
	}
	
	MObjectDescription* ptDesc = (MObjectDescription*) ivPtCombo->getModel()->getSelectedObject();
	if( ptCurrentOsc->getRtti() != ptDesc->getDescriptedRtti() )
	{
		MOsc *ptOsc = (MOsc*) ptDesc->getDescriptedRtti()->createInstance();
		ivPtModel->setOscAt( ptOsc, ivIndex );
		((MIntControl*)ptOsc->getControl( MOsc::OCTAVE ))->setValue( octave );
	}
	
	createView();
}

String MMultiOscControl::MOscWrapper::getClassName()
{
	return "MOscWrapper";
}