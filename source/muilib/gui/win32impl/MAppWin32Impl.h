#ifndef __MAppWin32Impl
#define __MAppWin32Impl

#include <framework/MTypes.h>
#include <framework/MNativeTypes.h>
#include "../MAppImpl.h"
#include "../MAcceleratorTable.h"

class MAppWin32Impl :
	public MAppImpl
{
protected:

	static unsigned int gvMsgCount;

	/** the windows app handle */
	HINSTANCE ivHInstance;

	/** a acceleratortable */
	MAcceleratorTable ivAccTable;

public:

	/** constructor */
	MAppWin32Impl();

	/** destructor */
	virtual ~MAppWin32Impl();

	/** sets the quit flag on true */
	virtual void setQuit();

	/** the message loop, returns the exit code */
	virtual int work();

	/** the message loop for modal stack, the return code is the code  */
	virtual int modalWork();

	/** the code behind pToRun will be invoked from ui thread */
	virtual void doWorkInGuiThread( IRunnable* pToRun );

	/** returns the native win app handle */
	virtual HINSTANCE getHandle();

	/** set native app handle */
	virtual void setHandle( HINSTANCE handle );

	/** handling one event */
	static LRESULT staticWindowProc(
		HWND wnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam );
};

#endif