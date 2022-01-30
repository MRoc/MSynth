#include "MToolTipCtrl.h"
#include <gui/MWndUtils.h>

#define TOOLTIPWND_ID 1024

MToolTipCtrl* MToolTipCtrl::gvInstance = 0;

MToolTipCtrl::MToolTipCtrl() :
	ptToolTipWnd( 0 )
{
	ptToolTipWnd = new MToolTipWnd();
}

MToolTipCtrl::~MToolTipCtrl()
{
	SAFE_DELETE( ptToolTipWnd );
};

void MToolTipCtrl::showToolTipInfo( MWnd* ptWnd, String text )
{
	MRect rect = MWndUtils::getScreenOffsetRect( ptWnd );
	ptToolTipWnd->show( text, MPoint( rect.getX(), rect.getY() - 20 ) );
}

void MToolTipCtrl::showToolTipInfo( MWnd* ptWnd, String text, int x, int y )
{
	ptToolTipWnd->show( text, MPoint( x, y ) );
}

void MToolTipCtrl::hideToolTipInfo()
{
	if( ptToolTipWnd )
		ptToolTipWnd->hide();
}

MToolTipCtrl* MToolTipCtrl::getInstance()
{
	if( gvInstance == 0 )
		gvInstance = new MToolTipCtrl();
	return gvInstance;
}

void MToolTipCtrl::release()
{
	if( gvInstance )
		delete gvInstance;
}
