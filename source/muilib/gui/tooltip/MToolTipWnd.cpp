#include "MToolTipWnd.h"
#include "../MUiGraphics.h"
#include <framework/io/MLogger.h>
#include "../MApp.h"

MToolTipWnd::MToolTipWnd() :
	MTopWnd()
{
	ivBkColor = RGB( 248, 255, 43 );
	ivFgColor = RGB( 0, 0, 0 );
	create();
}

MToolTipWnd::~MToolTipWnd()
{
}

bool MToolTipWnd::create()
{
	/*ivHwnd =
		CreateWindowEx(
			WS_EX_TOPMOST,
			"MTopWnd",
			NULL,
			WS_POPUP,
			0,
			0,
			10,
			10,
			NULL,
			NULL,
			MApp::getInstance()->getHInstance(),
			NULL );
	if( ivHwnd != NULL )
		gvMapHwnd2Wnd.insert( Hwnd2OsWndMap::value_type( ivHwnd, this ) );
	else
		MLogger::logError(
			"MToolTipWnd::create: CreateWindowEx failed" );

	return ivHwnd != NULL;*/

	return ivImpl->create( MTopWndImpl::POPUP_WND, MRect( 0, 0, 10, 10 ), this, 0 );
};

void MToolTipWnd::show( String text, MPoint point )
{
	setText( text );

	MSize textSize = MFont::getFont()->getTextSize( getText() );
	setRect(
		MRect(
			point,
			MSize(
				textSize.getWidth() + 4,
				textSize.getHeight() + 4 ) ) );
	ivImpl->setVisibleNoActivate( true );
}

void MToolTipWnd::hide()
{
	setVisible( false );
}

void MToolTipWnd::paint( IGraphics* pG, const MRect &rect )
{
	pG->fillRect( rect, ivBkColor );
	pG->drawCenteredText( getText(), rect, ivFgColor );
}