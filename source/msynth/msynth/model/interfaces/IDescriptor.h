/*(C)2000 MRoc hifiShock*/

#ifndef __IDescriptor
#define __IDescriptor

#include <framework/MTypes.h>

#include "../control/IControlCollection.h"

class IDescriptor : public IControlCollection
{
public:

	virtual String getShortName() = 0;
	virtual int getViewId() = 0;

	virtual String getType() = 0;
	virtual String getName() = 0;

	virtual unsigned int getControlCount() = 0;
	virtual IControl* getControl( unsigned int index ) = 0;

	virtual void registerTargets( IAutomationTracker* tracker ) = 0;
};

#endif