#ifndef __MAUTOMATIONEVENTLIST
#define __MAUTOMATIONEVENTLIST

#include <framework/MTypes.h>
#include <framework/xml/MXmlUtil.h>

#include "IAutomationEventList.h"
#include "ITargetRegistry.h"
#include "../control/MFloatControl.h"

class MAutomationEventList :
	public IAutomationEventList
{
protected:

	unsigned int ivLength;
	IAutomationEvent** ivData;

	IEventTarget* ivPtTarget;

	// the seralized id if the event target,
	// needed between the calls of load and resolveIds
	int ivSerId;

public:

	MAutomationEventList();
	MAutomationEventList( unsigned int length );
	MAutomationEventList( unsigned int length, IEventTarget* ptTarget );
	MAutomationEventList( MAutomationEventList* toClone );
	MAutomationEventList(
		unsigned int length,
		IAutomationEvent** data,
		IEventTarget* ptTarget );

	virtual ~MAutomationEventList();

	virtual void setTarget( IEventTarget* ptTarget );
	virtual IEventTarget* getTarget();

	// updates the length of a automation event list, if the new length
	// is less than ther old one, all events not used anymore are released,
	virtual void setLength( unsigned int newLength );

	virtual unsigned int getLength();

	// rescales the data field, if scaled down, events can be released
	virtual void scale( unsigned int newLength );

	virtual void setItem( IAutomationEvent* ptEvent, unsigned int index );
	virtual IAutomationEvent* getItem( unsigned int index );
	virtual unsigned int getItemCount();
	virtual void removeItem( unsigned int index );
	virtual void removeItem( IAutomationEvent* ptEvent );

	virtual void clear();

	virtual void glow( IAutomationEventList* ptEventList );
	virtual IAutomationEventList* cut( unsigned int index );

	virtual void release();

	virtual IAutomationEventList* clone();

	virtual MTreeNode* save();
	virtual void load( MTreeNode* ptNode );

protected:

	void init();

	// returns the index of the given event, -1 if not found
	int getIndex( IAutomationEvent* ptEvent );
};
#endif