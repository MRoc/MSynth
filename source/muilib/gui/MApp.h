#ifndef __MApp
#define __MApp

#include <framework/MTypes.h>
#include <framework/thread/IRunnable.h>
#include "MUiEventDispatcher.h"
#include <stack>

/**
 * forward declaration because of include problems
 */
class MTopWnd;
class MAppImpl;

/**
 * a win32 singleton app object
 */
class MApp
{
protected:

	static unsigned int gvMsgCount;

	/** the singleotn instance */
	static MApp* gvInstance;

	/** the native implementation */
	MAppImpl* ivImpl;

	/** the main wnd */
	MTopWnd* ivPtMainWnd;

	/** application's full path and filename */
	String ivParams;

	/** the app version */
	String ivVersion;

	/** a stack of modal windows */
	std::stack<MTopWnd*> ivModalStack;

	/** required for triggering events */
	MUiEventDispatcher ivDispatcher;

protected:

	/** constructor */
	MApp();

	/** destructor */
	virtual ~MApp();

public:

	/** returns the singleton instance */
	static MApp* getInstance();

	/** terminates the singleton instance */
	static void release();

	virtual MUiEventDispatcher* getDisptacher();

	/** sets the main window */
	virtual void setMainWnd( MTopWnd* pWnd );

	/** returns the main window */
	virtual MTopWnd* getMainWnd();

	/** sets the quit flag on true */
	virtual void setQuit();

	/** sets the application parameters */
	virtual void setParams( String file );

	/** invoked when app is coming up*/
	virtual bool onInit();

	/** invoked when app is exiting */
	virtual bool onExit();

	/** returns the application's name */
	virtual String getAppName() = 0;

	/** the code behind pToRun will be invoked from ui thread */
	virtual void doWorkInGuiThread( IRunnable* pToRun );

	/** starts a modal loop for the given window */
	virtual int startModal( MTopWnd* pWnd );

	/** stops a modal loop for the given window, returncode will be returned by modalWork */
	virtual void stopModal( MTopWnd* pWnd, int returncode );

	/** the message loop, returns the exit code */
	virtual int work();

	/** the message loop for modal stack, the return code is the code  */
	virtual int modalWork();

	/** returns the top modal window or null */
	virtual MTopWnd* getModal();

	/** returns the native app implementation */
	virtual MAppImpl* getImpl();

protected:

	static void registerUiClasses();
};
#endif