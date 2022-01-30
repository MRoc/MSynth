/*

	MReverb (C)2000-2003 MRoc hifiShock

	v 0.1 -		6 combFilter and 2 allpass filter with lowpass 
				without interpolation, runtime optimized

	v.0.2 -		4 combFilter, 2 allpass, modified parameters for longer decaytimes
	v.0.3 -		stereoable

*/
#include "MReverb.h"
#include "../../../MSynthConstants.h"

#define ALLPASS_FEEDBACK 1.0f

FP MReverb::COMB_DECAY_COEFFS[] = { 5.0f, 8.0f, 14.0f, 18.0f };
FP MReverb::ALLPASS_DECAY_COEFFS[] = { 15.0f, 32.0f };

MRtti MReverb::gvRtti = MRtti( "MReverb", MReverb::createInstance );

MReverb::MReverb() :
	ivPtMix( 0 ),
	ivPtInput( 0 ),
	ivPtDecay( 0 ),
	ivPtFeedback( 0 ),
	ivPtLowpass( 0 ),
	ivPtFeedback2( 0 ),
	ivReverbBuffer( 0 ),
	ivMaxDelayTime( 0 ),
	ivBufferLength( 0 ),
	ivCurrentStep( 0 )
{
#ifdef _DEBUG
	ivInitialized = false;
#endif
}

bool MReverb::create( unsigned int channelCount )
{
	initReverb( channelCount );
	return true;
}

void MReverb::initReverb( unsigned int channelCount )
{
#ifdef _DEBUG
	ASSERT( ! ivInitialized );
	ivInitialized = true;
#endif

	MSynchronize cls( &ivCriticalSection );

	ivPtMix = new MFloatControl( String( "Mix" ), 0.0f, 1.0f, 0.6f, false );
	ivPtInput = new MFloatControl( String( "Input" ), 0.0f, 7.0f, 0.4f, false );
	ivPtDecay = new MFloatControl( String( "Decay" ), 0.5f, 2.0f, 1.6f, true );
	ivPtDecay->addControlListener( this );
	ivPtFeedback = new MFloatControl( String( "Feedback" ), 0.0f, 1.0f, 0.5f, false );
	ivPtFeedback->addControlListener( this );
	ivPtLowpass = new MFloatControl( String( "Lowpass" ), 0.0f, 1.0f, 0.5f, false );
	ivPtFeedback2 = new MFloatControl( String( "Feedback2" ), 0.0f, 1.0f, 1.0f, false );

	ivMaxDelayTime = getMax( COMB_DECAY_COEFFS, MAXCOMBFILTER ) * 2;
	ivBufferLength = int( ivMaxDelayTime * MW_SAMPLINGRATE / 1000 );

	setChannelCount( channelCount );

	ivCurrentStep = 0;

	// allpass
	FP n = (FP)( MW_SAMPLINGRATE / 1000.0 );
	for( int i=0;i<MAXALLPASSFILTER;i++ )
	{
		ivAllpassDelayTimes[ i ] = (FP)ALLPASS_DECAY_COEFFS[ i ];
		ivAllpassOffsets[ i ]	= int( FP(ivAllpassDelayTimes[ i ]) * n );;
	}

	setFeedback2( ALLPASS_FEEDBACK );

	processEvent( &MEvent( MEvent::RESET ) );

	updateCoeffsFromDecay();
	updateCoeffsFromFeedback();
}

MReverb::~MReverb()
{
	MSynchronize cls( &ivCriticalSection );

	SAFE_DELETE( ivPtMix );
	SAFE_DELETE( ivPtInput );
	ivPtDecay->removeControlListener( this );
	SAFE_DELETE( ivPtDecay );
	ivPtFeedback->removeControlListener( this );
	SAFE_DELETE( ivPtFeedback );
	SAFE_DELETE( ivPtLowpass );
	SAFE_DELETE( ivPtFeedback2 );

	TRACE("<Deleting MReverb>\n");

	for( unsigned int i=0;i<ivChannelCount;i++ )
		SAFE_DELETE( ivReverbBuffer[ i ] );
	SAFE_DELETE( ivReverbBuffer );
}

void MReverb::setChannelCount( unsigned int channelCount )
{
	ivChannelCount = channelCount;
	ivReverbBuffer = new MReverbBuffer*[ channelCount ];
	for( unsigned int i=0;i<ivChannelCount;i++ )
		ivReverbBuffer[ i ] = new MReverbBuffer( ivBufferLength );
}

void MReverb::processEvent( MEvent* ptEvent )
{
	if( ptEvent->getType() == MEvent::RESET )
	{
		MSynchronize cls( &ivCriticalSection );
		for( unsigned int i=0;i<ivChannelCount;i++ )
			ivReverbBuffer[ i ]->resetBuffer();
	}
}

