#ifndef __IControlCollection
#define __IControlCollection

#include "IControl.h"
#include "../automation/IAutomationTracker.h"

class IControlCollection
{
public:

	virtual String toString() = 0;
	virtual String getName() = 0;
	virtual IControlCollection* getChild( unsigned int index ) = 0;
	virtual unsigned int getChildCount() = 0;
	virtual String getType() = 0;
	virtual unsigned int getControlCount() = 0;
	virtual IControl* getControl( unsigned int index ) = 0;
	virtual IControl* getControlByName( String name ) = 0;
	virtual void registerTargets( IAutomationTracker* tracker ) = 0;
};

#endif