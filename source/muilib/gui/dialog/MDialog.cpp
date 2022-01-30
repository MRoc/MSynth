#include "MDialog.h"
#include "../MApp.h"

/** constructor accepting a dialog template resource id */
MDialog::MDialog()
{
}

/** destructor */
MDialog::~MDialog()
{
}

/** shows the modal dialog */
MDialog::DlgResult MDialog::doModal()
{
	onInit();
	centerWindow();
	setVisible( true );
	MDialog::DlgResult back = (MDialog::DlgResult) MApp::getInstance()->startModal( this );
	onClose();
	return back;
}

/** invoked when the dialog is created */
void MDialog::onInit()
{
	/*ASSERT( ivHwnd );
	RECT wRect;
	GetWindowRect( ivHwnd, &wRect );
	int w = wRect.right - wRect.left;
	int h = wRect.bottom - wRect.top;
	int sW = GetSystemMetrics( SM_CXFULLSCREEN );
	int sH = GetSystemMetrics( SM_CYFULLSCREEN );
	int x = (sW-w)/2;
	int y = (sH-h)/2;
	setRect( MRect( x, y, w, h ) );*/
}

/** invoked when the dialog is destroyed */
void MDialog::onClose()
{
}

/**
 * this function terminates the dialog.
 *
 * can only invoked when doModal was invoked
 * before. after invocation of close, doModal
 * returns the the return code provided to close.
 */
void MDialog::close( MDialog::DlgResult returnCode )
{
	MApp::getInstance()->stopModal( this, returnCode );
}

/** process a system event */
bool MDialog::processEvent( IUiEvent* pEvent )
{
	switch( pEvent->getEventType() )
	{
	case IUiEvent::EVENT_CLOSE:
		{
			storeWindowPosition();

			unsigned int count = ivListeners.getListenerCount();
			for( unsigned int i=0;i<count;i++ )
				((IFrameWndListener*)ivListeners.getListener( i ))->onClose( this );
			onClose();
			MApp::getInstance()->stopModal( this, DlgResult::CANCEL );
			return true;
		}
		break;
	default:
		return MFrameWnd::processEvent( pEvent );
		break;
	}
}