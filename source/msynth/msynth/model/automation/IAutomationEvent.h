#ifndef __IAUTOMATIONEVENT
#define __IAUTOMATIONEVENT

#include <framework/treedocument/ISerializeable.h>

/**
 * super interface of a automation event.
 * this events follow the creator and consumer
 * pattern. only the creator and consumer
 * know the concrete subclass type of the event.
 */
class IAutomationEvent
{
public:

	/**
	 * deletes this event
	 */
	virtual void release() = 0;

	/**
	 * returns a copy of this event
	 */
	virtual IAutomationEvent* clone() = 0;

	/**
	 * saves the automationevent
	 */
	virtual MTreeNode* save( unsigned int index ) = 0;

	/**
	 * loads the automationevent
	 */
	virtual void load( MTreeNode* node ) = 0;
};

#endif