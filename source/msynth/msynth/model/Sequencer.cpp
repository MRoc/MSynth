/*------------------------------------------------------------------------------------

	Sequencer (C)2000-2003 MRoc hifiShock

	v. 0.10			-> full
	v. 0.11			-> note-bugfix, a#1
	v. 0.12			-> FX-Divide, one for synth, one for drums
	v. 0.13			-> next-pattern bugfix and play-first-note bugfix
	v. 0.2beta		-> portamento
	v. 0.20			-> portamento-bug-fix(shuffle)
	v. 0.21			-> render-modes
	v. 0.3beta		-> more synths? never tested, event-design
	v. 0.30			-> Add and remove synths now
	v. 0.31			-> step-loops, prework for bufferd system
	v. 0.32			-> Buffer-Concept, Mixer-Concept
	v. 0.4beta		-> triggerNoteOn/off concept
	v. 0.40			-> introduced MInstrument
	v. 0.41			-> introduced MTimeController and ISystemListener
	v. 0.42			-> mixing in buffers, fx rack, fx routing
	v. 0.43			-> SYSTEM_STEP 32
	v. 0.5			-> Performance Querry
	v. 0.6			-> fixed volume bug, now 2x
	v. 0.7			-> more stream orientied, delegated mixing to MDefaultMixer
	
------------------------------------------------------------------------------------*/

#include "Sequencer.h"
#include "interfaces/MSoundBufferUtils.h"

#define CLIP( value ) {	if((value)>MW_MAX ){(value) = MW_MAX;}else if((value)< -MW_MAX){(value)=-MW_MAX;}}


const unsigned int Sequencer::SYSTEM_STEPS = 32;

/*
* Shuffle32 Scale: 1/8
* 
*   5   1   2   1   9       20   4    8    4
*   - + - + - + - = - = 1 = -- + -- + -- + --
*   9   9   9   9   9       36   36   36   36
*
*   1   1   1   1   4       9    9    9    9
*   - + - + - + - = - = 1 = -- + -- + -- + -- 
*   4   4   4   4   4       36   36   36   36
*
*
*                           11   5    1    5
*                         + -- - -- - -- - --
*                           36   36   36   36
*
*
*/
const FP Sequencer::SYSTEM_SHUFFLE[] = {
	11.0f/36.0f,
	-5.0f/36.0f,
	-1.0f/36.0f,
	-5.0f/36.0f };

/**
 * constructor
 */
Sequencer::Sequencer() :
	ivPtSong( 0 ),
	ivPtTimeController( 0 ),
	ivPtMixer( new MDefaultMixer() ),
	ivBpm( 120 ),
	ivCurStep( 0 ),
	ivStepWidth( 0 ),
	ivShuffleValue( 0.0f )
{
	MLogger::logMessage( "<sequencer::sequencer/>\n" );
	ivPtMixer->setFxRack( new MFxRack() );
}

/**
 * destructor
 */
Sequencer::~Sequencer()
{
	MLogger::logMessage( "<sequencer::~sequencer()>\n" );

	if( ivPtTimeController )
		ivPtTimeController->removeSystemListener( this );

	delete ivPtMixer->getFxRack();
	SAFE_DELETE( ivPtMixer );
}

/**
 * invoked when the song is reseted
 */
void Sequencer::newSong()
{
	this->ivPtMixer->getFxRack()->deleteSlots();
}

/**
 * renders the song
 */
