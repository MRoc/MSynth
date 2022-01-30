/*(C)2000 MRoc hifiShock*/

#ifndef __CSplashWnd
#define __CSplashWnd

#include <framework/MTypes.h>
#include <gui/MGuiUtils.h>
#include <gui/timer/MTimer.h>
#include <gui/MTopWnd.h>

/**
 * a slpash window
 */
class CSplashWnd :
	public MTopWnd,
	public IActionListener
{
protected:

	/** runtime type info */
	static MRtti gvRtti;	

	/** the singleton instance */
	static CSplashWnd* gvInstance;

	/** the image in splash */
	MImage ivImage;

	/** the timer used for splash screen */
	MTimer ivTimer;

	/** the text to draw */
	String ivText;

public:

	/** returns the singleton instance */
	static CSplashWnd* getInstance();

	/** releases the singleton instance */
	static void release();

	/** constructor */
	CSplashWnd();

	/** destructor */
	virtual ~CSplashWnd();

	/** creates thw window */
	virtual bool create();

	/** starts the timer and sets visible */
	virtual void start();

	/** paints the window */
	virtual void paint( IGraphics* ptGraphics, const MRect &rect );

	/** the timer callback */
	virtual void onActionPerformed( void* pSrc );

	/** returns the runtime type info */
	virtual IRtti* getRtti() const;

	/** query interface */
	virtual void* getInterface( const String &className ) const;
};

#endif