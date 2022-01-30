/**
 * (C)2001-2003 MRoc
 */

#ifndef __MMULTIOSCCONTROL
#define __MMULTIOSCCONTROL

#include <framework/MObject.h>
#include <framework/MDefaultFactory.h>

#include "MAbstractDescriptorView.h"
#include "MDescriptorViewFactory.h"

#include <gui/label/MLabel.h>
#include <gui/counter/MCountControl.h>
#include <gui/combobox/MComboBox.h>
#include "../../../resource.h"
#include "../../model/interfaces/IDescriptor.h"
#include "../../model/source/MMultiOsc.h"
#include "../../model/source/MSourceRegistry.h"

class MMultiOscControl :
	public MAbstractDescriptorView
{
protected:

	class MOscWrapper;

	MLabel*						ivLabelName;

	unsigned int				ivOscCount;
	MOscWrapper**				ivPtControls;
	
public:

	FULL_RTTI_SUPER( MMultiOscControl, "MMultiOscControl", MAbstractDescriptorView );

	MMultiOscControl();
	MMultiOscControl( IDescriptor *descriptor, MColor bkColor );
	virtual ~MMultiOscControl();

	virtual void				paint( IGraphics* ptGraphics, MRect &rect );
	virtual void				updateFromModel();
	virtual void				setDescriptor( IDescriptor *descriptor );

	static MAbstractDescriptorView *createInstance();

protected:

	void						createView( int index );

	class MOscWrapper :
		public MWndCollection,
		public ISelectionListener,
		public ISpinButtonModelListener
	{
	protected:
		MMultiOsc* ivPtModel;
		unsigned int ivIndex;
		MComboBox* ivPtCombo;
		MAbstractDescriptorView* ivPtView;
		MCountControl* ivPtCounter;
	public:
		FULL_RTTI( MMultiOscControl::MOscWrapper, "MMultiOscControl::MOscWrapper" );

		MOscWrapper(MMultiOsc* pModel, unsigned int index);
		virtual ~MOscWrapper();

		virtual MComboBox* getCombo();
		virtual MAbstractDescriptorView* getView();
		virtual MCountControl* getCounter();

		virtual void selectionChanged( MObject* newSelection, MObject* ptSource );
		virtual void valueChanged( MSpinButtonModel* pModel );
		virtual String getClassName();

		virtual void createView();
		virtual void updateFromModel();
	};
};

#endif

