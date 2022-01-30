#include "MSynthStructureFrame.h"
#include <gui/MApp.h>

MSynthStructureFrame::MSynthStructureFrame( Synth* ptSynth ) :
	ivPtSynth( ptSynth ),
	ivPtView( 0 )
{
	setActionOnClose( MFrameWnd::DESTROY );
	ivBkColor = MColorMap::BK_COLOR3;
}

MSynthStructureFrame::~MSynthStructureFrame()
{
}

bool MSynthStructureFrame::create( MRect rect, MTopWnd* parent )
{
	ivPtView = new MSynthStructureWnd();
	ivPtView->setSynth( ivPtSynth );

	bool back = MFrameWnd::create( MRect( 0, 0, 340, 370 ), parent );
	if( back )
	{
		centerWindow();
		addChild( ivPtView );
		setText( "Synth Structure Editor" );
		doLayout();
	}
	return back;
}

void MSynthStructureFrame::doLayout()
{
	ivPtView->setRect( MRect( MPoint(), getClientSize() ) );
}