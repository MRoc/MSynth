#ifndef __MINSTRUMENT
#define __MINSTRUMENT

#include <framework/MTypes.h>
#include <framework/map/MProperties.h>
#include <framework/listener/MListenerList.h>

#include "IProcessor.h"
#include "../MAbstractInstrumentListener.h"
#include "../note/MDefaultNoteList.h"
#include "../mixer/MDefaultMixerChannel.h"
#include "../MEventNote.h"

/**
 * base implementation of a instrument
 */
class MInstrument :
	public MDefaultMixerChannel,
	public ISerializeable
{
public:

	/**
	 * runtime support
	 */
	static MRtti gvRtti;

protected:

	/**
	 * list storing the IInstrumentListeners
	 */
	MListenerList ivInstrumentListeners;

	/**
	 * reference to the automation tracker
	 */
	IAutomationTracker* ivPtTracker;

	/**
	 * properties that are saved with the instrument,
	 * editors can store their preferences
	 */
	MProperties* ivPtProperties;

public:

	MInstrument();
	virtual ~MInstrument();

	/** 
	 * returns the number of input channels, 1=mono 2=stereo
	 * (inheritanced from IMixerChannel)
	 */
	virtual unsigned int getChannelCount() = 0;

	virtual void	registerTargets( IAutomationTracker* ptTracker );

	virtual int		getNoteListId() = 0;
	virtual int		getEditorId() = 0;

	virtual void	addInstrumentListener( IInstrumentListener* ptListener );
	virtual void	removeInstrumentListener( IInstrumentListener* ptListener );

	virtual bool	noteListNeedsListener() = 0;
	virtual int		getBitmapId( int state ) = 0;

	virtual void	setAutomationTracker( IAutomationTracker* ptTracker );

	MProperties*	getProperties();

	virtual String	getType();

	/**
	 * returns the class name for runtime purpose
	 */
	virtual IRtti* getRtti() const;

	/**
	 * queries for a specified interface
	 */
	virtual void* getInterface( const String &className ) const;

protected:

	virtual void fireStateChanged( MInstrumentEvent anEvent );
};

#endif