#include "MTimeController.h"

MTimeController* MTimeController::gvSharedInstance = 0;

MTimeController* MTimeController::getInstance()
{
	if( gvSharedInstance == 0 )
		gvSharedInstance = new MTimeController();
	return gvSharedInstance;
}

void MTimeController::release()
{
	if( gvSharedInstance != 0 )
	{
		delete gvSharedInstance;
		gvSharedInstance = 0;
	}
}

MTimeController::MTimeController() :
	ivCurrentPattern( 0 ),
	ivCurrentSystemStep( 0 ),
	ivNextPattern( 1 ),
	ivNextSystemStep( 1 ),
	ivLeftMarker( 0 ),
	ivRightMarker( 3 )
{
}

MTimeController::~MTimeController()
{
	ivCurrentPattern = 0;
	ivCurrentSystemStep = 0;

	ivMarkerListeners.clear();
	ivSystemListeners.clear();
}

bool MTimeController::calculateNextSteps()
{
#ifdef __THREAD_SAFE
	MSynchronize cls( &ivCriticalSection );
	if( ivCriticalSection.Lock() )
	{
#endif

	bool patternChanged = false;
	int patternCount = getRightMarker() + 1;
	ivCurrentSystemStep++;
	ivNextSystemStep++;

	if( ivCurrentSystemStep >= 32 )
	{
		ivCurrentSystemStep = 0;
		ivCurrentPattern++;
		if( ivCurrentPattern >= patternCount )
			ivCurrentPattern = ivLeftMarker;

		patternChanged = true;
	}

	updateNext();

	if( patternChanged )
	{
		sync( false );
	}

	/*TRACE( "pat: %i step: %i nextPat: %i nextStep: %i\n",
		ivCurrentPattern,
		ivCurrentSystemStep,
		ivNextPattern,
		ivNextSystemStep );*/

#ifdef __THREAD_SAFE
	}
	else
		TRACE(
			"<error msg=\"could not lock\" file=\"%s\" line=\"%i\"/>\n",
			__FILE__,
			__LINE__ );
#endif

	return patternChanged;
}

void MTimeController::updateNext()
{
	ivNextPattern = ivCurrentPattern;
	ivNextSystemStep = ivCurrentSystemStep + 1;
	if( ivNextSystemStep >= 32 )
	{
		ivNextSystemStep = 0;
		ivNextPattern++;
		if( ivNextPattern >= ivRightMarker + 1 )
			ivNextPattern = ivLeftMarker;
	}
}

void MTimeController::setCurrentPatternEx( int index )
{
	systemStepChanged( 0 );
	setCurrentSystemStep( 0 );
	systemPatternChanged( index );
	setCurrentPattern( index );
	updateNext();
}

//-----------------------------------------------------------------------------
// + ADD MARKER LISTENER
//-----------------------------------------------------------------------------
void MTimeController::addMarkerListener( IMarkerListener* ptListener )
{
	ivMarkerListeners.push_back( ptListener );
}

//-----------------------------------------------------------------------------
// + REMOVE MARKER LISTENER
//-----------------------------------------------------------------------------
void MTimeController::removeMarkerListener( IMarkerListener* ptListener )
{
	for( unsigned int i=0;i<ivMarkerListeners.size();i++ )
		if( ivMarkerListeners[ i ] == ptListener )
		{
			ivMarkerListeners.erase( ivMarkerListeners.begin() + i );
			break;
		}
}

//-----------------------------------------------------------------------------
// # FIRE MARKER CHANGED
//-----------------------------------------------------------------------------
void MTimeController::fireMarkerChanged()
{
	//TRACE( "<MTimeController::fireMarkerChanged() left: %i right: %i>\n", ivLeftMarker, ivRightMarker );
	for( unsigned int i=0;i<ivMarkerListeners.size();i++ )
		ivMarkerListeners[i]->markerChanged();
}

//-----------------------------------------------------------------------------
// + ADD SYSTEM LISTENER
// low performant system listeners ( e.g. the sequencer )
//-----------------------------------------------------------------------------
void MTimeController::addSystemListener( ISystemListener* ptSystemListener )
{
#ifdef __THREAD_SAFE
	MSynchronize cls( &ivCriticalSection );
	if( ivCriticalSection.Lock() )
	{
#endif
	ivSystemListeners.push_back( ptSystemListener );
#ifdef __THREAD_SAFE
	}
	else
		TRACE(
			"<error msg=\"could not lock\" file=\"%s\" line=\"%i\"/>\n",
			__FILE__,
			__LINE__ );
#endif
}

//-----------------------------------------------------------------------------
// + REMOVE SYSTEM LISTENER
// low performant system listeners ( e.g. the sequencer )
//-----------------------------------------------------------------------------
void MTimeController::removeSystemListener( ISystemListener* ptSystemListener )
{
#ifdef __THREAD_SAFE
	MSynchronize cls( &ivCriticalSection );
	if( ivCriticalSection.Lock() )
	{
#endif
	int count = ivSystemListeners.size();
	for( int i=0;i<count;i++ )
	{
		if( ivSystemListeners[i] == ptSystemListener )
		{
			ivSystemListeners.erase( ivSystemListeners.begin() + i );
			break;
		}
	}
#ifdef __THREAD_SAFE
	}
	else
		TRACE(
			"<error msg=\"could not lock\" file=\"%s\" line=\"%i\"/>\n",
			__FILE__,
			__LINE__ );
#endif
}

