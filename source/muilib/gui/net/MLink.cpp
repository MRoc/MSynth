#include "MLink.h"
#include "../event/MMouseEvent.h"
#include <framework/io/MLogger.h>
#include <framework/MNativeTypes.h>
#include <framework/MUtil.h>

/** runtime type info */
MRtti MLink::gvRtti = MRtti( "MLink", MLink::createInstance );

/** constructor */
MLink::MLink() :
	MWnd(),
	ivUrl( "http://www.mroc.de" ),
	ivVisited( false ),
	ivNeutralColor( MColorMap::create(0,0,255) ),
	ivVisitedColor( MColorMap::create(128,0,128) )
{
}

/** constructor */
MLink::MLink( String url ) :
	MWnd(),
	ivUrl( url ),
	ivVisited( false ),
	ivNeutralColor( MColorMap::create(0,0,255) ),
	ivVisitedColor( MColorMap::create(128,0,128) )
{
}

/** destructor */
MLink::~MLink()
{
}

/** paints the link */
void MLink::paint( IGraphics* pG, const MRect& rect )
{
	pG->fillRect( rect, ivBkColor );
	pG->drawCenteredText(
		ivUrl,
		rect,
		ivVisited ? ivVisitedColor : ivNeutralColor );

}

/** process events ... */
bool MLink::processEvent( IUiEvent* pEvent )
{
	if( pEvent->getEventType() == IUiEvent::EVENT_MOUSE )
	{
		MMouseEvent* pMouseEvent = (MMouseEvent*) pEvent;
		if( pMouseEvent->getType() == MMouseEvent::MOUSE_DOWN &&
			pMouseEvent->getButton() == MMouseEvent::BUTTON1 )
		{
			if( ((LPARAM)ShellExecute( 0, "open", ivUrl, 0, 0, SW_SHOWNORMAL )) > 32 )
			{
				ivVisited = true;
				repaint();
			}
			else
				MLogger::logWarning(
					"MLink::onMouseDown: navigate to \"%s\" failed",
					ivUrl.getData() );
		}
		return true;
	}
	else
		return MWnd::processEvent( pEvent );
}

/** should set cursor */
bool MLink::onSetCursor()
{
	/*HCURSOR cursor = LoadCursor( 0, IDC_HAND );
	if( cursor )
	{
		SetCursor( cursor );
		return true;
	}*/
	return false;
}

/** loads from the given tree node */
void MLink::load( MTreeNode* ptNode )
{
	MWnd::load( ptNode );
	ivUrl = ptNode->getAttribute( "url", "http://www.mroc.de" );
	ivNeutralColor = MUtil::parseHexString( ptNode->getAttribute( "fgcolor", "0" ) );
	ivVisitedColor = MUtil::parseHexString( ptNode->getAttribute( "fgvisitedcolor", "ffffff" ) );
}

/** stores as tree node */
MTreeNode* MLink::save()
{
	MTreeNode* pBack = MWnd::save();
	pBack->setAttribute( "url", ivUrl );
	pBack->setAttribute( "fgcolor", MUtil::toHexString( ivNeutralColor ) );
	pBack->setAttribute( "fgvisitedcolor", MUtil::toHexString( ivVisitedColor ) );
	return pBack;
}

/** creates an instance of this class */
MObject* MLink::createInstance()
{
	return new MLink();
}

/** returns the runtime type info */
IRtti* MLink::getRtti() const
{
	return &gvRtti;
}