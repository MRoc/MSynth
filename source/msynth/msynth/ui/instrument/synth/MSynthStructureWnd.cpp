#include "MSynthStructureWnd.h"
#include "../../../../msynth/model/source/MSourceRegistry.h"
#include "../../../../msynth/model/transformer/MTransformerRegistry.h"
#include <framework/resource/text/MTextResource.h>
#include <gui/win32impl/MTopWndWin32Impl.h>

#define ID_LABEL_SRC			1
#define ID_LABEL_TRANSFORMER	2
#define ID_BUTTON_UP			3
#define ID_BUTTON_DOWN			4
#define ID_BUTTON_REMOVE		5
#define ID_BUTTON_ADD			6
#define ID_BUTTON_OK			7
#define ID_COMBO_OSC			8
#define ID_COMBO_TRANSFORMER	9
#define ID_LISTVIEW_TRANSFORMER	10

INIT_RTTI( MSynthStructureWnd::MScrComboListener, "MSynthStructureWnd::MScrComboListener" );
INIT_RTTI( MSynthStructureWnd::MButtonListener, "MSynthStructureWnd::MButtonListener" );

/** constructor */
MSynthStructureWnd::MSynthStructureWnd() :
	MWndCollection(),
	ivPtSynth( 0 ),
	ivPtLabelSrc( 0 ),
	ivPtLabelTransformer( 0 ),
	ivPtButtonUp( 0 ),
	ivPtButtonDown( 0 ),
	ivPtButtonRemove( 0 ),
	ivPtButtonAdd( 0 ),
	ivPtButtonOk( 0 ),
	ivPtOscCombo( 0 ),
	ivPtTransformerCombo( 0 ),
	ivPtTransformers( 0 ),
	ivPtComboSrcListener( 0 ),
	ivPtButtonListener( 0 )
{
	ivLayout.load( "resource/layout/MSynthStructureWnd.xml" );
	ivBkColor = MColorMap::BK_COLOR1;
}

/** destructor */
MSynthStructureWnd::~MSynthStructureWnd()
{
	SAFE_DELETE( ivPtButtonListener );
	SAFE_DELETE( ivPtComboSrcListener );

	this->ivPtOscCombo->getModel()->removeAll();
	this->ivPtTransformerCombo->getModel()->removeAll();
	this->ivPtTransformers->setModel( 0 );

	while( ivChilds.size() > 0 )
	{
		MWnd* ptWnd = ivChilds[ 0 ];
		ivChilds.erase( ivChilds.begin() );
		delete ptWnd;
	}
}