//-----------------------------------------------------------------------------
// + FIRE SYSTEM STEP CHANGED
// The inputs of the gui
//-----------------------------------------------------------------------------
void MTimeController::fireSystemStepChanged()
{
#ifdef __THREAD_SAFE
	MSynchronize cls( &ivCriticalSection );
	if( ivCriticalSection.Lock() )
	{
#endif
	int count = ivSystemListeners.size();
	for( int i=0;i<count;i++ )
	{
		((ISystemListener*)ivSystemListeners[i])->systemStepChanged( ivCurrentSystemStep );
	}
#ifdef __THREAD_SAFE
	}
	else
		TRACE(
			"<error msg=\"could not lock\" file=\"%s\" line=\"%i\"/>\n",
			__FILE__,
			__LINE__ );
#endif
}

//-----------------------------------------------------------------------------
// + FIRE SYSTEM PATTERN CHANGED
// The inputs of the gui
//-----------------------------------------------------------------------------
void MTimeController::fireSystemPatternChanged()
{
#ifdef __THREAD_SAFE
	MSynchronize cls( &ivCriticalSection );
	if( ivCriticalSection.Lock() )
	{
#endif
	int count = ivSystemListeners.size();
	for( int i=0;i<count;i++ )
	{
		((ISystemListener*)ivSystemListeners[i])->systemPatternChanged( ivCurrentPattern );
	}
#ifdef __THREAD_SAFE
	}
	else
		TRACE(
			"<error msg=\"could not lock\" file=\"%s\" line=\"%i\"/>\n",
			__FILE__,
			__LINE__ );
#endif
}

//-----------------------------------------------------------------------------
// + SET CURRENT PATTERN
// Sets the current pattern,
// send a notification to all registered listeners
//-----------------------------------------------------------------------------
void MTimeController::setCurrentPattern( int pattern )
{
	ivCurrentPattern = pattern;
	fireSystemPatternChanged();
	sync( true );
}

//-----------------------------------------------------------------------------
// + GET CURRENT PATTERN
//-----------------------------------------------------------------------------
int MTimeController::getCurrentPattern()
{
	return ivCurrentPattern;
}

//-----------------------------------------------------------------------------
// + SET CURRENT SYSTEM STEP
// Sets the current systemstep ( future 16/32/64 ??? ),
// send a notification to all registered listeners
//-----------------------------------------------------------------------------
void MTimeController::setCurrentSystemStep( int currentSystemStep )
{
	if( currentSystemStep != ivCurrentSystemStep )
	{
		ivCurrentSystemStep = currentSystemStep;
		fireSystemStepChanged();
	}
}

//-----------------------------------------------------------------------------
// + GET CURRENT SYSTEM STEP
//-----------------------------------------------------------------------------
int MTimeController::getCurrentSystemStep()
{
	return ivCurrentSystemStep;
}

//-----------------------------------------------------------------------------
// + SYSTEM PATTERN CHANGED
// The inputs of the sequencer
//-----------------------------------------------------------------------------
void MTimeController::systemPatternChanged( int currentPattern )
{
	if( ivCurrentPattern != currentPattern )
	{
		ivCurrentPattern = currentPattern;
		sync( false );
	}
}


void MTimeController::setNextPattern( int nextPattern )
{
	ivNextPattern = nextPattern;
}

int MTimeController::getNextPattern()
{
	return ivNextPattern;
}

void MTimeController::setNextSystemStep( int sysStep )
{
	ivNextSystemStep = sysStep;
}

int MTimeController::getNextSystemStep()
{
	return ivNextSystemStep;
}

//-----------------------------------------------------------------------------
// + SYSTEM STEP CHANGED
// The inputs of the sequencer
//-----------------------------------------------------------------------------
void MTimeController::systemStepChanged( int currentStep )
{
	if( ivCurrentSystemStep != currentStep )
	{
		ivCurrentSystemStep = currentStep;
	}
}

//-----------------------------------------------------------------------------
// + SET LEFT MARKER
// The left loop marker
//-----------------------------------------------------------------------------
void MTimeController::setLeftMarker( int leftMarker )
{
	if( ivLeftMarker != leftMarker )
	{
		ivLeftMarker = leftMarker;
		fireMarkerChanged();
	}
}

//-----------------------------------------------------------------------------
// + GET LEFT MARKER
// The left loop marker
//-----------------------------------------------------------------------------
int	MTimeController::getLeftMarker()
{
	return ivLeftMarker;
}

//-----------------------------------------------------------------------------
// + SET RIGHT MARKER
// The right loop marker
//-----------------------------------------------------------------------------
void MTimeController::setRightMarker( int rightMarker )
{
	if( ivRightMarker != rightMarker )
	{
		ivRightMarker = rightMarker;
		fireMarkerChanged();
	}
}

//-----------------------------------------------------------------------------
// + GET RIGHT MARKER
// The right loop marker
//-----------------------------------------------------------------------------
int MTimeController::getRightMarker()
{
	return ivRightMarker;
}

void MTimeController::sync( bool fireUpdate )
{
	unsigned int count = ivSyncListeners.size();
	for( unsigned int i=0;i<count;i++ )
		ivSyncListeners[i]->sync( fireUpdate );
}

void MTimeController::addSyncListener( ISyncControl* ptSync )
{
	ivSyncListeners.push_back( ptSync );
}

void MTimeController::removeSyncListener( ISyncControl* ptSync )
{
	unsigned int count = ivSyncListeners.size();
	for( unsigned int i=0;i<count;i++ )
		if( ivSyncListeners[ i ] == ptSync )
		{
			ivSyncListeners.erase( ivSyncListeners.begin() + i );
			break;
		}
}

void MTimeController::getRealtimePosition( int& pattern, int& step )
{
	pattern = this->getCurrentPattern();
	step = this->getCurrentSystemStep();
}