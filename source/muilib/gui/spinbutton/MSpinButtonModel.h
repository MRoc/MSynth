#ifndef __MSpinButtonModel
#define __MSpinButtonModel

#include <framework/MTypes.h>
#include <framework/listener/MListenerList.h>

class MSpinButtonModel;

class ISpinButtonModelListener :
	public IListener
{
public:
	virtual void valueChanged( MSpinButtonModel* pModel ) = 0;
};

class MSpinButtonModel
{
protected:

	int ivMin, ivMax, ivValue;

	MListenerList ivListeners;

public:

	MSpinButtonModel(void);
	virtual ~MSpinButtonModel(void);

	virtual int getValue();
	virtual void setValue( int value );

	virtual int getMin();
	virtual void setMin( int min );

	virtual int getMax();
	virtual void setMax( int max );

	void addListener( ISpinButtonModelListener* ptListener );
	void removeListener( ISpinButtonModelListener* ptListener );

protected:

	void fireActionPerformed();
};

#endif