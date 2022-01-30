#ifndef __MComboGui
#define __MComboGui

#include <framework/MTypes.h>
#include <framework/MUtil.h>
#include <framework/list/MSelectableObjectList.h>
#include <gui/combobox/MComboBox.h>

#include "../../model/control/MDiscreteComboControl.h"

class MComboBoxListener2;

class MComboGui :
	public MComboBox
{
protected:

	class MComboBoxListener;

	MDiscreteComboControl* ivPtControl;

	MComboBoxListener* ivPtListener;

public:

	MComboGui( MDiscreteComboControl* ptControl, MColor bkColor );
	virtual ~MComboGui();

protected:

	class MComboBoxListener :
		public MObject,
		public ISelectionListener,
		public IControlListener
	{
		protected:
			MDiscreteComboControl* ivPtControl;
			MComboGui* ivPtCombo;
		public:
			FULL_RTTI( MComboGui::MComboBoxListener, "MComboGui::MComboBoxListener" );
			MComboBoxListener( MDiscreteComboControl* ptControl, MComboGui* ptCombo );
			virtual ~MComboBoxListener();
			virtual void selectionChanged( MObject* newSelection, MObject* ptSource );
			virtual void valueChanged( MControlListenerEvent *anEvent );
			virtual String getClassName();
	};
};

#endif