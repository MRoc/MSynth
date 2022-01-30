#ifndef __MProgressDialog
#define __MProgressDialog

#include <framework/MTypes.h>
#include <framework/thread/IProgress.h>
#include <gui/dialog/MDialog.h>
#include <gui/progress/MProgressControl.h>
#include "../../../resource.h"
#include "../../model/MRenderThread.h"

class MProgressDialog :
	public MDialog
{
protected:

	MProgressControl* ivPtProgress;
	MRenderThread* ivPtThread;

public:

	MProgressDialog( MRenderThread* ptRenderThread );
	virtual ~MProgressDialog();

protected:

	virtual void onInit();
	virtual DlgResult onDlgCommand( unsigned int id );
	virtual void onDestroy();
};

#endif