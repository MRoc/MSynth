#include "MFloatControl.h"

MRtti MFloatControl::gvRtti = MRtti( "MFloatControl", 0 );


MFloatEvent::MFloatEvent() :
	ivValue( 0.0f )
{
}

MFloatEvent::MFloatEvent( FP value ) :
	ivValue( value )
{
}

MFloatEvent::MFloatEvent( MFloatEvent* ptEvent ) :
	ivValue( ptEvent->getValue() )
{
}

MFloatEvent::~MFloatEvent()
{

}

void MFloatEvent::release()
{
	delete this;
}

IAutomationEvent* MFloatEvent::clone()
{
	return new MFloatEvent( this );
};

FP MFloatEvent::getValue()
{
	return ivValue;
}

void MFloatEvent::setValue( FP value )
{
	ASSERT( value >= 0.0f && value <= 1.0f );
	ivValue = value;
}

MTreeNode* MFloatEvent::save( unsigned int index )
{
	MTreeNode* ptBack = new MTreeNode( "automationevent" );
	ptBack->setAttribute( "class", "MFloatEvent" );
	ptBack->setAttribute( "value", MFloat::toString( ivValue ) );
	ptBack->setAttribute( "index", MInt::toString( (int)index ) );
	return ptBack;
}

void MFloatEvent::load( MTreeNode* node )
{
	ivValue =
		MFloat::parse(
			node->getAttribute( "value" ),
			0.0f,
			1.0f,
			0.5f );
}

IAutomationEvent* MFloatEvent::createInstance()
{
	return new MFloatEvent();
}

/////////////////////////////////////////////////////////////////////////////////////////

MFloatControl::MFloatControl( String name, float minValue, float maxValue,
	float defaultValue, bool showNormalizedValue )
	: MDefaultControl( name )
{
	ivCurveType = CURVE_LINEAR;
	setMinValue( minValue );
	setMaxValue( maxValue );
	setDefaultValue( defaultValue );
	setValue( defaultValue );
	setShowNormalizedValue( showNormalizedValue );
}

MFloatControl::MFloatControl( String name, float minValue, float maxValue,
	float defaultValue, bool showNormalizedValue, CurveType curveType )
	: MDefaultControl( name )
{
	ivCurveType = curveType;
	setMinValue( minValue );
	setMaxValue( maxValue );
	setDefaultValue( defaultValue );
	setValue( defaultValue );
	setShowNormalizedValue( showNormalizedValue );
}

MFloatControl::MFloatControl( String name, float minValue, float maxValue,
	float defaultValue, bool showNormalizedValue, CurveType curveType,
	String einheit, float displayMulti )
	: MDefaultControl( name )
{
	ivDisplayMulti = displayMulti;
	ivCurveType = curveType;
	setMinValue( minValue );
	setMaxValue( maxValue );
	setDefaultValue( defaultValue );
	setValue( defaultValue );
	setEinheit( einheit );
	setShowNormalizedValue( showNormalizedValue );
}

MFloatControl::~MFloatControl()
{
	ivAutomationListeners.lock();
	unsigned int count = ivAutomationListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IEventTargetListener*)ivAutomationListeners.getListener( i ))->onEventTargetChange(
			this,
			IEventTargetListener::ET_TERMINATING );
	ivAutomationListeners.unlock();

	MTargetRegistry::getInstance()->unregisterContext( this );
}

void* MFloatControl::getInterface( const String &className ) const
{
	if( className == "MFloatControl" )
		return (void*) ((MFloatControl*)this);
	else if( className == "IEventTarget" )
		return (void*) ((IEventTarget*)this);
	else
		return MDefaultControl::getInterface( className );
}

IRtti* MFloatControl::getRtti() const
{
	return &gvRtti;
}

float MFloatControl::getMinValue()
{
	return ivMinValue;
}

float MFloatControl::getMaxValue()
{
	return ivMaxValue;
}

float MFloatControl::getRange()
{
	return ivMaxValue - ivMinValue;
}

float MFloatControl::getValue()
{
	return ivValue;
}

void MFloatControl::setValue( float value )
{
	ASSERT( value >= ivMinValue && value <= ivMaxValue );
	ivValue = value;
	fireValueChanged();
}

void MFloatControl::setNormalizedValue( float value )
{
	setNormalizedValue( value, true );
	//TRACE( "[MFloatControl::setNormalizedValue...]\n" );
}

float MFloatControl::getNormalizedValue()
{
	//TRACE( "[MFloatControl::getNormalizedValue...]\n" );
	float back = ( ivValue - ivMinValue ) / getRange();
	if( ivCurveType == CURVE_QUAD )
		back = float( sqrt( back ) );
	ASSERT( back >= 0.0 && back <= 1.0 );
	return back;
}

