/*(C)2000 MRoc hifiShock*/

#ifndef __MBeatBoxDialog
#define __MBeatBoxDialog

#include <framework/MTypes.h>
#include <gui/dialog/MDialog.h>
#include "../../../resource.h"

class MBeatBoxDialog :
	public MDialog
{
protected:

	unsigned int ivChannelCount;

public:

	MBeatBoxDialog();

	void setCurrentChannelCount( unsigned int channelCount );
	unsigned int getCurrentChannelCount();

protected:

	virtual void onInit();
	virtual void onDestroy();
	virtual DlgResult onDlgCommand( unsigned int id );
};

#endif