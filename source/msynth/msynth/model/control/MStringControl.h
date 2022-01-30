/*
	
	MStringControl (C)2001 MRoc hifiShock

	A descriptor that encapsulates a bool

*/

#ifndef __MStringControl
#define __MStringControl

#include "MDefaultControl.h"

class MStringControl :
	public MDefaultControl
{
public:

	static MRtti gvRtti;

protected:

	String ivValue;
	String ivDefaultValue;

public:

	MStringControl(
		String name,
		String defaultValue );

	virtual ~MStringControl();
	virtual String getValue();
	virtual void setValue( String value );
	virtual String getDefaultValue();
	virtual IRtti* getRtti() const;

protected:

	virtual void setDefaultValue( String defaultValue );

public:

	virtual MTreeNode* save();
	virtual void load( MTreeNode* node );
	virtual String toString();
};

#endif

