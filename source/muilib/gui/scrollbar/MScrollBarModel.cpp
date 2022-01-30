#include "MScrollBarModel.h"

MScrollBarModel::MScrollBarModel() :
	ivValue( 0 ),
	ivMin( 0 ),
	ivMax( 0 ),
	ivDif( 10 ),
	ivInner( 0 )
{
}

MScrollBarModel::~MScrollBarModel()
{
	ivListeners.clear();
}

void MScrollBarModel::setData( int min, int inner, int max )
{
	ivMin = min;
	ivInner = inner;
	ivMax = max;
	if( ivInner > ivMax )
		ivInner = ivMax;
	clipValue();
	fireActionPerformed();
}

void MScrollBarModel::setInner( int inner )
{
	if( inner != ivInner )
	{
		ivInner = inner;
		clipValue();
		fireActionPerformed();
	}
}

int MScrollBarModel::getInner()
{
	return ivInner;
}

void MScrollBarModel::inc()
{
	addValue( ivDif );
}

void MScrollBarModel::dec()
{
	subValue( ivDif );
}

void MScrollBarModel::incPage()
{
	addValue( ivInner );
}

void MScrollBarModel::decPage()
{
	subValue( ivInner );
}

void MScrollBarModel::setValue( int value )
{
	if( value != ivValue )
	{
		ivValue = value;
		clipValue();
		fireActionPerformed();
	}
}

void MScrollBarModel::clipValue()
{
	if( ivValue > ivMax - ivInner )
	{
		//TRACE( "CLIP1\n" );
		ivValue = ivMax - ivInner;
	}
	if( ivValue < ivMin )
	{
		//TRACE( "CLIP2\n" );
		ivValue = ivMin;
	}
}

void MScrollBarModel::addValue( int value )
{
	setValue( ivValue + value );
}

void MScrollBarModel::subValue( int value )
{
	setValue( ivValue - value );
}

void MScrollBarModel::setNormalizedValue( float f )
{
	if( f > 1.0f )
		f = 1.0f;
	else if( f < 0.0f )
		f = 0.0f;
    int newValue = getMin() + (int)(((float)(getRange()-getInner()))*f);
	setValue( newValue );
}

float MScrollBarModel::getNormalizedValue()
{
	return ((float)( getValue() - getMin() )) / ((float)getRange());
}

int MScrollBarModel::getValue()
{
	return ivValue;
}

int MScrollBarModel::getMin()
{
	return ivMin;
}

void MScrollBarModel::setMin( int min )
{
	if( ivMin != min )
	{
		ivMin = min;
		clipValue();
		fireActionPerformed();
	}
}

int MScrollBarModel::getMax()
{
	return ivMax;
}

void MScrollBarModel::setMax( int max )
{
	if( max != ivMax )
	{
		ivMax = max;
		clipValue();
		fireActionPerformed();
	}
}

void MScrollBarModel::setDif( int dif )
{
	ivDif = dif;
	fireActionPerformed();
}

int MScrollBarModel::getDif()
{
	return ivDif;
}

int MScrollBarModel::getRange()
{
	return ivMax - ivMin;
}

bool MScrollBarModel::isActive()
{
	return ivInner != getRange();
}

float MScrollBarModel::getInnerToRangeRatio()
{
	return ((float)getInner()) / ((float)getRange());
}

void MScrollBarModel::addActionListener( IScrollBarListener* ptListener )
{
	ivListeners.addListener( ptListener );
}

void MScrollBarModel::removeActionListener( IScrollBarListener* ptListener )
{
	ivListeners.removeListener( ptListener );
}

void MScrollBarModel::fireActionPerformed()
{
	unsigned int count = ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IScrollBarListener*)ivListeners.getListener( i ))->valueChanged( this );
}