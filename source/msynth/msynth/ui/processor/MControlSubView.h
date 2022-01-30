#ifndef __MControlSubView
#define __MControlSubView

#include <gui/MWndCollection.h>
#include "../control/MFloatGui.h"
#include "../control/MBoolGui.h"
#include "../../model/control/IControlCollection.h"

class MControlSubView :
	public MWndCollection
{
protected:

	IControlCollection*	ivPtControlCollection;
	std::vector<MWnd*> ivArControls;

public:

	MControlSubView();
	MControlSubView( IControlCollection* ptControlCollection, MColor bkColor );

	virtual ~MControlSubView();
	virtual void paint( IGraphics* ptGraphics, const MRect &rect );
	virtual void doLayout();
};

#endif