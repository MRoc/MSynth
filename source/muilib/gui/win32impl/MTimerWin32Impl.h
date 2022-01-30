#ifndef __MTimerWin32Impl
#define __MTimerWin32Impl

#include <framework/MTypes.h>
#include <framework/MNativeTypes.h>
#include <framework/listener/MActionListenerList.h>
#include "../timer/MTimerImpl.h"

class MTimerWin32Impl;
typedef std::map<UINT_PTR, MTimerWin32Impl*> Handle2TimerMap;
typedef Handle2TimerMap::value_type Handle2TimerMapValue;
typedef Handle2TimerMap::iterator Handle2TimerMapIter;

class MTimerWin32Impl :
	public MTimerImpl
{
protected:

	/** the timer map */
	static Handle2TimerMap gvTimerMap;

	/** the win32 timer object */
	UINT_PTR ivWin32TimerHandle;

	/** the connected listeners */
	MActionListenerList ivListeners;

	/** the number of cycles since last start */
	unsigned int ivTimerCycles;

public:

	/** constructor */
	MTimerWin32Impl();

	/** destructor */
	virtual ~MTimerWin32Impl();

	/** starts the timer */
	virtual void startTimer( int millis );

	/** stops the timer */
	virtual void stopTimer();

	/** returns true if timer is running */
	virtual bool isRunnning();

	/** adds a timer listener */
	virtual void addListener( IActionListener* pListener );

	/** removes a timer listener */
	virtual void removeListener( IActionListener* pListener );

	/** returns the number of cycles the timer was called back since last start */
	virtual unsigned int getTimerCycles() const;

private:

	/** the virtual timer callback */
	virtual void onTimer();

	/** the static win32 timer callback */
	static VOID CALLBACK
		TimerProc(
			HWND hwnd,         // handle to window
			UINT uMsg,         // WM_TIMER message
			UINT_PTR idEvent,  // timer identifier
			DWORD dwTime       // current system time
		);
};

#endif