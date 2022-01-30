/*
	
	MFloatControl (C)2001 MRoc hifiShock

	A descriptor that encapsulates a float and has a middle for normalized values

	e.g. the pitch for 2 octaves goes from 0.5 to 2.0:

		frequence * 0.5 = frequence one octave lower

		frequence * 1.0 = the original frequence

		frequence * 2.0 = frequence one octave higher

	1.0 as middle, the original octave, so we have 2 ranges,
	the first from 0.5 to 1.0, the second from 1.0 up to 2.0,
	but as normalized value, the middle seems to be 0.5.

*/

#ifndef __MRangeControl
#define __MRangeControl

#include "MFloatControl.h"

class MRangeControl :
	public MFloatControl
{
protected:

	float ivMiddle;

public:

	MRangeControl( String name,
		float minValue, float middleValue, float maxValue, float defaultValue ) : 
		MFloatControl( name, minValue, maxValue, defaultValue, true )
	{
			ivMiddle = middleValue;
	}

	virtual ~MRangeControl()
	{
	}

	virtual void setNormalizedValue( float value )
	{
		ASSERT( value >= 0.0f && value <= 1.0f );

		if( value >= 0.5f )
			setValue( ivMiddle + 2 * (value - 0.5f) * getHighRange() );
		else
			setValue( ivMinValue + 2 * value * getLowRange() );
	}

	virtual float getNormalizedValue()
	{
		float back;
		if( getValue() >= ivMiddle )
			back = ( getValue() - ivMiddle ) / ( 2 * getHighRange() ) + 0.5f;
		else
			back = ( getValue() - getMinValue() ) / ( 2 * getLowRange() );
		ASSERT( back >= 0.0 && back <= 1.0 );
		return back;
	}

	virtual MTreeNode* save()
	{
		MTreeNode* ptBack = new MTreeNode( "control" );
		ptBack->setAttribute( "name", MDefaultControl::getName() );
		ptBack->setAttribute( "value", MFloat::toString( getNormalizedValue() ) );
		ptBack->setAttribute( "id", MInt::toString( ((int)this) ) );
		return ptBack;
	};

	virtual void load( MTreeNode* node )
	{
		String nodeValue = node->getAttribute( "value" );
		if( nodeValue != "" )
		{
			setNormalizedValue(
				MFloat::parse(
					nodeValue,
					0.0f,
					1.0f,
					0.5f ) );
		}
		else
		{
			setValue( getDefaultValue() );
		}

		int id =
			MInt::parse( node->getAttribute( "id" ), -1 );
		if( id != -1 )
		{
			MTargetRegistry::getInstance()->getIdRegistry()->registerTarget(
				this,
				id );
		}
	};

protected:

	float getLowRange()
	{
		return ivMiddle - ivMinValue;
	}

	float getHighRange()
	{
		return ivMaxValue - ivMiddle;
	}
};


#endif
