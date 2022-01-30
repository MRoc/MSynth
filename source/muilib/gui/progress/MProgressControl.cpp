#include "MProgressControl.h"
#include <framework/MUtil.h>
#include <framework/primitive/MFloat.h>

/** runtime type info */
MRtti MProgressControl::gvRtti = MRtti( "MProgressControl", MProgressControl::createInstance );

/** constructor */
MProgressControl::MProgressControl() :
	MWnd(),
	ivValue( 0.0f ),
	ivFgColor( MColorMap::RED )
{
}

/** destructor */
MProgressControl::~MProgressControl()
{
}

/** paints the progress bar */
void MProgressControl::paint( IGraphics* pG, const MRect &rect )
{
	pG->fillRect( rect, ivBkColor );
	pG->fillRect(
		MRect(
			rect.getX(),
			rect.getY(),
			(unsigned int)(rect.getWidth() * ivValue),
			rect.getHeight() ),
			ivFgColor );
}

/** invoked to show the progress of a job 0.0 <= value <= 1.0 */
void MProgressControl::onProgress( float value )
{
	ivValue = value;
	repaint();
}

/** invoked when the job was finished */
void MProgressControl::onFinished()
{
	ivValue = 1.0f;
	repaint();
}

/** loads from the given tree node */
void MProgressControl::load( MTreeNode* ptNode )
{
	MWnd::load( ptNode );
	ivFgColor = MUtil::parseHexString( ptNode->getAttribute( "fgcolor", "0" ) );
	ivValue = MFloat::parse( ptNode->getAttribute( "value", "0.0" ) );
}

/** stores as tree node */
MTreeNode* MProgressControl::save()
{
	MTreeNode* pBack = MWnd::save();
	pBack->setAttribute( "fgcolor", MUtil::toHexString( ivFgColor ) );
	pBack->setAttribute( "value", MFloat::toString( ivValue ) );
	return pBack;
}