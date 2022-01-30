/**
 * (C)2000-2003 MRoc
 */

#include "MTransformer.h"

MRtti MTransformer::gvRtti = MRtti( "MTransformer", 0 );

MTransformer::MTransformer() :
	ivChannelCount( 0 )
{
}

MTransformer::~MTransformer()
{
}

bool MTransformer::init( MTransformerInitParam* params )
{
	return true;
}

bool MTransformer::create( unsigned int channelCount )
{
	this->ivChannelCount = channelCount;
	return true;
}

/**
 * fire a event into the processor
 */
void MTransformer::processEvent( MEvent* ptEvent )
{
	/*switch( ptEvent->getType() )
	{
	case MEvent::FALLDOWN:
		this->triggerFallDown();
		break;
	case MEvent::NOTE_ON:
		this->triggerNoteOn();
		break;
	case MEvent::NOTE_OFF:
		this->triggerNoteOff();
		break;
	case MEvent::RESET:
		reset();
		break;
	}*/
}

/*void MTransformer::reset()
{
}

void MTransformer::triggerNoteOn()
{
}

void MTransformer::triggerNoteOff()
{
}

void MTransformer::triggerFallDown()
{
}*/

int MTransformer::getViewId()
{
	return DESC_VIEW_NORMAL;
}

String MTransformer::getType()
{
	return String("processor");
}

MTreeNode* MTransformer::save()
{
	// create transformer node
	MTreeNode* ptBack = new MTreeNode( getType() );

	// store class name
	ptBack->setAttribute( "value", getRtti()->getClassName() );

	// store channelcount
	ptBack->setAttribute( "channelcount", MInt::toString( ivChannelCount ) );

	// control tags
	unsigned int count = getControlCount();
	for( unsigned int i=0;i<count;i++ )
		ptBack->addChild( getControl( i )->save() );

	return ptBack;
}

void MTransformer::load( MTreeNode* parentNode )
{
	// 1. fetch channel count from node
	this->ivChannelCount = MInt::parse( parentNode->getAttribute( "channelcount" ), 1 );

	// 2. create effect with specified channelcount
	this->create( ivChannelCount );

	// 3. init all params
	MTreeNodeIterator iter = parentNode->getIterator();
	while( iter.hasNext() )
	{
		MTreeNode* controlNode = iter.nextTreeNode();
		String nodeName = controlNode->getName();
		if( strcmp( nodeName, "control" ) == 0 )
		{
			String controlName = controlNode->getAttribute( "name" );
			IControl* pControl = getControlByName( controlName );
			if( pControl )
				pControl->load( controlNode );
			else
				MLogger::logWarning(
				"<mtransformer::load msg=\"could not resolve control\" control=\"%s\" transformer=\"%s\"/>\n",
				controlName.getData(),
				getName().getData()
				);
		}
	}
}

void MTransformer::registerTargets( IAutomationTracker* tracker )
{
	unsigned int count = getControlCount();
	for( unsigned int i=0;i<count;i++ )
		if( getControl( i ) )
		{
			IEventTarget* pTarget = (IEventTarget*)getControl( i )->getInterface( "IEventTarget" );
			if( pTarget )
			{
				IAutomationContext* context = new MAutomationContext( pTarget, tracker );
				MTargetRegistry::getInstance()->registerContext( context );
			}
		}
		else
			MLogger::logError( "MTransformer::registerTargets: getControl(%i) failed", i );
}

IControl* MTransformer::getControlByName( String name )
{
	IControl* back = 0;
	unsigned int count = getControlCount();
	for( unsigned int i=0;i<count;i++ )
		if( getControl( i ) != 0 &&
			getControl( i )->getName() == name )
			back = getControl( i );
	return back;
}

String MTransformer::toString()
{
	return getShortName();
}

IEventTarget* MTransformer::getTarget( unsigned int index )
{
	return (IEventTarget*) getControl( index )->getInterface("IEventTarget");
}

unsigned int MTransformer::getTargetCount()
{
	return getControlCount();
}

IControlCollection* MTransformer::getChild( unsigned int index )
{
	ASSERT( false ); // ???? 2.8.02 ???? <- was true
	return 0;
}

unsigned int MTransformer::getChildCount()
{
	return 0;
}

void* MTransformer::getInterface( const String &className ) const
{
	if( className == "MTransformer" )
		return (void*) ((MTransformer*)this);
	if( className == "IProcessor" )
		return (void*) ((IProcessor*)this);
	if( className == "IDescriptor" )
		return (void*) ((IDescriptor*)this);
	if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else
		return MObject::getInterface( className );
}

IRtti* MTransformer::getRtti() const
{
	return &gvRtti;
}