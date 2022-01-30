#include "Splash1.h" 
#include "../../../resource.h"
#include <framework/MUtil.h>
#include "../../../MAppData.h"
#include <gui/MApp.h>

/**
 * the singleton instance
 */
CSplashWnd* CSplashWnd::gvInstance = 0;

/**
 * the runtime type info
 */
MRtti CSplashWnd::gvRtti = MRtti( "CSplashWnd", 0 );

/** 
 * returns the singleton instance
 */
CSplashWnd* CSplashWnd::getInstance()
{
	if( gvInstance == 0 )
		gvInstance = new CSplashWnd();
	return gvInstance;
}

/** 
 * releases the singleton instance
 */
void CSplashWnd::release()
{
	if( gvInstance != 0 )
	{
		delete gvInstance;
		gvInstance = 0;
	}
}

/**
 * constructor
 */
CSplashWnd::CSplashWnd()
{
	ivImage.loadImage( IDB_SPLASH );
	ivTimer.addListener( this );
}

/**
 * destructor
 */
CSplashWnd::~CSplashWnd()
{
	ivTimer.removeListener( this );
}

/**
 * creates thw window
 */
bool CSplashWnd::create()
{
	return ivImpl->create(
		MTopWndImpl::POPUP_WND,
		MRect( 0, 0, ivImage.getWidth(), ivImage.getHeight() ),
		this,
		0 );
}

/**
 * paints the window
 */
void CSplashWnd::paint( IGraphics* pG, const MRect &rect )
{
	MRect textRect( 5, ivImage.getHeight() - 17, ivImage.getWidth() - 10, 12 );

	pG->drawImage( &ivImage, 0, 0 );

	pG->drawText(
		ivText, 
		textRect,
		MColorMap::WHITE );
}

/**
 * starts the timer and sets visible
 */
void CSplashWnd::start()
{
	setVisible( true );
	ivTimer.startTimer( 2000 );
}

/**
 * the timer callback
 */
void CSplashWnd::onActionPerformed( void* pSrc )
{
	ivTimer.stopTimer();
	if( MApp::getInstance()->getMainWnd() )
	{
		//MApp::getInstance()->getMainWnd()->setFocus();
	}
	release();
}

/**
* returns the runtime type info
*/
IRtti* CSplashWnd::getRtti() const
{
	return &CSplashWnd::gvRtti;
}

/**
* query interface
*/
void* CSplashWnd::getInterface( const String &className ) const
{
	if( className == "CSplashWnd" )
		return (void*) ((CSplashWnd*)this);
	else
		return MTopWnd::getInterface( className );
}