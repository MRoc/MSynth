#include "mspinbuttonmodel.h"

MSpinButtonModel::MSpinButtonModel() :
	ivMin( 0 ),
	ivMax( 100 ),
	ivValue( 0 )
{
}

MSpinButtonModel::~MSpinButtonModel()
{
}

int MSpinButtonModel::getValue()
{
	return ivValue;
}

void MSpinButtonModel::setValue( int value )
{
	if( value != ivValue )
	{
		ivValue = value;
		if( ivValue > ivMax )
			ivValue = ivMax;
		else if( ivValue < ivMin )
			ivValue = ivMin;
		fireActionPerformed();
	}
}

int MSpinButtonModel::getMin()
{
	return ivMin;
}

void MSpinButtonModel::setMin( int min )
{
	ivMin = min;
}

int MSpinButtonModel::getMax()
{
	return ivMax;
}

void MSpinButtonModel::setMax( int max )
{
	ivMax = max;
}

void MSpinButtonModel::addListener( ISpinButtonModelListener* ptListener )
{
	ivListeners.addListener( ptListener );
}

void MSpinButtonModel::removeListener( ISpinButtonModelListener* ptListener )
{
	ivListeners.removeListener( ptListener );
}

void MSpinButtonModel::fireActionPerformed()
{
	unsigned int count = ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((ISpinButtonModelListener*)ivListeners.getListener( i ))->valueChanged( this );
}