MFloatControl::CurveType MFloatControl::getCurveType()
{
	return ivCurveType;
}

float MFloatControl::getShowValue()
{
	//TRACE( "[MFloatControl::getShowValue...]\n" );
	float back;
	if( ivShowNormalizedValue )
		back = getNormalizedValue();
	else
		back = getValue();
	return back;
}

float MFloatControl::getDefaultValue()
{
	return ivDefaultValue;
}

String MFloatControl::getEinheit()
{
	return ivEinheit;
}

void MFloatControl::setMinValue( float minValue )
{
	ivMinValue = minValue;
}

void MFloatControl::setMaxValue( float maxValue )
{
	ivMaxValue = maxValue;
}

void MFloatControl::setDefaultValue( float defaultValue )
{
	ivDefaultValue = defaultValue;
}

void MFloatControl::setEinheit( String einheit )
{
	ivEinheit = einheit;
}

void MFloatControl::setShowNormalizedValue( bool value )
{
	ivShowNormalizedValue = value;
}

void MFloatControl::setNormalizedValue( float value, bool fireEvent )
{
	ASSERT( value >= 0.0 && value <= 1.0 );
	if( ivCurveType == CURVE_QUAD )
		value *= value;
	value = ivMinValue + getRange() * value;
	if( fireEvent )
		setValue( value );
	else
		ivValue = value;
}

MTreeNode* MFloatControl::save()
{
	MTreeNode* ptBack = new MTreeNode( "control" );
	ptBack->setAttribute( "name", MDefaultControl::getName() );
	ptBack->setAttribute( "value", MFloat::toString( getValue() ) );
	ptBack->setAttribute( "id", MInt::toString( (int) ((IEventTarget*)this) ) );
	return ptBack;
}

void MFloatControl::load( MTreeNode* node )
{
	float value = 
		MFloat::parse(
			node->getAttribute( "value" ),
			getMinValue(),
			getMaxValue(),
			getDefaultValue() );
	setValue( value );
	int id =
		MInt::parse(
			node->getAttribute( "id" ),
			-1 );
	if( id != -1 )
	{
		MTargetRegistry::getInstance()->getIdRegistry()->registerTarget(
			this,
			id );
	}
}

void MFloatControl::postEvent( IAutomationEvent* ptEvent )
{
	FP value = ((MFloatEvent*)ptEvent)->getValue();
	ASSERT( value >= 0.0f && value <= 1.0f );
	setNormalizedValue( value, false );
	fireValueChanged();
}

IAutomationEvent* MFloatControl::createEvent()
{
	return new MFloatEvent( getNormalizedValue() );
}

String MFloatControl::toString()
{
	char buffer[1024];
	sprintf( buffer, "%f\0", ivValue );
	return String( buffer );
}

String MFloatControl::toDisplayString()
{
	char buffer[2048];
	if( ivEinheit.GetLength() > 0 )
		sprintf( buffer, "%s %f %s", MDefaultControl::getName().getData(), ivValue * ivDisplayMulti, ivEinheit.getData() );
	else
		sprintf( buffer, "%s %s", MDefaultControl::getName().getData(), toString().getData() );
	return String( buffer );
}

unsigned int MFloatControl::getViewID()
{
	return DESC_TYPE_SLIDER;
}

void MFloatControl::resetToDefault()
{
	setValue( getDefaultValue() );
}

/** 
 * sets the automation state
 */
void MFloatControl::setAutomationState( AutomationState value )
{
	ivAutomationState = value;
	ivAutomationListeners.lock();
	unsigned int count = ivAutomationListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IEventTargetListener*)ivAutomationListeners.getListener( i ))->onEventTargetChange(
			this,
			ivAutomationState == IEventTarget::ADDED_TO_AUTOMATION ? 
				IEventTargetListener::ET_ADDED_TO_AUTOMATION :
				IEventTargetListener::ET_REMOVED_FROM_AUTOMATION );
	ivAutomationListeners.unlock();
}

/** 
 * returns the automation state
 */
IEventTarget::AutomationState MFloatControl::getAutomationState()
{
	return ivAutomationState;
}

/** 
 * adds a finalize listener
 */
void MFloatControl::addListener( IEventTargetListener* ptListener )
{
	ivAutomationListeners.addListener( ptListener );
}

/** 
 * removes a finalize listener
 */
void MFloatControl::removeListener( IEventTargetListener* ptListener )
{
	ivAutomationListeners.removeListener( ptListener );
}

String MFloatControl::getName()
{
	return MDefaultControl::getName();
}