void Sequencer::goNext( MSoundBuffer* pBuffer, unsigned int start, unsigned int stop )
{
	MSynchronize cls1( &ivPtSong->ivCriticalSection );

	MClock c1;
	c1.start();
	
	unsigned int samplesToRender = stop - start;
	unsigned int samplesRendered = 0;
	while( samplesRendered < samplesToRender )
	{
		// calc number of samples in current system step, includes shuffle...
		unsigned int samplesPerStep = unsigned int
			(FP(ivStepWidth) * getCurrentWidth( ivPtTimeController->getCurrentSystemStep(), ivShuffleValue ));

		// fallDownLocation - number of step to trigger a fallDown or noteOff event
		ASSERT( samplesPerStep >= 300 );
		unsigned int fallDownLocation = samplesPerStep - 300;

		// stepsLeftToRun - numbers of samples left to render(of one step)
		// clipped by the bufferlength
		unsigned int samplesLeftInStep = samplesPerStep - ivCurStep;
		unsigned int stepsLeftToRun = samplesLeftInStep;
		if( stepsLeftToRun + samplesRendered > samplesToRender )
			stepsLeftToRun = samplesToRender - samplesRendered;
		
		//int startBufferAt = curBufferStepCount;
		unsigned int currentStepMem = ivCurStep;

		// fetch position infos
		MSoundBuffer masterBuffer( 2, stepsLeftToRun );

		// trigger and render instruments...
		if( currentStepMem < fallDownLocation && fallDownLocation < (currentStepMem + stepsLeftToRun) )
		{
			int curPattern = ivPtTimeController->getCurrentPattern();
			int nextPattern = ivPtTimeController->getNextPattern();
			int curSysStep = ivPtTimeController->getCurrentSystemStep();
			int nextSysStep = ivPtTimeController->getNextSystemStep();

			// do twice calls of mixer::goNext with falldown triggering between
			unsigned int samplesLeftUntilFalldown = fallDownLocation - currentStepMem;
			ivPtMixer->goNext( &masterBuffer, 0, samplesLeftUntilFalldown );
			triggerNoteOff( curPattern, curSysStep );
			triggerFallDown( nextPattern, nextSysStep );
			ivPtMixer->goNext( &masterBuffer, samplesLeftUntilFalldown, stepsLeftToRun );
		}
		else
		{
			// render once into the masterbuffer...
			ivPtMixer->goNext( &masterBuffer, 0, stepsLeftToRun );
		}

		// paset master buffer into output buffer...
		MSoundBufferUtils::pasteBuffer( pBuffer, &masterBuffer, start + samplesRendered, stepsLeftToRun );
		/*pBuffer->pasteBuffer(
			&masterBuffer,
			start + samplesRendered, 
			stepsLeftToRun );*/

		// update rendering state...
		ivCurStep += stepsLeftToRun;
		samplesRendered += stepsLeftToRun;

		// do some sequencer calculating...
		if( ivCurStep >= samplesPerStep )
		{
			ivCurStep=0;

			if( ivPtTimeController->calculateNextSteps() )
				triggerOfflineUpdate();
			triggerNoteOn();
		}
	}

	c1.stop();
	//TRACE( "rendering took %f msec\n", c1.getLastTimeSpan() );

	//MClock c;
	//c.start();
	this->ivPtMixer->fireNotifications();
	//c.stop();
	//TRACE( "notify took %f msec\n", c.getLastTimeSpan() );*/
}

void Sequencer::processEvent( MEvent* pEvent )
{
}

/**
 * getCurrentWidth
 */
FP Sequencer::getCurrentWidth( int systemStep, FP shuffle )
{
	ASSERT( systemStep >= 0 && systemStep < SYSTEM_STEPS );
	int rest = systemStep % 4;
	return (9.0f/36.0f + (shuffle * SYSTEM_SHUFFLE[ rest ])) * 4.0f;
}

/**
 * play
 */
void Sequencer::play()
{
	MLogger::logMessage( "<sequencer::play/>\n" );

	ivPtTimeController->setCurrentPatternEx( ivPtTimeController->getCurrentPattern() );
	triggerReset();
	triggerNoteOn();
}

/** 
 * setBPM
 */
void Sequencer::setBpm( unsigned int bpm )
{
	ivBpm = bpm;
	ivStepWidth	= unsigned int((44100.0*60.0) / FP(ivBpm*8));
	fireBpmChanged();
}

/**
 * getBPM
 */
unsigned int Sequencer::getBpm()
{
	return ivBpm;
}

/**
 * getStepwidth
 */
int	Sequencer::getStepwidth()
{
	return ivStepWidth;
}

/**
 * setShuffle
 */
void Sequencer::setShuffle(FP value)
{
	ivShuffleValue = value;
}

/**
 * getShuffle
 */
FP Sequencer::getShuffle()
{
	return ivShuffleValue;
}

/**
 * stores as tree node
 */
