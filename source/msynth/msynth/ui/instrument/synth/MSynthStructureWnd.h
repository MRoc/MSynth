#ifndef __MSynthStructureWnd
#define __MSynthStructureWnd

#include <gui/MWndCollection.h>
#include <gui/button/MOkButton.h>
#include <gui/label/MLabel.h>
#include <gui/combobox/MComboBox.h>
#include <gui/layout/MLayout.h>

#include "../../../model/Synth.h"

/**
 * a synth structure editor
 */
class MSynthStructureWnd :
	public MWndCollection
{
protected:

	class MScrComboListener;
	class MButtonListener;

	MLayout ivLayout;

	MLabel* ivPtLabelSrc;
	MLabel* ivPtLabelTransformer;
	MButton* ivPtButtonUp;
	MButton* ivPtButtonDown;
	MButton* ivPtButtonRemove;
	MButton* ivPtButtonAdd;
	MButton* ivPtButtonOk;
	MComboBox* ivPtOscCombo;
	MComboBox* ivPtTransformerCombo;
	MListView* ivPtTransformers;

	/**
	 * handles incoming events from the
	 * oscilator combo box
	 */
	MScrComboListener* ivPtComboSrcListener;

	/** 
	 * handles incoming events from the buttons
	 */
	MButtonListener* ivPtButtonListener;

	/**
	 * the synth editing
	 */
	Synth* ivPtSynth;
    
public:

	/**
	 * constructor
	 */
	MSynthStructureWnd();

	/**
	 * destructor
	 */
	virtual ~MSynthStructureWnd();

	/**
	 * sets the edited synth
	 */
	virtual void setSynth( Synth* ptSynth );

	/**
	 * returns the edited synth
	 */
	virtual Synth* getSynth();

	/**
	 * layouts all controls
	 */
	virtual void doLayout();

	virtual void setParent( MWnd* pParent );

protected:

	friend class MScrComboListener;
	/** 
	 * handles incoming events from the osci combo box
	 */
	class MScrComboListener :
		public MObject,
		public ISelectionListener
	{
	protected:
		MSynthStructureWnd* ivPtWnd;
	public:
		FULL_RTTI( MSynthStructureWnd::MScrComboListener, "MSynthStructureWnd::MScrComboListener" );
		MScrComboListener( MSynthStructureWnd* ptWnd );
		virtual ~MScrComboListener();
		/**
		 * invoked when selection changed
		 * newSelection = the new selected object
 		 * ptSource = the object list
		 */
		virtual void selectionChanged( MObject* newSelection, MObject* ptSource );
	};

	friend class MButtonListener;
	/**
	 * handles incoming events from the buttons
	 */
	class MButtonListener :
		public MObject,
		public IActionListener
	{
	protected:
		MSynthStructureWnd* ivPtWnd;
	public:
		FULL_RTTI( MSynthStructureWnd::MButtonListener, "MSynthStructureWnd::MButtonListener" );
		MButtonListener( MSynthStructureWnd* ptWnd );
		virtual ~MButtonListener();
		virtual void onActionPerformed( void* ptSrc );
	};

};

#endif