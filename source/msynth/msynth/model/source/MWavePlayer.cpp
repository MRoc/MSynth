/*-----------------------------------------------------------------------------
	MWavePlayer (C)2000 MRoc hifiShock

	v. 0.1			-> design
	v. 0.2			-> performanceTune(loop, accent out of goNext),
						better resampling
	v. 0.3			-> bufferConcept
	v. 0.3a			-> highliy optimized goNext()-method,
						less performance ever but filling stereo-buffers

	v. 0.4			-> ThreadSafe
	v. 0.4a			-> cleanup
	v. 0.5			-> stereo sample support

-----------------------------------------------------------------------------*/
#include "MWavePlayer.h"
#include "../interfaces/MSoundBufferUtils.h"

/*
 * runtime support
 */
MRtti MWavePlayer::gvRtti = MRtti( "MWavePlayer", MWavePlayer::createInstance );

/**
 * constructor 1
 */
MWavePlayer::MWavePlayer() :
	ivId( -1 ),
	ivPtWave( 0 ), 
	ivReversed( false ),
	ivBpm( 120 ),
	ivPtNameControl( 0 ),
	ivPtLoopControl( 0 ),
	ivPtMuteControl( 0 ),
	ivPtRevControl( 0 ),
	ivPtVolControl( 0 ),
	ivPtPitchControl( 0 ),
	ivPtPanControl( 0 ),
	ivPtExclusiveControl( 0 ),
	ivIsOn( false ),
	ivIsFallDown( false ),
	ivNoteVolume( 1.0f ),
	ivFallDown( 1.0f ),
	ivPlayPos( 0.0f ),
	ivPtValidControl( 0 )
{
	init();
}

/**
 * constructor 2
 */
MWavePlayer::MWavePlayer( int id ) :
	ivId( id ),
	ivPtWave( 0 ), 
	ivReversed( false ),
	ivBpm( 120 ),
	ivPtNameControl( 0 ),
	ivPtLoopControl( 0 ),
	ivPtMuteControl( 0 ),
	ivPtRevControl( 0 ),
	ivPtVolControl( 0 ),
	ivPtPitchControl( 0 ),
	ivPtPanControl( 0 ),
	ivPtExclusiveControl( 0 ),
	ivIsOn( false ),
	ivIsFallDown( false ),
	ivNoteVolume( 1.0f ),
	ivFallDown( 1.0f ),
	ivPlayPos( 0.0f ),
	ivPtValidControl( 0 )
{
	init();
}

/**
 * constructor 3
 */
MWavePlayer::MWavePlayer(int id, unsigned int bpm, IAutomationTracker* pTracker ) :
	ivId( id ),
	ivBpm( bpm ),
	ivPtWave( 0 ), 
	ivReversed( false ),
	ivPtNameControl( 0 ),
	ivPtLoopControl( 0 ),
	ivPtMuteControl( 0 ),
	ivPtRevControl( 0 ),
	ivPtVolControl( 0 ),
	ivPtPitchControl( 0 ),
	ivPtPanControl( 0 ),
	ivPtExclusiveControl( 0 ),
	ivIsOn( false ),
	ivIsFallDown( false ),
	ivNoteVolume( 1.0f ),
	ivFallDown( 1.0f ),
	ivPlayPos( 0.0f ),
	ivPtValidControl( 0 )
{
	init();
	registerTargets( pTracker );
}

/**
 * Sets the waveplayer in its initial state
 */
void MWavePlayer::init()
{
	ivPtValidControl = new MBoolControl( String( "IsValid" ), false );
	ivPtExclusiveControl = new MBoolControl( String( "Exclusiv" ), false );
	ivPtNameControl = new MStringControl( String( "Name" ), String( "" ) );
	ivPtLoopControl = new MBoolControl( String( "Loop" ), false );
	ivPtMuteControl = new MBoolControl( String( "Mute" ), false );
	ivPtMuteControl->addControlListener( this );
	ivPtRevControl = new MBoolControl( String( "Reverse" ), false );
	ivPtRevControl->addControlListener( this );
	ivPtVolControl = new MFloatControl( String( "Volume" ), 0.0f, 2.0f, 1.0f, false );
	ivPtPitchControl = new MRangeControl( String( "Tune" ), 0.5f, 1.0f, 2.0f, 1.0f );
	ivPtPanControl = new MFloatControl( String( "Panorama" ), 0.0f, 1.0f, 0.5f, false );
}
		
/**
 * destructor
 */
