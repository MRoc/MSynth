#include "MInspectorCombo.h"

INIT_RTTI( MInspectorCombo::MInspectorSelectionListener, "MInspectorCombo::MInspectorSelectionListener" );
INIT_RTTI( MInspectorCombo::MTargetRegistryListener, "MInspectorCombo::MTargetRegistryListener" );

MInspectorCombo::MInspectorCombo() : 
	MComboBox(),
	ivPtBar( 0 ),
	ivPtInspector( 0 ),
	ivPtListener( 0 ),
	ivPtRegistryListener( 0 )
{
	ivPtListener = new MInspectorSelectionListener( this );
	ivPtPopup->getModel()->addSelectionListener( ivPtListener );
	ivPtRegistryListener = new MTargetRegistryListener( this );
}

MInspectorCombo::~MInspectorCombo()
{
	ivPtPopup->getModel()->removeSelectionListener( ivPtListener );
	setBar( 0 );
	SAFE_DELETE( ivPtListener );
	SAFE_DELETE( ivPtRegistryListener );
}

void MInspectorCombo::setBar( MBar* ptBar )
{
	ivPtBar = ptBar;
	if( ivPtBar )
		init();
}

MBar* MInspectorCombo::getBar()
{
	return ivPtBar;
}

void MInspectorCombo::setNoteInspector( MNoteInspector* ptInspector )
{
	ivPtInspector = ptInspector;
	if( ivPtInspector )
		this->onselectionChanged();
}

MNoteInspector* MInspectorCombo::getNoteInspector()
{
	return ivPtInspector;
}

void MInspectorCombo::init()
{
	ASSERT( ivPtInspector );

	// delete everything first
	ivPtPopup->getModel()->deleteAll();

	// add volume inspector item if nescessary
	if( ivPtBar->getNoteList()->hasVolumeControl() )
		ivPtPopup->getModel()->add( new MInspectorComboItem( "Volume", ivPtInspector->getInspectorByName("Volume"), 0 ) );

	// collect float event inspectors
	IAutomationTracker* ptTracker = ivPtBar->getTracker();
	std::vector<IEventTarget*> collector;
	ptTracker->collectTargets( collector );
	unsigned int count = collector.size();
	for( unsigned int i=0;i<count;i++ )
	{
		ivPtPopup->getModel()->add(
			new MInspectorComboItem(
				collector[ i ]->getName(),
				ivPtInspector->getInspectorByName( "floatEvent" ),
				collector[ i ] ) );
	}

	// set selection if available
	if( ivPtPopup->getModel()->getLength() > 0 )
		ivPtPopup->getModel()->setSelection( 0 );
}

bool MInspectorCombo::hasTarget( IEventTarget* ptTarget )
{
	return getIndex( ptTarget ) != -1;
}

int MInspectorCombo::getIndex( IEventTarget* ptTarget )
{
	int back = -1;
	MSelectableObjectList* ptList = ivPtPopup->getModel();
	unsigned int count = ptList->getLength();
	for( unsigned int i=1;i<count;i++ )
		if( ((MInspectorComboItem*)ptList->get( i ))->getTarget() == ptTarget )
		{
			back = i;
			break;
		}
	return back;
}

void MInspectorCombo::selectTarget( IEventTarget* ptTarget )
{
	ASSERT( hasTarget( ptTarget ) );
	int index = getIndex( ptTarget );
	ivPtPopup->getModel()->setSelection( index );
}

void MInspectorCombo::onselectionChanged()
{
	int curSel = ivPtPopup->getModel()->getSelection();
	if( curSel != -1 && ivPtInspector )
	{
		if( ivPtBar->getNoteList()->hasVolumeControl() &&
			curSel == 0 )
		{
			ivPtInspector->selectInspectorByName( "Volume" );
		}
		else
		{
			ivPtInspector->selectInspectorByName( "floatEvent" );
			IEventTarget* ptEventTarget = ((MInspectorComboItem*)ivPtPopup->getModel()->getSelectedObject())->getTarget();
			IBarInspector* pti = ivPtInspector->getInspectorByName( "floatEvent" );
			MFloatEventInspector* ptfi = (MFloatEventInspector*) pti;
			ptfi->setCurrentTarget( ptEventTarget );
		}
		ivPtInspector->repaint();
	}
}