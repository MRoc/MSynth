#ifndef __MFxSlotWnd
#define __MFxSlotWnd

#include <gui/MWndCollection.h>
#include <gui/label/MLabel.h>
#include <gui/combobox/MComboBox.h>
#include "../processor/MDescriptorView.h"
#include "../processor/MDescriptorViewFactory.h"
#include "../../model/MFxRack.h"
#include "../../model/transformer/MTransformerRegistry.h"

/**
 * a view displaying a slot of a effect rack
 */
class MFxSlotWnd :
	public MWndCollection
{
protected:

	class MFxSlotListener;

	MInsets ivInsets;

	/**
	 * the model of this view,
	 * a slot in the effect rack
	 */
	MFxSlot* ivPtModel;

	/**
	 * the effect slot listener
	 */
	MFxSlotListener* ivPtListener;

	/**
	 * the label displaying the effect index
	 */
	MLabel* ivPtLabel;

	/**
	 * the combo box using the MSelectableObjectList
	 * stored in the effect slot
	 */
	MComboBox* ivPtCombo;

	/**
	 * the effect view, can be null
	 */
	MDescriptorView* ivPtView;

public:

	/**
	 * constructor
	 */
	MFxSlotWnd( unsigned int index, MFxSlot* ptSlot );

	/**
	 * destructor
	 */
	virtual ~MFxSlotWnd();

	/**
	 * paints the background
	 */
	virtual void paint( IGraphics* ptGraphics, const MRect &rect );

	/**
	 * deletes (if nescessary) and
	 * creates (if nescessary)
	 * a new MDescriptorView
	 *
	 * autosize - if true the parent
	 * windows and frames are relayouted
	 * and autosized.
	 */
	virtual void updateView( bool autosize);

	/**
	 * layouts all components
	 */
	virtual void doLayout();

	/**
	 * returns the preferred size of this control
	 */
	virtual MSize getPreferredSize();

protected:

	friend class MFxSlotListener;
	/**
	 * handles incoming events from the fx slot
	 */
	class MFxSlotListener :
		public MObject,
		public IFxSlotListener
	{
	protected:

		/**
		 * the view
		 */
		MFxSlotWnd* ivPtView;

	public:

		FULL_RTTI( MFxSlotWnd::MFxSlotListener, "MFxSlotWnd::MFxSlotListener" );

		/** 
		 * constructor
		 */
		MFxSlotListener( MFxSlotWnd* ptView );

		/**
		 * destructor
		 */
		virtual ~MFxSlotListener();

		/**
		* invoked when a processor changed
		* ptOld - the previous slected transformer, can be null will be deleted after
		* ptNew - the new selected transformer, can be null
		*/
		virtual void onProcessorChanged( MTransformer* ptOld, MTransformer* ptNew );
	};
};

#endif