MWavePlayer::~MWavePlayer()
{
	MSynchronize sl( &ivCriticalSection );

	SAFE_DELETE( ivPtValidControl );
	SAFE_DELETE( ivPtNameControl );
	SAFE_DELETE( ivPtLoopControl );
	ivPtMuteControl->removeControlListener( this );
	SAFE_DELETE( ivPtMuteControl );
	SAFE_DELETE( ivPtExclusiveControl );
	ivPtRevControl->removeControlListener( this );
	SAFE_DELETE( ivPtRevControl );
	SAFE_DELETE( ivPtVolControl );
	SAFE_DELETE( ivPtPitchControl );
	SAFE_DELETE( ivPtPanControl );
	SAFE_DELETE( ivPtWave );
}

/**
 * sets the used wave sample
 */
void MWavePlayer::setWave( MWave *wave )
{
	MSynchronize sl( &ivCriticalSection );

	if( ivPtWave )
		SAFE_DELETE( ivPtWave );
	
	ivPtWave = wave;

	ivPtValidControl->setValue( ivPtWave ? true : false );
}

/**
 * returns the used wave sample
 */
MWave *MWavePlayer::getWave()
{
	return ivPtWave;
}

/**
 * renders the samples
 */
void MWavePlayer::goNext( MSoundBuffer *buffer, unsigned int renderFrom, unsigned int renderTo  )
{
	MSynchronize sl( &ivCriticalSection );

	// validate mute&off...
	if( ivPtMuteControl->getValue() && ivIsOn )
		this->resetPlayState();

	// return if waveplayer is in an invalid state...
	if( ivIsOn && ! ivPtMuteControl->getValue() && isValid() )
	{
		// fetch sample data...
		MW_SAMPLETYPE* pSample = (MW_SAMPLETYPE*) ivPtWave->getData();
		unsigned int sampleLength = ivPtWave->getDataLength() / ( ivPtWave->getChannelCount() * sizeof(MW_SAMPLETYPE) );

		// fetch play pos delta...
		FP playPosDelta;
		if( ivPtLoopControl->getValue() )
			playPosDelta = sampleLength / ( ( 240.0f / ivBpm ) * MW_SAMPLINGRATE );
		else
			playPosDelta = ivPtPitchControl->getValue();

		// sample data...
		unsigned int modSampleLength = sampleLength - 1; // better leaf the last out

		// calculate how long to render and alloc mem
		unsigned int samplesToRender = renderTo - renderFrom;
		MSoundBuffer tmpBuffer( ivPtWave->getChannelCount(), samplesToRender );
		unsigned int writtenSamples =
			(ivPlayPos + samplesToRender * playPosDelta) > sampleLength ? 
				int( (sampleLength - ivPlayPos) / playPosDelta ) :
				samplesToRender;

		// FIRST STEP - Render to buffer
		FP multplyWith = ivPtVolControl->getValue() * ivNoteVolume;
		unsigned int iterationsLeft = (int) (( modSampleLength - ivPlayPos ) / playPosDelta);
		if( iterationsLeft > samplesToRender )
			iterationsLeft = samplesToRender;

		ASSERT(
			iterationsLeft <= samplesToRender &&
			(ivPlayPos + playPosDelta * iterationsLeft) < sampleLength );

		// required to prevent round off error...
		unsigned int iter = 0;
		FP oldPP = ivPlayPos;

		// required for inerpolation...
		unsigned int i1, i2;

		// render...
		if( ivPtWave->getChannelCount() == 1 )
		{
			FP* ptStart = tmpBuffer.getData( 0 );
			FP* ptStop = ptStart + iterationsLeft;
			for( FP *pt=ptStart;pt<ptStop;pt++ )
			{
				// linear interpolation
				i1 = unsigned int(ivPlayPos);
				i2 = i1 + 1;
				ASSERT( i1 < unsigned int( sampleLength ) );
				ASSERT( i2 < unsigned int( sampleLength ) );
				(*pt) = ((pSample[i2]-pSample[i1]) * (ivPlayPos - i1) + pSample[i1]) * multplyWith;

				ivPlayPos = oldPP + (++iter) * playPosDelta; 
			}
		}
		else if( ivPtWave->getChannelCount() == 2 )
		{
			FP* ptStart1 = tmpBuffer.getData( 0 );
			FP* ptStop1 = ptStart1 + iterationsLeft;
			FP* ptStart2 = tmpBuffer.getData( 1 );
			FP* ptStop2 = ptStart2 + iterationsLeft;
			FP* pt1 = ptStart1;
			FP* pt2 = ptStart2;
			unsigned int i12, i22;
			for( ;pt1<ptStop1;pt1++,pt2++ )
			{
				// linear interpolation
				i1 = unsigned int(ivPlayPos);
				i2 = i1 + 1;
				ASSERT( i1 < unsigned int( sampleLength ) );
				ASSERT( i2 < unsigned int( sampleLength ) );

				i12 = i1*2;
				i22 = i2*2;
				(*pt1) = ((pSample[i22]-pSample[i12]) * (ivPlayPos - i1) + pSample[i12]) * multplyWith;
				(*pt2) = ((pSample[i22+1]-pSample[i12+1]) * (ivPlayPos - i1) + pSample[i12+1]) * multplyWith;

				ivPlayPos = oldPP + (++iter) * playPosDelta; 
			}
		}

		// sample-end reached???
		if( ivPlayPos >= modSampleLength )
			resetPlayState();			

		if( ivIsFallDown )
			ivFallDown = (FP)
				MSoundBufferUtils::fade(
					&tmpBuffer,
					0,
					tmpBuffer.getDataLength(),
					ivFallDown,
					-0.003333 );

		MSoundBufferUtils::addStereo(
			buffer,
			&tmpBuffer,
			1/MW_MAX,
			ivPtPanControl->getValue() );
	}
}

