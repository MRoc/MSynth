#include "MComboGui.h"
#include <framework/primitive/MObjString.h>

INIT_RTTI( MComboGui::MComboBoxListener, "MComboGui::MComboBoxListener" );

MComboGui::MComboGui( MDiscreteComboControl* ptControl, MColor bkColor ) :
	MComboBox(),
	ivPtControl( ptControl ),
	ivPtListener( 0 )
{
	ASSERT( ivPtControl );
	setBkColor( bkColor );
	ivPtListener = new MComboBoxListener( ptControl, this );
	ivPtPopup->getModel()->addSelectionListener( ivPtListener );

	unsigned int count = ivPtControl->getValueCount();
	for( unsigned int i=0;i<count;i++ )
		getModel()->add( new MObjString( ivPtControl->getStringAt( i ) ) );

	getModel()->setSelection( ivPtControl->getSelectedIndex() );
}

MComboGui::~MComboGui()
{
	ivPtPopup->getModel()->removeSelectionListener( ivPtListener );
	SAFE_DELETE( ivPtListener );
}

MComboGui::MComboBoxListener::MComboBoxListener( MDiscreteComboControl* ptControl, MComboGui* ptCombo ) :
	ivPtControl( ptControl ),
	ivPtCombo( ptCombo )	
{
}

MComboGui::MComboBoxListener::~MComboBoxListener()
{
}
		
void MComboGui::MComboBoxListener::selectionChanged( MObject* newSelection, MObject* ptSource )
{
	ivPtControl->setSelectedIndex( ivPtCombo->getSelection() );
}

void MComboGui::MComboBoxListener::valueChanged( MControlListenerEvent *anEvent )
{
	// TO DO: CHECK OUT IF THIS DOESN'T PRODUCE ERRORS, NEVER INVOKED
	/*if( ivPtControl->getSelectedIndex() != getModel()->getSelection() )
	{
		getModel()->setSelection( ivPtControl->getSelectedIndex() );
	}*/
}

String MComboGui::MComboBoxListener::getClassName()
{
	return "MComboGui::MComboBoxListener";
}