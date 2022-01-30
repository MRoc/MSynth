#ifndef __MBoolControl
#define __MBoolControl

#include "MDefaultControl.h"

/**
 * MBoolControl (C)2001-2003 MRoc hifiShock
 *
 * A descriptor that encapsulates a bool
 */
class MBoolControl :
	public MDefaultControl
{
public:

	/**
	 * runtime type info
	 */
	static MRtti gvRtti;

protected:

	/**
	 * the current value
	 */
	bool ivValue;

	/**
	 * the default value
	 */
	bool ivDefaultValue;

public:

	/**
	 * constructor
	 */
	MBoolControl(
		String name,
		bool defaultValue );

	/**
	 * destructor
	 */
	virtual ~MBoolControl();

	/**
	 * returns the current value
	 */
	virtual bool getValue();

	/**
	 * sets the current value
	 */
	virtual void setValue( bool value );

	/**
	 * returns the default value
	 */
	virtual bool getDefaultValue();

	/**
	 * resets the control to default value
	 */
	virtual void resetToDefault();

	/**
	 * saves the control
	 */
	virtual MTreeNode* save();

	/**
	 * loads the control
	 */
	virtual void load( MTreeNode* node );

	/**
	 * returns a string representation
	 */
	virtual String toString();

	/**
	 * query for implemented interfaces and superclasses
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * returns the view's id
	 */
	virtual unsigned int getViewID();
};

#endif