/** 
 * sets sound off, playpos to the begining and resets falldown
 */
void MWavePlayer::resetPlayState()
{
	ivIsOn			= false;
	ivPlayPos		= 0;
	
	ivFallDown		= 1.0;
	ivIsFallDown	= false;
}

/**
 * setOn
 */
void MWavePlayer::setOn(bool value)
{
	if( ! ivPtMuteControl->getValue() )
	{
		resetPlayState();
		ivIsOn = value;
	}
}

/**
 * getOn
 */
bool MWavePlayer::getOn()
{
	return ivIsOn;
}

/**
 * setFallDown
 */
void MWavePlayer::setFallDown(bool value)
{
	ivIsFallDown = value;
}

/**
 * getFallDown
 */
bool MWavePlayer::getFallDown()
{
	return ivIsFallDown;
}

/**
 * getExclusive
 */
bool MWavePlayer::getExclusive()
{
	return ivPtExclusiveControl->getValue();
}

/**
 * getLoop
 */
bool MWavePlayer::getLoop()
{
	return ivPtLoopControl->getValue();
}

/**
 * setNoteVolume
 */
void MWavePlayer::setNoteVolume( FP noteVolume )
{
	ivNoteVolume = noteVolume;
}

/**
 * setNoteVolume
 */
FP MWavePlayer::getNoteVolume()
{
	return ivNoteVolume;
}

/** 
 * setBpm
 */
void MWavePlayer::setBpm(unsigned int bpm)
{
	ivBpm = bpm;
}

/** 
 * getBpm
 */
unsigned int MWavePlayer::getBpm()
{
	return ivBpm;
}

/**
 * loadFile
 */
void MWavePlayer::loadFile(const char *fileName)
{
	MSynchronize sl( &ivCriticalSection );

	resetPlayState();

	TRACE( "<Loading file: %s>\n", fileName );

	if( ivPtWave == 0 )
		ivPtWave = new MWave();	
	
	try
	{
		ivPtWave->loadFile( fileName );
		if( ivPtWave->getChannelCount() != 1 &&	ivPtWave->getChannelCount() != 2 )
			ivPtWave->makeMono();
		if( ivPtWave->getBitsPerSample() != 16 )
			ivPtWave->make16bit();
		ivPtValidControl->setValue( true );
	}
	catch( MException anException )
	{
		SAFE_DELETE( ivPtWave );
		ivPtValidControl->setValue( false );
		setSampleName( "" );
		throw anException;
	}

	setSampleName( fileName );	
}

/** 
 * saveFile
 */
void MWavePlayer::saveFile( const char *fileName )
{
	MSynchronize sl( &ivCriticalSection );
	ivPtWave->saveFile( fileName );
}

/**
 * isValid
 */
bool MWavePlayer::isValid()
{
	return ivPtWave && ivPtWave->getData();
}

/**
 * resetSample
 */
void MWavePlayer::resetSample()
{
	MSynchronize sl( &ivCriticalSection );

	resetPlayState();

	SAFE_DELETE( ivPtWave );
	setSampleName( "" );

	ivPtValidControl->setValue( false );
}

/**
 * getSampleName
 */
String MWavePlayer::getSampleName()
{
	return ivPtNameControl->getValue();
}

/**
 * setSampleName
 */
void MWavePlayer::setSampleName( String name )
{
	ivPtNameControl->setValue( name );
}

/** 
 * returns the type
 */
String MWavePlayer::getType()
{
	return String("MDrumbox");
}

/**
 * returns the class name
 */
String MWavePlayer::getName()
{
	return String("MWavePlayer");
}

String MWavePlayer::getShortName()
{
	return String("Wave");
}

int MWavePlayer::getViewId()
{
	return DESC_VIEW_WAVEPLAYER;
};
	
