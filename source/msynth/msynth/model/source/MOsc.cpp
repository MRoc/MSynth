#include "MOsc.h"

MOsc::MOsc() :
	ivFreq( 0.0f ),
	ivOctaveMultiplier( 0.0 ),
	ivPortamentoLength( 0 ),
	ivPortamentoStep( 0 ),
	ivPortamentoTargetFrequence( 0.0f ),
	ivIsPortamento( false )
{
	ivPtOctaveControl = new MIntControl( "Octave", 1, 8, 4 );
	ivPtVolumeControl = new MFloatControl( "Volume", 0.0f, 1.0f, 1.0f, true );
	ivPtTuneControl = new MRangeControl( "Tune", 0.5f, 1.0f, 2.0f, 1.0f );
	ivPtPhaseControl = new MFloatControl( "Phase", 0.0f, 1.0f, 0.0f, true );

	ivPtOctaveControl->addControlListener( this );
	ivPtTuneControl->addControlListener( this );
			
	int value = ivPtOctaveControl->getValue();
	ivOctaveMultiplier = 0.0625;
	for( int i=0;i<value;i++ )
		ivOctaveMultiplier *= 2;

	reset();
}

MOsc::~MOsc()
{
	ivPtOctaveControl->removeControlListener( this );
	ivPtTuneControl->removeControlListener( this );
	
	SAFE_DELETE( ivPtOctaveControl );
	SAFE_DELETE( ivPtVolumeControl );
	SAFE_DELETE( ivPtTuneControl );
	SAFE_DELETE( ivPtPhaseControl );
}

/**
 * fire a event into the processor
 */
void MOsc::processEvent( MEvent* ptEvent )
{
	switch( ptEvent->getType() )
	{
	case MEvent::RESET:
		reset();
		break;
	case MEvent::NOTE_ON:
		{
			MEventNoteOn* ptNoteOn = (MEventNoteOn*) ptEvent;
			setFreq( ptNoteOn->getFrequency() );
		}
		break;
	case MEvent::PORTAMENTO:
		{
			MEventPortamento* ptPortamentoOn = (MEventPortamento*) ptEvent;
			this->triggerPortamento( ptPortamentoOn->getFrequency(), ptPortamentoOn->getPortamentoLength() );
		}
		break;
	}
}

void MOsc::setFreq( FP freq )
{
	this->ivFreq = freq;
}

void MOsc::addPitch( FP delta )
{
	this->ivFreq += delta;
}

unsigned int MOsc::getControlCount()
{
	return MOSC_CONTROL_COUNT;
}

IControl* MOsc::getControl( unsigned int index )
{
	IControl* back = 0;
	switch( index )
	{
		case OCTAVE: back = ivPtOctaveControl; break;
		case VOLUME: back = ivPtVolumeControl; break;
		case TUNE: back = ivPtTuneControl; break;
		case PHASE: back = ivPtPhaseControl; break;
	};
	return back;
}

void MOsc::valueChanged( MControlListenerEvent *anEvent )
{
	IControl* pSrc = anEvent->getSource();
	if( pSrc == this->ivPtOctaveControl )
		updateOctaveMultiplier();
	else if( pSrc == this->ivPtTuneControl )
		setFreq( getFreq() );
}

void MOsc::reset()
{
	stopPortamento();
}

void MOsc::triggerPortamento( FP targetFrequence, unsigned int portamentoLength )
{
	ivPortamentoLength = portamentoLength;
	ivPortamentoTargetFrequence = targetFrequence;
	ivPortamentoStep = 0;
	ivIsPortamento = true;
}

void MOsc::stopPortamento()
{
	ivPortamentoLength = 0;
	ivPortamentoStep = 0;
	ivPortamentoTargetFrequence = 0;
	ivIsPortamento = false;
}

bool MOsc::getPortamento()
{
	return ivIsPortamento;
}

void MOsc::updateOctaveMultiplier()
{
	int value = ivPtOctaveControl->getValue();
	ivOctaveMultiplier = 0.0625;
	for( int i=0;i<value;i++ )
		ivOctaveMultiplier *= 2;

	setFreq( ivFreq );
}

FP MOsc::getFreq()
{
	return ivFreq;
}

String	MOsc::getType()
{
	return String( "sourceprocessor" );
}
		
int MOsc::getViewId()
{
	return DESC_VIEW_NORMAL;
}

MTreeNode* MOsc::save()	
{
	MTreeNode* ptBack = new MTreeNode( getType() );
	ptBack->setAttribute( "value", getRtti()->getClassName() );

	unsigned int count = getControlCount();
	for( unsigned int i=0;i<count;i++)
		if( getControl( i ) )
			ptBack->addChild( getControl( i )->save() );

	return ptBack;
}

void MOsc::load( MTreeNode* node )
{
	MTreeNodeIterator iter = node->getIterator();
	while( iter.hasNext() )
	{
		MTreeNode* controlNode = iter.nextTreeNode();
		String nodeName = controlNode->getName();
		if( strcmp( nodeName, "control" ) == 0 )
		{
			String controlName = controlNode->getAttribute( "name" );
			IControl* ptControl = getControlByName( controlName );
			if( ptControl )
				ptControl->load( controlNode );
		}
	}
}

void MOsc::registerTargets( IAutomationTracker* tracker )
{
	unsigned int count = getControlCount();
	for( unsigned int i=0;i<count;i++ )
	{
		if( getControl( i ) )
		{
			IEventTarget* pTarget = (IEventTarget*) getControl( i )->getInterface( "IEventTarget" );
			if( pTarget )
				MTargetRegistry::getInstance()->registerContext(
					new MAutomationContext( pTarget, tracker ) );
		}
	}
}

IControl* MOsc::getControlByName( String name )
{
	IControl* back = 0;
	unsigned int count = getControlCount();
	for( unsigned int i=0;i<count;i++ )
		if( getControl( i ) != 0 &&
			getControl( i )->getName() == name )
			back = getControl( i );
	return back;
}

String MOsc::toString()
{
	return getName();
}

void* MOsc::getInterface( const String &className ) const
{
	if( className == "MOsc" )
		return (void*) ((MOsc*)this);
	else if( className == "IProcessor" )
		return (void*) ((IProcessor*)this);
	else if( className == "IDescriptor" )
		return (void*) ((IDescriptor*)this);
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else if( className == "IControlListener" )
		return (void*) ((IControlListener*)this);
	else
		return MObject::getInterface( className );
}

IControlCollection* MOsc::getChild( unsigned int index )
{
	ASSERT( true );
	return 0;
}

unsigned int MOsc::getChildCount()
{
	return 0;
}