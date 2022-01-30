#ifndef __MEvent
#define __MEvent

#include <framework/MObject.h>
#include <framework/primitive/MInt.h>
#include <framework/treedocument/ISerializeable.h>

/**
 * defines a event for the IProcessor interface
 */
class MEvent :
	public MObject,
	public ISerializeable
{
public:

	/**
	 * the runtime type info
	 */
	static MRtti gvRtti;

	/**
	 * types of events
	 */
	enum Type
	{
		NOTE_ON,
		NOTE_OFF,
		RESET,
		PORTAMENTO,
		FALLDOWN
	};

protected:

	/**
	 * the event type
	 */
	Type ivType;

public:

	/**
	 * constructor
	 */
	MEvent( Type t );

	/**
	 * destructor
	 */
	virtual ~MEvent();

	/**
	 * returns the type of the event
	 */
	virtual Type getType();

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * returns a pointer to the specified interface if available
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * saves the event
	 */
	virtual MTreeNode* save();

	/**
	 * loads the event
	 */
	virtual void load( MTreeNode* ptNode );

	/**
	 * returns a string representation of the note event
	 */
	virtual String toString();
};

#endif