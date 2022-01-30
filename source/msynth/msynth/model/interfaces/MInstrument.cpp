/*

	MInstrument (C)2001 MRoc hifiShock

*/

#include "MInstrument.h"

/**
 * runtime support
 */
MRtti MInstrument::gvRtti = MRtti( "MInstrument", NULL );

//-----------------------------------------------------------------------------
// + CONSTRUCTION
//-----------------------------------------------------------------------------
MInstrument::MInstrument() :
	MDefaultMixerChannel(),
	ivPtTracker( 0 )
{
	ivPtProperties = new MProperties();
}

//-----------------------------------------------------------------------------
// + DESTRUCTION
//-----------------------------------------------------------------------------
MInstrument::~MInstrument()
{
	if( ivInstrumentListeners.getListenerCount() > 0 )
	{
		TRACE(
			"ERROR: MInstrument::~MInstrument() left with %i listeners (IInstrumentListener)!!!\n",
			ivInstrumentListeners.getListenerCount() );
		for( unsigned int i=0;i<ivInstrumentListeners.getListenerCount();i++ )
		{
			TRACE( "\tlistener %s\n", ((IInstrumentListener*)ivInstrumentListeners.getListener( i ) )->getInstrumentListenerName() );
		}
		ivInstrumentListeners.clear();
	}
	SAFE_DELETE( ivPtProperties );
}

String MInstrument::getType()
{
	return "source";
}

//-----------------------------------------------------------------------------
// + addInstrumentListener
//-----------------------------------------------------------------------------
void MInstrument::addInstrumentListener( IInstrumentListener* ptListener )
{
	ivInstrumentListeners.addListener( ptListener );
}

//-----------------------------------------------------------------------------
// + removeInstrumentListener
//-----------------------------------------------------------------------------
void MInstrument::removeInstrumentListener( IInstrumentListener* ptListener )
{
	ivInstrumentListeners.removeListener( ptListener );
}

//-----------------------------------------------------------------------------
// # fireStateChanged
//-----------------------------------------------------------------------------
void MInstrument::fireStateChanged( MInstrumentEvent anEvent )
{
	//TRACE( "<minstrument::firestatechanged>\n" );
	unsigned int count = ivInstrumentListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
	{
		IInstrumentListener* ptListener = ((IInstrumentListener*)ivInstrumentListeners.getListener(i));
		ptListener->stateChanged( anEvent );
		//TRACE( "\t<listener name=\"%s\"/>\n", ptListener->getInstrumentListenerName() );
	}
	//TRACE( "</minstrument::firestatechanged>\n" );
}

void MInstrument::setAutomationTracker( IAutomationTracker* ptTracker )
{
	ASSERT( ptTracker != ivPtTracker );
	ivPtTracker = ptTracker;
	//registerTargets( ptTracker );

	unsigned int count = getControlCount();
	MTargetRegistry* ptReg = MTargetRegistry::getInstance();
	for( unsigned int i=0;i<count;i++ )
	{
		IEventTarget* pTarget = (IEventTarget*) getControl( i )->getInterface( "IEventTarget" );
		if( pTarget )
			ptReg->registerContext( new MAutomationContext( pTarget, ptTracker ) );
	}
}

void MInstrument::registerTargets( IAutomationTracker* ptTracker )
{
	unsigned int count = getControlCount();
	MTargetRegistry* ptReg = MTargetRegistry::getInstance();
	for( unsigned int i=0;i<count;i++ )
	{
		IEventTarget* pTarget = (IEventTarget*) getControl( i )->getInterface( "IEventTarget" );
		if( pTarget )
			ptReg->registerContext( new MAutomationContext( pTarget, ptTracker ) );
	}

	count = getChildCount();
	for( i=0;i<count;i++ )
		if( getChild( i ) )
			getChild( i )->registerTargets( ptTracker );
}

MProperties* MInstrument::getProperties()
{
	return ivPtProperties;
}

/**
 * returns the class name for runtime purpose
 */
IRtti* MInstrument::getRtti() const
{
	return &gvRtti;
}

/**
 * querries for a specified interface
 */
void* MInstrument::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MInstrument*)this);
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else if( className == "IProcessor" )
		return (void*) ((IProcessor*)this);
	else if( className == "IControlCollection" )
		return (void*) ((IControlCollection*)this);
	else
		return MDefaultMixerChannel::getInterface( className );
}