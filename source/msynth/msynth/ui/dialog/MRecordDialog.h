/*(C)2000 MRoc hifiShock*/

#ifndef __MRecordDialog
#define __MRecordDialog

#include <gui/dialog/MDialog.h>
#include "MProgressDialog.h"
#include "../../../resource.h"


class MRecordDialog : public MDialog
{
public:

	enum RecordMode
	{
		RENDER,
		STREAMING
	};

protected:

	RecordMode ivMode;

public:

	MRecordDialog();
	virtual ~MRecordDialog();

	RecordMode getSelectedMode(){ return ivMode; };

protected:
	
	virtual DlgResult onDlgCommand( unsigned int id );
	virtual void onInit();
	virtual void onDestroy();
};


#endif