#ifndef __MDiscreteComboControl
#define __MDiscreteComboControl

#include "MDiscreteControl.h"

class MDiscreteComboControl : public MDiscreteControl
{
public:

	static MRtti gvRtti;

protected:

	StrList ivStrings;

public:

	MDiscreteComboControl( String name, IntList values, StrList strings, int defaultValue );
	MDiscreteComboControl( String name, const int values[], const char* strings[], unsigned int valueLength, int defaultValue );

	virtual ~MDiscreteComboControl();
	virtual String getStringAt( unsigned int index );
	virtual unsigned int getViewID();
	virtual IRtti* getRtti() const;
};

#endif