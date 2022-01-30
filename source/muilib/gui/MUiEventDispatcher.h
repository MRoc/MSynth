#ifndef __MUiEventDispatcher
#define __MUiEventDispatcher

class MTopWnd;
class IUiEvent;

/**
 * - dispatches incoming events onto the right target.
 * - mouse events are recoordinated to the right window
 * - mouse/key events can be in modal loops if required in future (TODO)
 * - use MWndManager to store ui states
 */
class MUiEventDispatcher
{
protected:

	/** counts all routed events */
	unsigned int ivMsgCount;

public:

	/** constructor */
	MUiEventDispatcher();

	/** destructor */
	virtual ~MUiEventDispatcher();

	/** triggers a event into the ui dispatcher */
	virtual void processUiEvent( MTopWnd* pTopWnd, IUiEvent* pEvent );
};

#endif