#ifndef __MScrollBarModel
#define __MScrollBarModel

#include <framework/listener/MListenerList.h>
#include "IScrollBarListener.h"

class MScrollBarModel
{
protected:

	int ivValue;
	int ivMin;
	int ivMax;
	int ivDif;
	int ivInner;

	MListenerList ivListeners;

public:

	MScrollBarModel(void);
	virtual ~MScrollBarModel(void);

	void setValue( int value );
	int getValue();

	void inc();
	void dec();

	void incPage();
	void decPage();

	void addValue( int value );
	void subValue( int value );

	void setNormalizedValue( float f );
	float getNormalizedValue();
	float getInnerToRangeRatio();

	void setData( int min, int inner, int max );

	void setInner( int inner );
	int getInner();

	int getMin();
	void setMin( int min );

	int getMax();
	void setMax( int max );

	void setDif( int dif );
	int getDif();

	int getRange();
	bool isActive();

	void addActionListener( IScrollBarListener* ptListener );
	void removeActionListener( IScrollBarListener* ptListener );

protected:

	void clipValue();
	void fireActionPerformed();
};

#endif