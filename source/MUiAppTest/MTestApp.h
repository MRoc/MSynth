#ifndef __MTestApp
#define __MTestApp

#include <gui\MApp.h>
#include "MTestWindow.h"

class MTestApp :
	public MApp
{
protected:

	/** the test window */
	MTestWindow* ivPtWindow;

public:

	/** constructor */
	MTestApp();

	/** destructor */
	virtual ~MTestApp();

	/** invoked when app is coming up */
	virtual bool onInit();

	/** invoked when app exists */
	virtual bool onExit();

	/** returns the application's name */
	virtual String getAppName(){ return "MTestApp"; };
};

#endif