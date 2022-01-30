#ifndef __SETTINGS
#define __SETTINGS

#include <framework/MTypes.h>
#include <framework/map/MProperties.h>
#include <gui/dialog/MDialog.h>
#include "../../../resource.h"
#include "../../model/MSequencerControl.h"
#include "../../model/streamingengine/MAudioEngine.h"

/**
 * (C)2000-2003 MRoc hifiShock
 */
class Settings : public MDialog
{
private:

	static const int		gvDefBufLen[];

public:

	Settings();
	virtual ~Settings();

	virtual void onInit();
	virtual MDialog::DlgResult onDlgCommand( unsigned int id );
	virtual void onDestroy();
};

#endif