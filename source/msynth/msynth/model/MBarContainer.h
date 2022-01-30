/*

	MBarContainer (C)2001 MRoc hifiShock

	A MBarContainer contains some MBars (chained list)

*/


#ifndef __MBARCONTAINER
#define __MBARCONTAINER

#include <framework/MUtil.h>
#include <framework/treedocument/ISerializeable.h>

#include "MBar.h"
#include "Synth.h"
#include "MBeatBox.h"

#include "interfaces/MInstrument.h"
#include "audiochannel/MAudioChannel.h"
#include "audiochannel/MAudioTriggerList.h"
#include "list/MList.h"
#include "note/MNoteFactory.h"

/**
 * the MBarContainer is a list of bars and references an instrument.
 * if the instrument's notelists require a information about notification
 * from the instrument, the barcontainer is registered as a
 * IInstrumentListener and forwards all events to the notelists
 */
class MBarContainer :
	public MList,
	public IInstrumentListener,
	public IAutomationTracker,
	public ITargetRegistryListener,
	public IFinalizeListener
{
protected:

	MInstrument*	ivPtInstrument;
	unsigned int	ivMaxBarCount;
	unsigned int	ivRow;
	MCriticalSection ivSync;
	
public:

	FULL_RTTI_SUPER( MBarContainer, "MBarContainer", MList );

	MBarContainer();
	virtual ~MBarContainer();

	virtual MBar*			getBarStartingAt( int barNo );
	virtual MBar*			getBarOverlappingAt( int index );
	virtual MDefaultNote*	getNoteStartsAt( int barStartsAt, int noteStartsAt );
	virtual MDefaultNote*	getNoteStopsAt( int barStartsAt, int noteStartsAt );
	
	virtual void			setInstrument( MInstrument* ptInstrument );
	virtual MInstrument*	getInstrument();

	virtual void			setMaxBarCount( unsigned int maxBarCount );
	virtual unsigned int	getMaxBarCount();
	virtual unsigned int	getMinBarCount();

	virtual unsigned int	getBarCount();

	virtual bool			canInsertBar( MBar* ptBar, int insertIndex );

	virtual MBar*			createDefaultBar();

	virtual int				getMaxPossibleLength( MBar* ptBar );

	virtual void			stateChanged( MInstrumentEvent anEvent );
	virtual String			getInstrumentListenerName();

	virtual void			setRow( unsigned int index );
	virtual unsigned int	getRow();

	virtual void	insertAt( MListItem *item );
	virtual MListItem* getItemAt( int index );
	virtual bool	isFree( unsigned int index );

	/**
	 * loads from the given tree node
	 */
	virtual void load( MTreeNode* ptNode, IBpmControl* ptControl );

	/**
	 * stores as tree node
	 */
	virtual MTreeNode* save();

	// inserts the automation event in the event list at the given position
	virtual void addAutomationEvent(
		IAutomationEvent* ptEvent,
		IEventTarget* ptTarget,
		unsigned int pattern,
		unsigned int systemStep );

	// removes the given automation event
	virtual void removeAutomationEvent( IAutomationEvent* ptEvent );

	// removes all automation events for this control
	virtual void removeAutomationEvents( IEventTarget* ptTarget );

	virtual void triggerAutomationEvents( unsigned int pattern, unsigned int step );

	virtual void automationAdded( IEventTarget* ptControl );
	virtual void automationRemoved( IEventTarget* ptControl );

	virtual bool hasTarget( IEventTarget* ptTarget );
	virtual void collectTargets( std::vector<IEventTarget*> &collector );

	/**
	 * called if the object is deleted, after recievieing this message,
	 * the object MUST deregister itself as listener because the observed
	 * object will be deleted, if not there it will result in an endless loop!!!
	 **/
	virtual void objectTerminated( void* obj );

protected:

	virtual bool hasTarget(
		IControlCollection* ptCollection,
		IEventTarget* ptTarget );

	virtual void collectTargets(
		IControlCollection* ptCollection,
		std::vector<IEventTarget*> &collector );

	virtual bool isOverlapping( int start1, int end1, int start2, int end2 );
};

#endif