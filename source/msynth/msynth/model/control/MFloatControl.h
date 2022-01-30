/*
	
	MFloatControl (C)2001 MRoc hifiShock

	A descriptor that encapsulates a float

*/

#ifndef __MFloatControl
#define __MFloatControl

#include <math.h>

#include "MDefaultControl.h"

#include "../automation/IAutomationEvent.h"
#include <framework/primitive/MFloat.h>
#include <framework/primitive/MInt.h>
#include <framework/io/MLogger.h>
#include "../automation/MTargetRegistry.h"

class MFloatEvent :
	public IAutomationEvent
{
protected:

	FP ivValue;

public:

	MFloatEvent();
	MFloatEvent( FP value );
	MFloatEvent( MFloatEvent* ptEvent );
	virtual ~MFloatEvent();

	// deletes this event
	virtual void release();
	virtual IAutomationEvent* clone();
	FP getValue();
	void setValue( FP value );
	virtual MTreeNode* save( unsigned int index );
	virtual void load( MTreeNode* node );
	static IAutomationEvent* createInstance();
};

class MFloatControl :
	public MDefaultControl,
	public IEventTarget
{
public:

	enum CurveType
	{
		CURVE_LINEAR,
		CURVE_QUAD
	};

	static MRtti gvRtti;

protected:

	MListenerList	ivAutomationListeners;
	AutomationState	ivAutomationState;

	float			ivValue,
					ivMinValue,
					ivMaxValue,
					ivDefaultValue;

	CurveType		ivCurveType;

	String			ivEinheit;
	float			ivDisplayMulti;

	bool			ivShowNormalizedValue;

public:

	MFloatControl( String name, float minValue, float maxValue,
		float defaultValue, bool showNormalizedValue );
	MFloatControl( String name, float minValue, float maxValue,
		float defaultValue, bool showNormalizedValue, CurveType curveType );
	MFloatControl( String name, float minValue, float maxValue,
		float defaultValue, bool showNormalizedValue, CurveType curveType,
		String einheit, float displayMulti );
	virtual ~MFloatControl();

	virtual void* getInterface( const String &className ) const;
	virtual IRtti* getRtti() const;

	virtual float getMinValue();
	virtual float getMaxValue();
	virtual float getRange();
	virtual float getValue();
	virtual void setValue( float value );

	virtual void setNormalizedValue( float value );
	virtual float getNormalizedValue();
	virtual CurveType getCurveType();
	virtual float getShowValue();
	virtual float getDefaultValue();
	virtual String getEinheit();
	virtual String toDisplayString();
	virtual unsigned int getViewID();
	virtual void resetToDefault();

	virtual String getName();

	virtual IAutomationEvent* createEvent();

protected:

	virtual void setMinValue( float minValue );
	virtual void setMaxValue( float maxValue );
	virtual void setDefaultValue( float defaultValue );
	virtual void setEinheit( String einheit );
	virtual void setShowNormalizedValue( bool value );
	virtual void setNormalizedValue( float value, bool fireEvent );

public: // ISerializeable

	virtual MTreeNode* save();
	virtual void load( MTreeNode* node );

public: // IEventTarget

	/**
	 * returns a string representation of the target
	 */
	virtual String toString();

	/**
	 * the input side, processes an automation event
	 */
	virtual void postEvent( IAutomationEvent* ptEvent );

	/** 
	 * sets the automation state
	 */
	virtual void setAutomationState( AutomationState value );

	/** 
	 * returns the automation state
	 */
	virtual AutomationState getAutomationState();

	/** 
	 * adds a finalize listener
	 */
	virtual void addListener( IEventTargetListener* ptListener );

	/** 
	 * removes a finalize listener
	 */
	virtual void removeListener( IEventTargetListener* ptListener );
};

#endif

