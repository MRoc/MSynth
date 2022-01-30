#include "MApp.h"

#include <framework/thread/IRunnable.h>
#include <framework/io/MLogger.h>
#include <framework/MDefaultFactory.h>

#include "MFont.h"
#include "MTopWnd.h"
#include "MWndManager.h"
#include "event/MPositionEvent.h"
#include "event/MMouseEvent.h"
#include "event/MHoverEvent.h"
#include "event/MKeyEvent.h"
#include "event/MFocusEvent.h"
#include "event/MCommandEvent.h"
#include "event/MPaintEvent.h"
#include "event/MActivateEvent.h"
#include "event/MSizeEvent.h"
#include "event/MDestroyEvent.h"
#include "event/MCloseEvent.h"
#include "event/MWin32ResizeRequest.h"
#include "event/MNativeEvent.h"
#include "event/MPositionEvent.h"
#include "win32impl/MTopWndWin32Impl.h"
#include "win32impl/MAppWin32Impl.h"

#include "framewnd/MFrameWnd.h"

#include "button/MButton.h"
#include "button/MToggleButton.h"
#include "button/MArrowButtonRenderer.h"
#include "button/MFlatTextButtonRenderer.h"
#include "button/MTextButtonRenderer.h"
#include "button/MTransparentBitmapButtonRenderer.h"
#include "button/MCheckBoxRenderer.h"
#include "button/MRadioButtonRenderer.h"
#include "imagewnd/MImageWnd.h"
#include "net/MLink.h"
#include "label/MLabel.h"
#include "scrollbar/MScrollBar.h"
#include "scrollbar/MScrollPane.h"
#include "progress/MProgressControl.h"
#include "counter/MCountControl.h"
#include "textfield/MTextField.h"
#include "list/MListView.h"
#include "list/MScrolledListView.h"
#include "combobox/MComboBox.h"
#include "spinbutton/MSpinButton.h"

#define WM_POST_WORK_TO_GUITHREAD ( WM_USER + 9345 )
#define WM_END_MODAL ( WM_USER + 9346 )

unsigned int MApp::gvMsgCount = 0;

/*
todo:

platform indep. keystrokes
*/

extern MApp* createApp();

/** the singleotn instance */
MApp* MApp::gvInstance = 0;

/** returns the singleton instance */
MApp* MApp::getInstance()
{
	if( gvInstance == 0 )
	{
		gvInstance = createApp();
		registerUiClasses();
	}
	return gvInstance;
}

/** terminates the singleton instance */
void MApp::release()
{
	if( gvInstance )
	{
		MDefaultFactory::release();
		delete gvInstance;
		gvInstance = 0;
	}
}

/** constructor */
MApp::MApp() :
	ivPtMainWnd( 0 ),
	ivImpl( 0 )
{
	ivImpl = new MAppWin32Impl();
	if( gvInstance == 0 )
		gvInstance = this;
}

/** destructor */
MApp::~MApp()
{
	SAFE_DELETE( ivImpl );
	MWndManager::release();
	MFont::release();
}

/** invoked when app is coming up */
bool MApp::onInit()
{
	return true;
}

/** do the message process */
int MApp::work()
{
	return ivImpl->work();
}

/** the message loop for modal stack */
int MApp::modalWork()
{
	return ivImpl->modalWork();
}

/** invoked when app is exiting */
bool MApp::onExit()
{
	if( ivPtMainWnd )
	{
		delete ivPtMainWnd;
		ivPtMainWnd = 0;
	}
	return true;
}

/** returns the native app object */
MAppImpl* MApp::getImpl()
{
	return ivImpl;
}

/** sets the application parameters */
void MApp::setParams( String file )
{
	ivParams = file;
}

/** sets the quit flag on true */
void MApp::setQuit()
{
	ivImpl->setQuit();
}

/** sets the main window */
void MApp::setMainWnd( MTopWnd* pWnd )
{
	ivPtMainWnd = pWnd;
}

/** returns the main window */
MTopWnd* MApp::getMainWnd()
{
	return ivPtMainWnd;
}

/** the code behind pToRun will be invoked from ui thread */
void MApp::doWorkInGuiThread( IRunnable* pToRun )
{
	ivImpl->doWorkInGuiThread( pToRun );
}

/** Microsoft Windows main entry point */
int APIENTRY WinMain(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPSTR lpCmdLine,
		int nCmdShow )
{
#ifdef _DEBUG
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	MLogger::logMessage( "<winmain commandline=\"%s\"/>\n", lpCmdLine );

	int back = 0;
	MApp* app = MApp::getInstance();
	((MAppWin32Impl*)app->getImpl())->setHandle( hInstance );
	app->setParams( lpCmdLine );
	if( app->onInit() == true )
	{
		back = app->work();
		app->onExit();
	}
	MApp::release();

	return back;
}

/** starts a modal loop for the given window */
int MApp::startModal( MTopWnd* pWnd )
{
	if( ivModalStack.size() > 0 && ivModalStack.top() == pWnd )
	{
		MLogger::logWarning( "MApp::startModal failed: given window is already modal!" );
		return -1;
	}
	else
	{
		ivModalStack.push( pWnd );
		return modalWork();
	}
}

/** stops a modal loop for the given window */
void MApp::stopModal( MTopWnd* pWnd, int returncode )
{
	if( ivModalStack.top() != pWnd )
		MLogger::logError(
			"MApp::stopModal failed: given window is not modal!" );
	else
	{
		PostMessage( 0, WM_END_MODAL, returncode, 0 );
		ivModalStack.pop();
	}
}

/** returns the top modal window or null */
MTopWnd* MApp::getModal()
{
	if( ivModalStack.size() )
		return ivModalStack.top();
	else
		return 0;
}

MUiEventDispatcher* MApp::getDisptacher()
{
	return &ivDispatcher;
}

void MApp::registerUiClasses()
{
	MDefaultFactory* pFac = MDefaultFactory::getInstance();
	if( pFac )
	{
		pFac->store( &MWnd::gvRtti );
		pFac->store( &MWndCollection::gvRtti );
		pFac->store( &MTopWnd::gvRtti );
		pFac->store( &MFrameWnd::gvRtti );
		pFac->store( &MButton::gvRtti );
		pFac->store( &MToggleButton::gvRtti );
		pFac->store( &MArrowButtonRenderer::gvRtti );
		pFac->store( &MFlatTextButtonRenderer::gvRtti );
		pFac->store( &MTextButtonRenderer::gvRtti );
		pFac->store( &MTransparentBitmapButtonRenderer::gvRtti );
		pFac->store( &MCheckBoxRenderer::gvRtti );
		pFac->store( &MRadioButtonRenderer::gvRtti );
		pFac->store( &MLabel::gvRtti );
		pFac->store( &MProgressControl::gvRtti );
		pFac->store( &MLink::gvRtti );
		pFac->store( &MCounter::gvRtti );
		pFac->store( &MCountControl::gvRtti );
		pFac->store( &MScrollBar::gvRtti );
		pFac->store( &MScrollPane::gvRtti );
		pFac->store( &MTextField::gvRtti );
		pFac->store( &MListView::gvRtti );
		pFac->store( &MScrolledListView::gvRtti );
		pFac->store( &MComboBox::gvRtti );
		pFac->store( &MSpinButton::gvRtti );
		pFac->store( &MImageWnd::gvRtti );
	}
}