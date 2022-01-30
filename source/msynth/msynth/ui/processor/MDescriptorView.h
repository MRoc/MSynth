/**
 * (C)2000-2003 MRoc
 */

#ifndef __MDESCRIPTORVIEW
#define __MDESCRIPTORVIEW

#include <gui/label/MLabel.h>
#include "MAbstractDescriptorView.h"
#include "../control/MFloatGui.h"

class MDescriptorView :
	public MAbstractDescriptorView
{
protected:

	MLabel*				ivLabelName;

	std::vector<MWnd*>	ivArPtComponents;
	std::vector<int>	ivArIndex;

public:

	MDescriptorView();
	MDescriptorView( IDescriptor *descriptor, MColor bkColor );
	virtual ~MDescriptorView();

	virtual void		paint( IGraphics* ptGraphics, const MRect &rect );
	virtual void		doLayout();
	virtual void		updateFromModel();

	virtual void		setDescriptor( IDescriptor *descriptor );

	static MAbstractDescriptorView* createInstance();

};

#endif