MTreeNode* Sequencer::save()
{
	MTreeNode* ptBack = new MTreeNode( "sequencer" );

	// attributes
	ptBack->setAttribute( "bpm", MInt::toString( getBpm() ) );
	ptBack->setAttribute( "shuffle", MFloat::toString( getShuffle() ) );
	ptBack->setAttribute( "leftMarker", MInt::toString( int( ivPtTimeController->getLeftMarker() ) ) );
	ptBack->setAttribute( "rightMarker", MInt::toString( int( ivPtTimeController->getRightMarker() ) ) );

	// mixer controls
	unsigned int count = getMixer()->getControlCount();
	for( unsigned int i=0;i<count;i++ )
		ptBack->addChild( getMixer()->getControl( i )->save() );

	// song
	ptBack->addChild( ivPtSong->save() );

	// fx
	ptBack->addChild( ivPtMixer->getFxRack()->save() );

	return ptBack;
}

/**
 * loads from the given tree node
 */
void Sequencer::load( MTreeNode* parentNode )
{
	setBpm(
		MInt::parse(
			parentNode->getAttribute( "bpm" ),
			60,
			260,
			120 ) );

	setShuffle(
		MFloat::parse(
			parentNode->getAttribute( "shuffle" ),
			0.0f,
			1.0f,
			0.0f ) );

	ivPtTimeController->setLeftMarker(
		MInt::parse(
			parentNode->getAttribute( "leftMarker" ),
			0,
			640,
			0 ) );

	ivPtTimeController->setRightMarker(
		MInt::parse(
			parentNode->getAttribute( "rightMarker" ),
			0,
			640,
			3 ) );

	MTreeNodeIterator iter = parentNode->getIterator();
	while( iter.hasNext() )
	{
		MTreeNode* node = iter.nextTreeNode();
		String nodeName = node->getName();

		if( nodeName == "control" )
		{
			IControl* ptControl;
			if( (ptControl=getMixer()->getControlByName( nodeName )) )
				ptControl->load( node );
		}
		else if( nodeName == "MSong" )
			ivPtSong->load( node );
		else if( nodeName == "mobject" &&
			node->getAttribute( "class" ) == "MFxRack" )
			ivPtMixer->getFxRack()->load( node );
		else
			MLogger::logWarning( "Sequencer::load: sequencer found unknown tag!" );
	}

	ivPtTimeController->setCurrentPatternEx( 0 );
	ivPtTimeController->systemStepChanged( 0 );
	ivPtTimeController->systemPatternChanged( 0 );
}

/**
 * setSong
 */
void Sequencer::setSong( MSong* ptSong )
{
	MLogger::logMessage( "<sequencer::setsong song=\"0x%x\"/>\n", ptSong );
	ivPtSong = ptSong;
	if( ivPtSong )
		ivPtMixer->setSong( ivPtSong );
}

/**
 * getSong
 */
MSong* Sequencer::getSong()
{
	return ivPtSong;
}

/**
 * getMixer
 */
MDefaultMixer* Sequencer::getMixer()
{
	return ivPtMixer;
}

/**
 * setTimeController
 */
void Sequencer::setTimeController( MTimeController* ptController )
{
	if( ivPtTimeController )
		ivPtTimeController->removeSystemListener( this );

	ivPtTimeController = ptController;

	if( ivPtTimeController )
		ivPtTimeController->addSystemListener( this );
}

/**
 * getTimeController
 */
MTimeController* Sequencer::getTimeController()
{
	return ivPtTimeController;
}

void Sequencer::triggerNoteOff(int patternStep, int noteStep)
{
	unsigned int count = ivPtSong->getBarContainerCount();
	for( unsigned int i=0;i<count;i++ )
		triggerNoteOff( i, patternStep, noteStep );
}

void Sequencer::triggerFallDown(int patternStep, int noteStep)
{
	unsigned int count = ivPtSong->getBarContainerCount();
	for( unsigned int i=0;i<count;i++ )
		triggerFallDown( i, patternStep, noteStep );
}

/**
 * trigger note on
 */
void Sequencer::triggerNoteOn()
{
	int curPattern = ivPtTimeController->getCurrentPattern();
	int curSysStep = ivPtTimeController->getCurrentSystemStep();
	unsigned int count = (unsigned int)ivPtSong->getBarContainerCount();
	for( unsigned int i=0;i<count;i++ )
	{
		triggerNoteOn( i, curPattern, curSysStep );
		ivPtSong->getBarContainer( i )->triggerAutomationEvents(
			curPattern, curSysStep );
	}
}