/** sets the edited synth */
void MSynthStructureWnd::setSynth( Synth* ptSynth )
{
	ASSERT( ptSynth );

	ivPtSynth = ptSynth;

	ivPtLabelSrc = new MLabel( "Oscilator", MColorMap::FG_COLOR3, MColorMap::BK_COLOR1 );
	ivPtLabelSrc->setSize( MSize( 70, 22 ) );
	ivPtLabelSrc->setCenterText( false );

	ivPtLabelTransformer = new MLabel( "Transformer", MColorMap::FG_COLOR3, MColorMap::BK_COLOR1 );
	ivPtLabelTransformer->setSize( MSize( 70, 22 ) );
	ivPtLabelTransformer->setCenterText( false );

	ivPtButtonUp = new MButton( MArrowRenderer::UP, MColorMap::BK_COLOR1 );
	ivPtButtonUp->setSize( MSize( 70, 22 ) );
	ivPtButtonUp->setToolTipText( MTextResource::getInstance()->getString("TT_SYNTH_EDITOR_UP_BUTTON" ) );

	ivPtButtonDown = new MButton( MArrowRenderer::DOWN, MColorMap::BK_COLOR1 );
	ivPtButtonDown->setSize( MSize( 70, 22 ) );
	ivPtButtonDown->setToolTipText( MTextResource::getInstance()->getString("TT_SYNTH_EDITOR_DOWN_BUTTON" ) );

	ivPtButtonRemove = new MButton( String("Remove"), MColorMap::FG_COLOR3, MColorMap::FG_COLOR1, MColorMap::BK_COLOR1 );
	ivPtButtonRemove->setSize( MSize( 70, 22 ) );
	ivPtButtonRemove->setToolTipText( MTextResource::getInstance()->getString("TT_SYNTH_EDITOR_DELETE_BUTTON" ) );

	ivPtButtonAdd = new MButton( String("Add"), MColorMap::FG_COLOR3, MColorMap::FG_COLOR1, MColorMap::BK_COLOR1 );
	ivPtButtonAdd->setSize( MSize( 70, 22 ) );
	ivPtButtonAdd->setToolTipText( MTextResource::getInstance()->getString("TT_SYNTH_EDITOR_CREATE_BUTTON" ) );

	ivPtButtonOk = new MOkButton( "OK", MColorMap::FG_COLOR3, MColorMap::FG_COLOR1, MColorMap::BK_COLOR1 );
	ivPtButtonOk->setSize( MSize( 70, 22 ) );
	addChild( ivPtButtonOk );

	ivPtOscCombo = new MComboBox();
	ivPtOscCombo->setSize( MSize( 200, 22 ) );

	ivPtTransformerCombo = new MComboBox();
	ivPtTransformerCombo->setSize( MSize( 200, 22 ) );

	ivPtTransformers = new MListView( ivPtSynth->getTransformers() );
	ivPtTransformers->setPreSelectionEnabled( false );
	ivPtTransformers->setBkColor( MColorMap::BK_COLOR4 );
	ivPtTransformers->setSize( MSize( 200, 200 ) );

	MSourceRegistry *ptRegistry = MSourceRegistry::getInstance();
	unsigned int sourceCount = ptRegistry->getRegistrationCount();
	IRtti* currentSource = ((MOsc*)ivPtSynth->getOsc())->getRtti();
	for( unsigned int i=0;i<sourceCount;i++ )
	{
		ivPtOscCombo->getModel()->add( ptRegistry->getRegistration( i ) );
		if( currentSource == ptRegistry->getRegistration( i )->getDescriptedRtti() )
			ivPtOscCombo->getModel()->setSelection( i );
	}

	MTransformerRegistry *transformerRegistry = MTransformerRegistry::getInstance();
	unsigned int transformerCount = transformerRegistry->getTransformerCount();
	for( i=0;i<transformerCount;i++ )
	{
		ivPtTransformerCombo->getModel()->add(
			transformerRegistry->getTransformer( i ) );
	}
	ivPtTransformerCombo->setSelection( 0 );

	addChild( ivPtLabelSrc );
	addChild( ivPtLabelTransformer );
	addChild( ivPtButtonUp );
	addChild( ivPtButtonDown );
	addChild( ivPtButtonRemove );
	addChild( ivPtButtonAdd );
	addChild( ivPtOscCombo );
	addChild( ivPtTransformerCombo );
	addChild( ivPtTransformers );

	ivPtComboSrcListener = new MScrComboListener( this );
	ivPtButtonListener = new MButtonListener( this );

	ivPtNamespace = new MWndNamespace( this );
	ivLayout.getRoot()->setNamespace( getNamespace() );
	ivLayout.getRoot()->registerWnd( ivPtLabelSrc, "label.osc" );
	ivLayout.getRoot()->registerWnd( ivPtLabelTransformer, "label.transformer" );
	ivLayout.getRoot()->registerWnd( ivPtButtonUp, "button.up" );
	ivLayout.getRoot()->registerWnd( ivPtButtonDown, "button.down" );
	ivLayout.getRoot()->registerWnd( ivPtButtonAdd, "button.add" );
	ivLayout.getRoot()->registerWnd( ivPtButtonRemove, "button.remove" );
	ivLayout.getRoot()->registerWnd( ivPtButtonOk, "button.ok" );
	ivLayout.getRoot()->registerWnd( ivPtOscCombo, "combo.osc" );
	ivLayout.getRoot()->registerWnd( ivPtTransformerCombo, "combo.transformer" );
	ivLayout.getRoot()->registerWnd( ivPtTransformers, "listview.transformer" );
}

