/*
	
	IControl (C)2001-2003 MRoc hifiShock

	A abstract descriptor is an interface that defines an descriptor.
	A descriptor has a name and a value,
	the value can be a double, int, string... and must be implemented
	in classes that implements this interface.
	The descriptor links a value of an algorythmus with an viewable control.

*/

#ifndef __ICONTROL
#define __ICONTROL

enum CONTROL_TYPE
{
	DESC_TYPE_UNDEF,
	DESC_TYPE_SLIDER,
	DESC_TYPE_BUTTON,
	DESC_TYPE_GROUP,
	DESC_TYPE_ID,
	DESC_TYPE_COMBO,
	DESC_TYPE_COUNTER
};

enum DATA_TYPE
{
	DT_UNDEF,
	DT_FLOAT,
	DT_INT,
	DT_BOOL
};

enum VIEW_TYPE
{
	DESC_VIEW_UNDEF,
	DESC_VIEW_NORMAL,
	DESC_VIEW_WAVEOSC,
	DESC_VIEW_WAVEPLAYER,
	DESC_VIEW_MIIR,
	DESC_VIEW_MULTIOSC
};

#include <framework/MTypes.h>
#include <framework/treedocument/ISerializeable.h>
#include <framework/listener/IListener.h>

class IControl;
class IControlListener;

/**
 * superclass of a event posted by a IControl.
 * the event has a flag signaling it's type.
 */
class MControlListenerEvent
{
public:

	/**
	 * the type definition of a controlevent
	 */
	enum Type
	{
		CONTROL_EVENT_UNDEF,
		CONTROL_EVENT_VALUE_CHANGED,
		CONTROL_TERMINATING
	};

protected:

	/**
	 * the type of the controlevent
	 */
	Type ivType;

	/**
	 * the source control sending this event
	 */
	IControl* ivPtSource;

public:

	/**
	 * defaultcontructor
	 */
	MControlListenerEvent() :
		ivPtSource( 0 ),
		ivType( CONTROL_EVENT_UNDEF )
	{
	}

	/**
	 * normal constructor
	 */
	MControlListenerEvent( IControl* pControl, Type type ) :
		ivPtSource( pControl ),
		ivType( type )
	{
	}

	/**
	 * destructor
	 */
	virtual ~MControlListenerEvent()
	{
	}

	/**
	 * returns the control sending this event
	 */
	virtual IControl* getSource()
	{
		return ivPtSource;
	}

	/**
	 * returns the event's type
	 */
	virtual Type getEventType()
	{
		return ivType;
	}
};

/**
 * superinterface of all control listeners
 */
class IControlListener :
	public IListener
{
public:

	/**
	 * invoked by internal state change in a control
	 */
	virtual void valueChanged( MControlListenerEvent *anEvent ) = 0;
};


/**
 * a IControl interface
 */
class IControl :
	public IMUnknown,
	public ISerializeable
{
public:

	/**
	 * returns the control's name
	 */
	virtual String getName() = 0;

	/**
	 * resets the control to default value
	 */
	virtual void resetToDefault() = 0;

	/**
	 * adds a control listener to be informed
	 * about control changes and termination
	 */
	virtual void addControlListener( IControlListener* ptListener ) = 0;

	/**
	 * removes a presious installed listener
	 */
	virtual void removeControlListener( IControlListener* ptListener ) = 0;

	/**
	 * for GUI creation
	 */
	virtual unsigned int getViewID() = 0;

	/**
	 * loads from the given tree node
	 * (inheritanced from ISerializeable)
	 */
	virtual void load( MTreeNode* ptNode ) = 0;

	/**
	 * stores as tree node
	 * (inheritanced from ISerializeable)
	 */
	virtual MTreeNode* save() = 0;
};

#endif

