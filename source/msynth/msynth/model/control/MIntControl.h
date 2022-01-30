/*
	
	MIntControl (C)2001 MRoc hifiShock

	A descriptor that encapsulates a int

*/
#ifndef __MIntControl
#define __MIntControl

#include "MDefaultControl.h"

class MIntControl :
	public MDefaultControl
{
public:

	static MRtti gvRtti;

protected:

	int			ivValue,
				ivMinValue,
				ivMaxValue,
				ivDefaultValue;

	char*		ivEinheit;

public:

	MIntControl( String name, int minValue, int maxValue, int defaultValue );

	virtual ~MIntControl();
	int	getMinValue();
	int	getMaxValue();
	int	getRange();
	int	getValue();
	void setValue( int value );
	int getDefaultValue();
	const char* getEinheit();

protected:

	void setMinValue( int minValue );
	void setMaxValue( int maxValue );
	void setDefaultValue( int defaultValue );
	void setEinheit( char* einheit );

public:

	virtual MTreeNode* save();

	virtual void load( MTreeNode* node );

	virtual String toString();

	virtual IRtti* getRtti() const;

};

#endif