void MReverb::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt )
{
	MSynchronize cls( &ivCriticalSection );

	FP wetMix = ivPtMix->getValue();
	FP dryMix = 1 - wetMix;
	FP combReverbInput = ivPtInput->getValue();

	int currentStep;
	FP apFb = ivPtFeedback2->getValue(); //allpass feedback
	FP lowpass = ivPtLowpass->getValue();
	for( unsigned int i=0;i<ivChannelCount;i++ )
	{
		currentStep = ivCurrentStep;

		FP *bufferStop = buffer->getData(i) + stopAt;
		for( FP *pointer=buffer->getData(i) + startFrom; pointer<bufferStop; pointer++ )
		{
			FP	output = 0;
			FP	combOutput = 0;
			int	tempStep = 0;

			FP* combFeedbacks = ivCombFeedbacks;
			FP** combRingBuffers = ivReverbBuffer[ i ]->ivPtCombRingBuffers;
			// comb
			for( int j=0;j<MAXCOMBFILTER;j++, combFeedbacks++, combRingBuffers++ )
			{
				tempStep = currentStep - ivCombOffsets[ j ];
		
				if( tempStep < 0 )
					tempStep += ivBufferLength;
	
				ASSERT( tempStep < ivBufferLength && tempStep >= 0 );
				ASSERT( currentStep < ivBufferLength && currentStep >= 0 );
		
				combOutput = (*combRingBuffers)[tempStep];
				(*combRingBuffers)[ currentStep ] = combOutput * (*combFeedbacks) + combReverbInput * (*pointer);
	
				output += combOutput;
			}
	
			// lowpass
			ivReverbBuffer[ i ]->ivIl += lowpass * (output - ivReverbBuffer[ i ]->ivUc - ivReverbBuffer[ i ]->ivIl );
			output = ivReverbBuffer[ i ]->ivUc += lowpass * ivReverbBuffer[ i ]->ivIl;
	
			// allpass
			FP** allpassRingBuffers = ivReverbBuffer[ i ]->ivPtAllpassRingBuffers;
			int* allpassOffsets = ivAllpassOffsets;
			for( j=0;j<MAXALLPASSFILTER;j++, allpassRingBuffers++, allpassOffsets++ )
			{
				tempStep = currentStep - (*allpassOffsets);
				if( tempStep < 0 )
					tempStep += ivBufferLength;
	
				ASSERT( tempStep < ivBufferLength && tempStep >= 0 );
				ASSERT( currentStep < ivBufferLength && currentStep >= 0 );

				FP temp = (*allpassRingBuffers)[ tempStep ];
				(*allpassRingBuffers)[ tempStep ] = apFb * temp + output;
				output = temp - apFb * (*allpassRingBuffers)[ tempStep ];
			}
	
			if( (++currentStep) >= ivBufferLength )
				currentStep = 0;
	
			(*pointer) = (*pointer) * dryMix + wetMix * output;
		}
	};

	ivCurrentStep = currentStep;
}

void MReverb::setInput( FP input /*0.0 - 1.0 */)
{
	ASSERT( input >= 0.0 && input <= 1.0 );
	ivPtInput->setValue( input );
}

FP MReverb::getInput()
{
	return ivPtInput->getValue();
}

void MReverb::setDecay( FP decay /*0.5 - 2.0*/)
{
	ASSERT( decay >= 0.5 && decay <= 2.0 );
	ivPtDecay->setValue( decay );
}

void MReverb::updateCoeffsFromDecay()
{
	FP n = FP(MW_SAMPLINGRATE) / 1000.0f;
	for( int i=0;i<MAXCOMBFILTER;i++ )
	{
		ivCombDelayTimes[ i ] = MReverb::COMB_DECAY_COEFFS[ i ] * ivPtDecay->getValue();
		ivCombOffsets[ i ] = int( FP(ivCombDelayTimes[i]) * n );
		ASSERT( ivCombOffsets[ i ] <= ivBufferLength );
	}
}

FP MReverb::getDecay()
{
	return ivPtDecay->getValue();
}

void MReverb::setFeedback( FP feedback )
{
	ASSERT( feedback >= 0.0 && feedback <= 1.0 );
	ivPtFeedback->setValue( feedback );

}

void MReverb::updateCoeffsFromFeedback()
{
	for( int i=0;i<MAXCOMBFILTER;i++ )
		ivCombFeedbacks[ i ] = (FP)(0.25 * ivPtFeedback->getValue() + 0.74); // max: 0.99
}

FP MReverb::getFeedback()
{
	return ivPtFeedback->getValue();
}

void MReverb::setFeedback2( FP feedback2 )
{
	ASSERT( feedback2 >= 0.0 && feedback2 <= 1.0 );
	ivPtFeedback2->setValue( feedback2 );
}

FP MReverb::getFeedback2()
{
	return ivPtFeedback2->getValue();
}

void MReverb::setHighdamp( FP highDump /* 0.0 - 1.0 */ )
{
	ASSERT( highDump >= 0.0 && highDump <= 1.0 );
	ivPtLowpass->setValue( highDump );
}

FP MReverb::getHighdamp()
{
	return ivPtLowpass->getValue();
}

String MReverb::getName()
{
	return String("Reverb");
}

String MReverb::getShortName()
{
	return getName();
}

unsigned int MReverb::getControlCount()
{
	return 6;
}

IControl* MReverb::getControl( unsigned int index )
{
	switch( index )
	{
		case MIX: return ivPtMix; break;
		case INPUT: return ivPtInput; break;
		case DECAY: return ivPtDecay; break;
		case FEEDBACK: return ivPtFeedback; break;
		case LOWPASS: return ivPtLowpass; break;
		case FEEDBACK2: return ivPtFeedback2; break;
		default: return 0;
	};
}

void MReverb::valueChanged( MControlListenerEvent *anEvent )
{
	IControl* pSrc = anEvent->getSource();
	if( pSrc == ivPtDecay )
		updateCoeffsFromDecay();
	else if( pSrc == ivPtFeedback )
		updateCoeffsFromFeedback();
}

MObject* MReverb::createInstance()
{
	return new MReverb();
}

FP MReverb::getMax( FP* data, unsigned int dataCount )
{
	FP max = 0;
	for( unsigned int i=0;i<dataCount;i++ )
		if( data[ i ] > max )
			max = data[ i ];

	return max;
}

IRtti* MReverb::getRtti() const
{
	return &gvRtti;
}

void* MReverb::getInterface( const String &className ) const
{
	if( className == "MReverb" )
		return (void*) ((MReverb*)this);
	else
		return MTransformer::getInterface( className );
}