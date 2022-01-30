/*

	MSynthScrollView (C)2000/2001 MRoc hifiShock

*/

#ifndef __MSYNTHVIEW
#define __MSYNTHVIEW

#include "../../processor/MDescriptorView.h"
#include "../../processor/MDescriptorViewFactory.h"
#include "../../../model/Synth.h"
#include "../../../model/interfaces/IDescriptor.h"
#include "../../../model/interfaces/IDescriptorCollection.h"
#include <gui/MWndCollection.h>
#include <framework/primitive/MInsets.h>

class MSynthScrollView :
	public MWndCollection
{
protected:

	class MSynthListener;

	MSynthListener* ivPtListener;

	int ivSpaceX;
	int ivSpaceY;

	int ivHeight;
	int ivWidth;

	MInsets ivInsets;

	Synth* ivPtSynth;

	std::vector<MAbstractDescriptorView*> ivArPtDescriptorViews;

public:

	MSynthScrollView();
	virtual	~MSynthScrollView();

	virtual Synth*	getSynth();
	virtual void	setSynth( Synth* ptSynth );

	virtual void	doLayout();
	virtual void	updateFromModel();
	virtual void	updateFromModel( unsigned int index );
	virtual void	updateGui();
	virtual MSize	getPreferredSize();

protected:
	
	MSize			calculateSize();
	void			deleteDescriptorViews();

	MAbstractDescriptorView* createDescriptorView( IDescriptor* ptDesc );

	friend class MSynthListener;
	class MSynthListener :
		public MObject,
		public IInstrumentListener
	{
	private:
		MSynthScrollView* ivPtScrollView;
	public:
		FULL_RTTI( MSynthScrollView::MSynthListener, "MSynthScrollView::MSynthListener" );
		MSynthListener( MSynthScrollView* ptScrollView );
		virtual ~MSynthListener();
		virtual void stateChanged( MInstrumentEvent anEvent );
		virtual String getInstrumentListenerName();
	private:
		void layoutControls();
	};
};

#endif