MTreeNode* MWavePlayer::save()
{
	MTreeNode* ptBack = 0;
	MSynchronize sl( &ivCriticalSection );

	ptBack = new MTreeNode( "channel" );
	unsigned int count = getControlCount();
	for( unsigned int i=0;i<count;i++ )
		ptBack->addChild( getControl( i )->save() );

	if( ivPtWave )
		ptBack->addChild( ivPtWave->save() );

	return ptBack;
}

void MWavePlayer::load( MTreeNode* node )
{
	MSynchronize sl( &ivCriticalSection );

	MTreeNodeIterator iter = node->getIterator();
	while( iter.hasNext() )
	{
		MTreeNode* controlNode = iter.nextTreeNode();
		String nodeName = controlNode->getName();
		if( nodeName == "control" )
		{
			String controlName = controlNode->getAttribute( "name" );
			IControl* ptControl = getControlByName( controlName );
			if( ptControl )
				ptControl->load( controlNode );
		}
		else if( nodeName == MObject::ELEM_OBJECT )
		{
			if( controlNode->getAttribute( MObject::ATTR_CLASS ) ==	MWave::gvRtti.getClassName() )
			{
				if( ! ivPtWave )
					ivPtWave = new MWave();
				ivPtWave->load( controlNode );
			}
		}
		else if( nodeName == "sample" )
		{
			if( ! ivPtWave )
				ivPtWave = new MWave();
			ivPtWave->load( controlNode );
		}
	}
}

void MWavePlayer::valueChanged( MControlListenerEvent *anEvent )
{
	IControl* pSrc = anEvent->getSource();
	if( pSrc == ivPtRevControl )
	{
		if( ivPtRevControl->getValue() != ivReversed )
		{
			if( isValid() )
			{
				MSynchronize sl( &ivCriticalSection );

				ivReversed = ! ivReversed;
				unsigned int samples = ivPtWave->getDataLength() / sizeof(MW_SAMPLETYPE);
				MW_SAMPLETYPE *buffer = new MW_SAMPLETYPE[ samples ];	
				MW_SAMPLETYPE *source = (MW_SAMPLETYPE*) (ivPtWave->getData() + ivPtWave->getDataLength());

				for(unsigned int i=0;i<samples;i++)
					buffer[i] = *(--source);
				
				ivPtWave->setData( (unsigned char*)buffer, ivPtWave->getDataLength() );
			}
		}
	}
	else if( pSrc == ivPtMuteControl )
	{
		if( ivIsOn )
			resetPlayState();
	}
}

unsigned int MWavePlayer::getControlCount()
{
	return ID;
}

IControl* MWavePlayer::getControl( unsigned int index )
{
	switch( index )
	{
		case SAMPLENAME: return ivPtNameControl; break;
		case VOLUME: return ivPtVolControl; break;
		case PITCH: return ivPtPitchControl; break;
		case PAN: return ivPtPanControl; break;
		case REV: return ivPtRevControl; break;
		case MUTE: return ivPtMuteControl; break;
		case LOOP: return ivPtLoopControl; break;
		case EXCLUSIVE: return ivPtExclusiveControl; break;
		case ISVALID: return ivPtValidControl; break;
		default: return 0; break;
	}
}

IControl* MWavePlayer::getControlByName( String name )
{
	IControl* back = 0;
	unsigned int count = getControlCount();
	for( unsigned int i=0;i<count;i++ )
		if( getControl( i ) != 0 &&
			getControl( i )->getName() == name )
			back = getControl( i );
	return back;
}

void MWavePlayer::registerTargets( IAutomationTracker* tracker )
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

MBoolControl* MWavePlayer::getValidControl()
{
	return ivPtValidControl;
}

String MWavePlayer::toString()
{
	return this->ivPtNameControl->getValue();
}

/**
 * returns the runtime type information
 */
IRtti* MWavePlayer::getRtti() const
{
	return &gvRtti;
}

/**
 * querries for a specified interface
 */
void* MWavePlayer::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MWavePlayer*)this);
	else if( className == "IControlListener" )
		return (void*) ((IControlListener*)this);
	else if( className == "IControlCollection" )
		return (void*) ((IControlCollection*)this);
	else
		return MObject::getInterface( className );
}

/**
 * creates an instance of this class
 */
MObject* MWavePlayer::createInstance()
{
	return new MWavePlayer();
}

bool MWavePlayer::getMute()
{
	return ivPtMuteControl->getValue();
}

unsigned int MWavePlayer::getChildCount()
{
	return 0;
}

IControlCollection* MWavePlayer::getChild( unsigned int index )
{
	ASSERT( false );
	return 0;
}