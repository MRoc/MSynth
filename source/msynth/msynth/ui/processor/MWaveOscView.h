/**
 * (C)2000-2003 MRoc
 */

#ifndef __MWAVEOSCVIEW
#define __MWAVEOSCVIEW

#include <framework/MTypes.h>

#include <gui/label/MLabel.h>
#include <gui/MGuiUtils.h>
#include <gui/button/MToggleButton.h>
#include <gui/layout/MLayout.h>

#include "MAbstractDescriptorView.h"
#include "MWaveView.h"
#include "../../../resource.h"
#include "../../model/source/MWaveOsc.h"
#include "../../model/interfaces/IDescriptor.h"
#include "../control/MFloatGui.h"

class MWaveOscView :
	public MAbstractDescriptorView
{
protected:

	class MSaveButtonHandler;
	class MLoadButtonHandler;
	class MReverseButtonHandler;
	class MLoopButtonHandler;

	int							ivHorizontalSpace,
								ivVerticalSpace;
	std::vector<MFloatGui*>		ivFloatGuis;

	MLabel*						ivLabelName;
	MLabel*						ivPtLabelSampleName;
	MButton*					ivPtButtonLoad;
	MButton*					ivPtButtonSave;
	MToggleButton*				ivPtButtonLoop;
	MToggleButton*				ivPtButtonReverse;
	MWaveView*					ivPtView;

	MSaveButtonHandler*			ivPtSaveButtonHandler;
	MLoadButtonHandler*			ivPtLoadButtonHandler;
	MReverseButtonHandler*		ivPtReverseButtonHandler;
	MLoopButtonHandler*			ivPtLoopButtonHandler;

	MLayout						ivLayout;

public:

	MWaveOscView();
	MWaveOscView( IDescriptor *descriptor, MColor bkColor);
	virtual ~MWaveOscView();

	virtual void doLayout();
	virtual void paintBackground( IGraphics* ptGraphics, MRect &rect );
	virtual void updateFromModel();
	virtual void setDescriptor( IDescriptor *descriptor );
	static MAbstractDescriptorView* createInstance();

protected:

	friend class MSaveButtonHandler;
	class MSaveButtonHandler :
		public MObject,
		public IActionListener
	{
	protected:
		MWaveOscView* ivPtView;
	public:
		FULL_RTTI( MWaveOscView::MSaveButtonHandler, "MWaveOscView::MSaveButtonHandler" );
		MSaveButtonHandler( MWaveOscView* ptView );
		virtual ~MSaveButtonHandler();
		virtual void onActionPerformed( void* ptSrc );
	};

	friend class MLoadButtonHandler;
	class MLoadButtonHandler :
		public MObject,
		public IActionListener
	{
	protected:
		MWaveOscView* ivPtView;
	public:
		FULL_RTTI( MWaveOscView::MLoadButtonHandler, "MWaveOscView::MLoadButtonHandler" );
		MLoadButtonHandler( MWaveOscView* ptView );
		virtual ~MLoadButtonHandler();
		virtual void onActionPerformed( void* ptSrc );
	};

	friend class MReverseButtonHandler;
	class MReverseButtonHandler :
		public MObject,
		public IActionListener
	{
	protected:
		MWaveOscView* ivPtView;
	public:
		FULL_RTTI( MWaveOscView::MReverseButtonHandler, "MWaveOscView::MReverseButtonHandler" );
		MReverseButtonHandler( MWaveOscView* ptView );
		virtual ~MReverseButtonHandler();
		virtual void onActionPerformed( void* ptSrc );
	};

	friend class MLoopButtonHandler;
	class MLoopButtonHandler :
		public MObject,
		public IActionListener
	{
	protected:
		MWaveOscView* ivPtView;
	public:
		FULL_RTTI( MWaveOscView::MLoopButtonHandler, "MWaveOscView::MLoopButtonHandler" );
		MLoopButtonHandler( MWaveOscView* ptView );
		virtual ~MLoopButtonHandler();
		virtual void onActionPerformed( void* ptSrc );
	};
};

#endif