/**
 * trigger note on 2
 */
void Sequencer::triggerNoteOn( int instrumentNo, int patternStep, int noteStep )
{
	MInstrument* pInstrument = ivPtSong->getBarContainer( instrumentNo )->getInstrument();
	if( pInstrument->getMute() == false && pInstrument->getOffline() == false )
	{
		MBarContainer* ptContainer = ivPtSong->getBarContainer( instrumentNo );
		MDefaultNote* ptNote = ptContainer->getNoteStartsAt( patternStep, noteStep );
		if( ptNote != 0 )
		{
			pInstrument->processEvent( &MEventNote( MEvent::NOTE_ON, ptNote ) );
		}
	}
}

/**
 * trigger note off
 */
void Sequencer::triggerNoteOff( int instrumentNo, int patternStep, int noteStep )
{
	MDefaultNote* ptNote = ivPtSong->getBarContainer( instrumentNo )->getNoteStopsAt( patternStep, noteStep );
	if( ptNote != 0 )
		ivPtSong->getBarContainer( instrumentNo )->getInstrument()->processEvent(
			&MEventNote( MEvent::NOTE_OFF, ptNote ) );
}

/**
 * trigger falldown
 */
void Sequencer::triggerFallDown(  int instrumentNo, int patternStep, int noteStep )
{
	MDefaultNote* ptNote = ivPtSong->getBarContainer( instrumentNo )->getNoteStartsAt( patternStep, noteStep );
	if( ptNote != 0 )
	{
		ivPtSong->getBarContainer( instrumentNo )->getInstrument()->processEvent(
			&MEventNote( MEvent::FALLDOWN, ptNote ) );
	}
}

/**
 * trigger reset
 */
void Sequencer::triggerReset()
{
	ivCurStep = 0;
	int instrumentCount = ivPtSong->getBarContainerCount();
	MEvent event( MEvent::RESET );
	for( int i=0;i<instrumentCount;i++ )
		ivPtSong->getBarContainer( i )->getInstrument()->processEvent( &event );
	ivPtMixer->getFxRack()->resetSlots();
}

/**
 * hasOverlappingBar
 * called to render only instruments with existing bars
 */
bool Sequencer::hasOverlappingBar( unsigned int instrumentNumber, unsigned int pattern )
{
	return ivPtSong->getBarContainer( instrumentNumber )->getBarOverlappingAt( pattern ) != 0;
}

/**
 * systemStepChanged (inheritanced from ISystemListener)
 */
void Sequencer::systemStepChanged( int currentStep )
{
	//i_curStep = 0;
	//TRACE( "<void Sequencer::systemStepChanged( int currentStep=%i )>\n", currentStep );
}

/**
 * systemPatternChanged (inheritanced from ISystemListener)
 */
void Sequencer::systemPatternChanged( int currentPattern )
{
	triggerOfflineUpdate();
	triggerReset();
	triggerNoteOn();
}

/**
 * add bpm listener
 */
void Sequencer::addBpmListener( IBpmListener* ptListener )
{
	ivBpmListeners.push_back( ptListener );
}

/**
 * remove bpm listener
 */
void Sequencer::removeBpmListener( IBpmListener* ptListener )
{
	for( BpmIterator i=ivBpmListeners.begin();i!=ivBpmListeners.end();i++ )
		if( (*i) == ptListener )
		{
			ivBpmListeners.erase( i );
			break;
		}
}

/**
 * fire bpm changed
 */
void Sequencer::fireBpmChanged()
{
	for( BpmIterator i=ivBpmListeners.begin();i!=ivBpmListeners.end();i++ )
		(*i)->bpmChanged( getBpm() );
}

/**
 * get steps current sample
 */
unsigned int Sequencer::getStepsCurrentSample()
{
	return ivCurStep;
}

/**
 * get steps per beat
 */
int Sequencer::getStepsPerBar()
{
	return SYSTEM_STEPS;
}

void Sequencer::triggerOfflineUpdate()
{
	int pattern = ivPtTimeController->getCurrentPattern();
	unsigned int count = this->ivPtSong->getBarContainerCount();
	for( unsigned int i=0;i<count;i++ )
		ivPtSong->getBarContainer( i )->getInstrument()->setOffline(
			! hasOverlappingBar( i, pattern ) );
}