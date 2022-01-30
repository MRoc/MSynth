#include "MLfo.h"

INIT_RTTI( LFO, "INIT_RTTI" );

LFO::LFO()
{
	ivPtOnControl = new MBoolControl( String( "Lfo" ), false );
	ivPtSyncControl = new MBoolControl( String( "Sync" ), true );
	ivPtFreqControl = new MFloatControl( String("Frequency"), 0.0f, 30.0f, 1.0f, false, MFloatControl::CURVE_LINEAR, String( "Hz" ), 0.5f );
	ivPtFreqControl->addControlListener( this );
	ivPtWidthControl = new MFloatControl( String("Bandwidth"), 0.0f, 1.0f, 0.5f, false );
	ivPtPhaseControl = new MFloatControl( String( "Phase" ), 0.0f, 1.0f, 0.0f, true, MFloatControl::CURVE_LINEAR, "", 1.0f );
	ivPtPhaseControl->addControlListener( this );

	reset();
}

LFO::~LFO()
{
	ivPtFreqControl->removeControlListener( this );
	ivPtPhaseControl->removeControlListener( this );
	SAFE_DELETE( ivPtSyncControl );
	SAFE_DELETE( ivPtFreqControl );
	SAFE_DELETE( ivPtOnControl );
	SAFE_DELETE( ivPtWidthControl );
	SAFE_DELETE( ivPtPhaseControl );
}

String LFO::toString()
{
	return getName();
}

IControlCollection* LFO::getChild( unsigned int index )
{
	return 0;
}

unsigned int LFO::getChildCount()
{
	return 0;
}

unsigned int LFO::getControlCount()
{
	return 5;
}

IControl* LFO::getControl( unsigned int index )
{
	ASSERT( index < getControlCount() );

	switch( index )
	{
		case LFOON: return ivPtOnControl; break;
		case FREQUENCY: return ivPtFreqControl; break;
		case BANDWIDTH: return ivPtWidthControl; break;
		case SYNC: return ivPtSyncControl; break;
		case PHASE: return ivPtPhaseControl; break;
		default: return 0; break;
	}
}

bool LFO::getLfoState()
{
	return ivPtOnControl->getValue();
}

void LFO::setLfoState( bool value )
{
	ivPtOnControl->setValue( value );
}

void LFO::setLfoBandWidth( FP bandWidth )
{
	ivPtWidthControl->setValue( bandWidth );
}

FP LFO::getLfoBandWidth()
{
	return ivPtWidthControl->getValue();
}

void LFO::setLfoFrequence( FP frequence )
{
	ivPtFreqControl->setValue( frequence );
}

FP LFO::getLfoFrequence()
{
	return ivPtFreqControl->getValue();
}

void LFO::reset()
{
	if( this->ivPtSyncControl->getValue() )
	{
		valueChanged( 
			&MControlListenerEvent(
				ivPtFreqControl,
				MControlListenerEvent::CONTROL_EVENT_VALUE_CHANGED ) );

		valueChanged(
			&MControlListenerEvent(
				ivPtPhaseControl,
				MControlListenerEvent::CONTROL_EVENT_VALUE_CHANGED  ) );
	}
}

void LFO::goNext( FP* ptData, unsigned int startAt, unsigned int stopAt )
{
	FP *ptStop = ptData + stopAt;
	FP temp = 0;
	FP bandWidth = getLfoBandWidth();
	for( FP* pt=ptData+startAt;pt<ptStop;pt++ )
	{
		double xTmp = ivCurrentSin, yTmp = ivCurrentCos;
		ivCurrentSin = ivCos*xTmp + ivSin*yTmp;
		ivCurrentCos = -ivSin*xTmp + ivCos*yTmp;

		(*pt) *= (float) ( 1 + (ivCurrentSin+1) * bandWidth );
	}
}

void LFO::valueChanged( MControlListenerEvent *anEvent )
{
	if( anEvent->getSource() == ivPtFreqControl )
	{
		double freq	= ivPtFreqControl->getValue();
		double tmp = freq * 2 * PI / MW_SAMPLINGRATE;
		ivSin = sin( tmp );
		ivCos = cos( tmp );
	}
	else if( anEvent->getSource() == ivPtPhaseControl )
	{
		double phase = ivPtPhaseControl->getValue();
		double tmp = phase * 2 * PI;
		ivCurrentSin = sin( tmp );
		ivCurrentCos = cos( tmp );
	}
}

String LFO::getType()
{
	return String("individualprocessor" );
}

String LFO::getName()
{
	return String( "Lfo" );
}

void LFO::registerTargets( IAutomationTracker* tracker )
{
	unsigned int count = getControlCount();
	for( unsigned int i=0;i<count;i++ )
	{
		IEventTarget* pTarget = (IEventTarget*) getControl( i )->getInterface( "IEventTarget" );
		if( pTarget )
			MTargetRegistry::getInstance()->registerContext(
				new MAutomationContext( pTarget, tracker ) );
	}
}

IControl* LFO::getControlByName( String name )
{
	IControl* back = 0;
	unsigned int count = getControlCount();
	for( unsigned int i=0;i<count;i++ )
		if( getControl( i ) != 0 &&
			getControl( i )->getName() == name )
			back = getControl( i );
	return back;
}