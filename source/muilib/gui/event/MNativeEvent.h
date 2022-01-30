#ifndef __MNativeEvent
#define __MNativeEvent

#include "IUiEvent.h"
#include <framework/MTypes.h>
#include <framework/MNativeTypes.h>

/** 
 * a wrapper arround a native window event
 */
class MNativeEvent :
	public IUiEvent
{
protected:

	HWND ivHwnd;
	UINT ivMessage;
	LPARAM ivLParam;
	WPARAM ivWParam;

	/** the return code that should be returned by the window proc,
	 * set by the message receiver */
	LRESULT ivReturnCode;

public:

	MNativeEvent( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );
	virtual ~MNativeEvent();

	virtual UINT getMessage() const;
	virtual WPARAM getWParam() const;
	virtual LPARAM getLParam() const;
	virtual HWND getHwnd() const;

	virtual const IUiEvent::MEventType getEventType() const;
	virtual String toString() const;

	virtual void setReturnCode( LRESULT returnCode );
	virtual LRESULT getReturnCode() const;
};

#endif