/** returns the edited synth */
Synth* MSynthStructureWnd::getSynth()
{
	return ivPtSynth;
}

/** layouts all controls */
void MSynthStructureWnd::doLayout()
{
	ivLayout.doLayout( this );
}

MSynthStructureWnd::MScrComboListener::MScrComboListener( MSynthStructureWnd* ptWnd ) :
	ivPtWnd( ptWnd )
{
	ivPtWnd->ivPtOscCombo->getModel()->addSelectionListener( this );
}

MSynthStructureWnd::MScrComboListener::~MScrComboListener()
{
	ivPtWnd->ivPtOscCombo->getModel()->removeSelectionListener( this );
}

/** invoked when selection changed newSelection = the new selected object ptSource = the object list */
void MSynthStructureWnd::MScrComboListener::selectionChanged( MObject* newSelection, MObject* ptSource )
{
	MObjectDescription* ptDesc = (MObjectDescription*) ivPtWnd->ivPtOscCombo->getModel()->getSelectedObject();
	ivPtWnd->ivPtSynth->setSoundSource( (MOsc*) ptDesc->getDescriptedRtti()->createInstance() );
}

MSynthStructureWnd::MButtonListener::MButtonListener( MSynthStructureWnd* ptWnd ) :
	ivPtWnd( ptWnd )
{
	ivPtWnd->ivPtButtonAdd->addActionListener( this );
	ivPtWnd->ivPtButtonRemove->addActionListener( this );
	ivPtWnd->ivPtButtonUp->addActionListener( this );
	ivPtWnd->ivPtButtonDown->addActionListener( this );
	ivPtWnd->ivPtButtonOk->addActionListener( this );
}

MSynthStructureWnd::MButtonListener::~MButtonListener()
{
	ivPtWnd->ivPtButtonAdd->removeActionListener( this );
	ivPtWnd->ivPtButtonRemove->removeActionListener( this );
	ivPtWnd->ivPtButtonUp->removeActionListener( this );
	ivPtWnd->ivPtButtonDown->removeActionListener( this );
	ivPtWnd->ivPtButtonOk->removeActionListener( this );
}

void MSynthStructureWnd::MButtonListener::onActionPerformed( void* ptSrc )
{
	if( ptSrc == ivPtWnd->ivPtButtonAdd )
	{
		MTransformerRtti* selectedRtti = (MTransformerRtti*) ivPtWnd->ivPtTransformerCombo->getModel()->getSelectedObject();
		MTransformer* ptTransformer = MTransformerRegistry::getInstance()->createTransformer( selectedRtti, 1 );
		if( ptTransformer )
			ivPtWnd->ivPtSynth->addTransformer( ptTransformer );
	}
	else if( ptSrc == ivPtWnd->ivPtButtonRemove )
	{
		ivPtWnd->ivPtSynth->removeSelectedTransformer();
	}
	else if( ptSrc == ivPtWnd->ivPtButtonDown )
	{
		ivPtWnd->ivPtSynth->transformerDown();
	}
	else if( ptSrc == ivPtWnd->ivPtButtonUp )
	{
		ivPtWnd->ivPtSynth->transformerUp();
	}
	else if( ptSrc == ivPtWnd->ivPtButtonOk )
	{
		DestroyWindow( ((MTopWndWin32Impl*)((MTopWnd*)ivPtWnd->getTopParent())->getImpl())->getHandle() );
	}
}

void MSynthStructureWnd::setParent( MWnd* pParent )
{
	MWnd::setParent( pParent );
	((MOkButton*)ivPtButtonOk)->setOnDelete( (MTopWnd*)getTopParent() );
}