#include "MTargetRegistry.h"
#include <framework/io/MLogger.h>

/** 
 * the singleton instance
 */
MTargetRegistry* MTargetRegistry::gvPtInstance = 0;

/**
 * constructor
 */
MTargetRegistry::MTargetRegistry() :
	ivRecordMode( DISCARD_EVENTS )
{
	MLogger::logMessage( "<mtargetregistry::mtargetregistry/>\n" );
}

/** 
 * destructor
 */
MTargetRegistry::~MTargetRegistry()
{
	if( ivContextMap.size() != 0 )
		MLogger::logError(
			"MTargetRegistry::~MTargetRegistry: ivContextMap.size() = %i", ivContextMap.size() );

	if( ivAutomationMap.size() != 0 )
		MLogger::logError(
			"MTargetRegistry::~MTargetRegistry: ivAutomationMap.size() = %i", ivAutomationMap.size() );

	if( ivListeners.getListenerCount() != 0 )
		MLogger::logError(
		"MTargetRegistry::~MTargetRegistry: ivListeners.size() = %i", ivListeners.getListenerCount() );

	ivContextMap.clear();
	ivIdRegistry.clear();
	ivAutomationMap.clear();
}

/**
 * returns the singleton instance
 */
MTargetRegistry* MTargetRegistry::getInstance()
{
	if( gvPtInstance == 0 )
		gvPtInstance = new MTargetRegistry();
	return gvPtInstance;
}

/**
 * releases the singleton instance
 */
void MTargetRegistry::release()
{
	if( gvPtInstance )
	{
		delete gvPtInstance;
		gvPtInstance = 0;
	}
}

/**
 * registers a automation context for a target
 * inheritanced from ITargetRegistry
 */
void MTargetRegistry::registerContext( IAutomationContext* ptContext )
{
	ASSERT( ivContextMap.find( ptContext->getTarget() ) == ivContextMap.end() );
	ivContextMap[ ptContext->getTarget() ] = ptContext;
}

/**
 * unregisters a automation context for a target
 * inheritanced from ITargetRegistry
 */
void MTargetRegistry::unregisterContext( IEventTarget* ptTarget )
{
	if( ivContextMap.find( ptTarget ) != ivContextMap.end() )
	{
		if( isInAutomation( ptTarget ) )
			removeFromAutomation( ptTarget );
		ContextMapIter i = ivContextMap.find( ptTarget );
		IAutomationContext* ptContext = i->second;
		ivContextMap.erase( i );
		ptContext->release();
	}
}

/** 
 * query registry for the automation context of the given target
 * inheritanced from ITargetRegistry
 */
IAutomationContext* MTargetRegistry::getContext( IEventTarget* ptTarget )
{
	IAutomationContext* back = 0;

	ContextMapIter i = ivContextMap.find( ptTarget );
	if( i != ivContextMap.end() )
		back = i->second; 

	return back;
}

/**
 * sets the global play mode (what should be done with recorded events)
 */
void MTargetRegistry::setRecordMode( MTargetRegistry::RecordMode mode )
{
	ivRecordMode = mode;
}

/**
 * returns the global play mode (what should be done with recorded events)
 */
MTargetRegistry::RecordMode MTargetRegistry::getRecordMode()
{
	return ivRecordMode;
}

/** 
 * returns the id map used for serialisation
 */
ITargetIdRegistry* MTargetRegistry::getIdRegistry()
{
	return &ivIdRegistry;
}

/**
 * adds a control to automation
 * inheritanced from ITargetRegistry
 */
void MTargetRegistry::addToAutomation( IEventTarget* ptControl )
{
	ASSERT( ivAutomationMap.find( ptControl ) == ivAutomationMap.end() );
	ivAutomationMap[ ptControl ] = ptControl;
	fireAutomationAdded( ptControl );
	ptControl->setAutomationState( IEventTarget::ADDED_TO_AUTOMATION );
}

/** 
 * removes a control from automation
 * inheritanced from ITargetRegistry
 */
void MTargetRegistry::removeFromAutomation( IEventTarget* ptControl )
{
	ASSERT( ivAutomationMap.find( ptControl ) != ivAutomationMap.end() );
	removeAllEvents( ptControl );
	ivAutomationMap.erase( ivAutomationMap.find( ptControl ) );
	fireAutomationRemoved( ptControl );
	ptControl->setAutomationState( IEventTarget::REMOVED_FROM_AUTOMATION );
}

/** 
 * returns true if the given target is in automation
 * inheritanced from ITargetRegistry
 */
bool MTargetRegistry::isInAutomation( IEventTarget* ptControl )
{
	return ivAutomationMap.find( ptControl ) != ivAutomationMap.end();
}

/** 
 * removes all events stored in automation for the given target
 * inheritanced from ITargetRegistry
 */
void MTargetRegistry::removeAllEvents( IEventTarget* ptTarget )
{
	TRACE( "<MTargetRegistry::onRemoveAllEvents()>\n" );
	IAutomationContext* ptContext = getContext( ptTarget );
	if( ptContext )
	{
		IAutomationTracker* ptTracker = ptContext->getTracker();
		ASSERT( ptTracker );
		ptTracker->removeAutomationEvents( ptTarget );
	}
}

/**
 * adds a registry listener to be informed about changed
 * on the automation targets.
 * inheritanced from ITargetRegistry
 */
void MTargetRegistry::addListener( ITargetRegistryListener* ptListener )
{
	ivListeners.addListener( ptListener );
}

/**
 * removes a registry listener to be informed about changed
 * on the automation targets.
 * inheritanced from ITargetRegistry
 */
void MTargetRegistry::removeListener( ITargetRegistryListener* ptListener )
{
	ivListeners.removeListener( ptListener );
}

/**
 * fires a automation added event
 */
void MTargetRegistry::fireAutomationAdded( IEventTarget* ptControl )
{
	unsigned int count = ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((ITargetRegistryListener*)ivListeners.getListener(i))->automationAdded( ptControl );
}

/**
 * fires a automation removed event
 */
void MTargetRegistry::fireAutomationRemoved( IEventTarget* ptControl )
{
	unsigned int count = ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((ITargetRegistryListener*)ivListeners.getListener(i))->automationRemoved( ptControl );
}

#ifdef _DEBUG
/**
 * dumps the registry content
 */
String MTargetRegistry::getContent()
{
	String buffer("");
	String back = "<mtargetregistry>\r\n";

	back += "\t<registry size=\"";
	buffer.Format( "%i", ivContextMap.size() );
	back += buffer;
	back += "\">\r\n";
	for( ContextMapIter iter=ivContextMap.begin();
		iter!=ivContextMap.end();iter++ )
	{
		back += "\t\t<target name=\"";
		back += (*iter).second->getTarget()->getName();
		back += "\"/>\r\n";
	}
	back += "\t</registry>\r\n";

	back += "\t<automation size=\"";
	buffer.Format( "%i", ivAutomationMap.size() );
	back += buffer;
	back += "\">\r\n";
	for( AutomationMap::iterator i=ivAutomationMap.begin();
		i!=ivAutomationMap.end();i++ )
	{
		back += "\t\t<target name=\"";
		back += (*i).first->getName();
		back += "\"/>\r\n";
	}
	back += "\t</automation>\r\n";

	back += "</mtargetregistry>\r\n";
			
	return back;
}
#endif