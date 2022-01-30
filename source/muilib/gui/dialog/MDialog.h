#ifndef __MDialog
#define __MDialog

#include "../framewnd/MFrameWnd.h"
#include <stack>

/**
 * this class is a awfull wrapper arround the
 * shitty win32 api modal dialog system.
 */
class MDialog :
	public MFrameWnd
{
public:

	/** the return code returned by doModal */
	enum DlgResult
	{
		DLG_ERROR = -1,
		CANCEL = 0,
		OK = 1
	};

public:

	/** constructor accepting a dialog template resource id */
	MDialog();

	/** destructor */
	virtual ~MDialog();

	/** shows the modal dialog */
	virtual DlgResult doModal();

protected:

	/** process a system event */
	virtual bool processEvent( IUiEvent* pEvent );

	/** invoked when the dialog is created */
	virtual void onInit();

	/** invoked wen the dialog is destroyed */
	virtual void onClose();

	/**
	 * this function terminates the dialog.
	 *
	 * can only invoked when doModal was invoked
	 * before. after invocation of close, doModal
	 * returns the the return code provided to close.
	 */
	virtual void close( DlgResult returnCode );
};

#endif