#ifndef __MSynthStructureFrame
#define __MSynthStructureFrame

#include <gui/framewnd/MFrameWnd.h>
#include "MSynthStructureWnd.h"

class MSynthStructureFrame :
	public MFrameWnd
{
protected:

	Synth* ivPtSynth;
	MSynthStructureWnd* ivPtView;

public:
	MSynthStructureFrame( Synth* ptSynth );
	virtual ~MSynthStructureFrame();

	virtual bool create( MRect rect, MTopWnd* parent );
	virtual void doLayout();
};

#endif