/**
 * MWaveOsc (C)2000-2003 MRoc hifiShock
 */

#include "MWaveOsc.h"

MRtti MWaveOsc::gvRtti = MRtti( "MWaveOsc", MWaveOsc::createInstance );

MWaveOsc::MWaveOsc() :
	ivIsReversed( false ),
	ivIsOn( false ),
	ivPtWave( 0 ),
	ivPtSampleNameControl( 0 ),
	ivPtReverseControl( 0 ),
	ivPtLoopControl( 0 ),
	ivPitch( 1.0 ),
	ivSampleFrequence( 220 ),
	ivPlayPos( 0 )
{
	ivFreq = 220;

	ivPtWave = new MWave();

	ivPtReverseControl = new MBoolControl( "Reverse", false );
	ivPtReverseControl->addControlListener( this );
	ivPtLoopControl = new MBoolControl( "Loop", false );
	ivPtLoopControl->addControlListener( this );
	ivPtSampleNameControl = new MStringControl( "", "" );
}
		
MWaveOsc::~MWaveOsc()
{
	ivPtReverseControl->removeControlListener( this );
	SAFE_DELETE( ivPtReverseControl );
	ivPtLoopControl->removeControlListener( this );
	SAFE_DELETE( ivPtLoopControl );
	SAFE_DELETE( ivPtSampleNameControl );

	if( ivPtWave )
		SAFE_DELETE( ivPtWave );
}

unsigned int MWaveOsc::getControlCount()
{
	return MWAVEOSC_CONTROL_COUNT;
}

IControl* MWaveOsc::getControl( unsigned int index )
{
	IControl* back = 0;
	switch( index )
	{
		case OCTAVE: back = ivPtOctaveControl; break;
		case VOLUME: back = ivPtVolumeControl; break;
		case TUNE: back = ivPtTuneControl; break;
		case LOOP: back = ivPtLoopControl; break;
		case REVERSE: back = ivPtReverseControl; break;
		case SAMPLENAME: back = ivPtSampleNameControl; break;
	}
	return back;
}

void MWaveOsc::valueChanged( MControlListenerEvent *anEvent )
{
	IControl* pSrc = anEvent->getSource();
	if( pSrc == this->ivPtOctaveControl )
		updateOctaveMultiplier();
	else if( pSrc == this->ivPtTuneControl )
		setFreq( getFreq() );
	else if( pSrc == this->ivPtReverseControl )
	{
		if( getReversed() != ivIsReversed )
			reverse();
	}
}

void MWaveOsc::setFreq(FP freq)
{
	ivFreq = freq;
	ivPitch = (ivFreq * ivOctaveMultiplier * ivPtTuneControl->getValue()) / ivSampleFrequence;

	setOn(true);
}

void MWaveOsc::addPitch(FP delta)
{
	ivFreq += delta;
	ivPitch = (ivFreq * ivOctaveMultiplier * ivPtTuneControl->getValue()) / ivSampleFrequence;
}

void MWaveOsc::reset()
{
	setOn(false);
}

void MWaveOsc::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt )
{
	MSynchronize cls( &ivCriticalSection );

	if( isValid() && getOn() )
	{
		unsigned int sampleLength = ivPtWave->getDataLength() / sizeof( MW_SAMPLETYPE );
		unsigned int modSampleLength = sampleLength - 1;
		MW_SAMPLETYPE* ptData = (MW_SAMPLETYPE*) ivPtWave->getData();
		FP multiplyWith = ivPtVolumeControl->getValue() / MW_MAX;			

		FP *ptStop = buffer->getData(0) + stopAt;

		if( getPortamento() )
		{
			ASSERT( ivPortamentoStep <= ivPortamentoLength );

			unsigned int samplesToRender = stopAt - startFrom;
			unsigned int portamentoStepsLeft = ivPortamentoLength - ivPortamentoStep;
			unsigned int portamentoStepsRendering = portamentoStepsLeft;
			if( portamentoStepsRendering > samplesToRender )
				portamentoStepsRendering = samplesToRender;

			FP frequencyDelta = ivPortamentoTargetFrequence - ivFreq;
			FP portamentoDelta = frequencyDelta / portamentoStepsRendering;

			FP constPortamentoDeltaMulti = ivOctaveMultiplier * ivPtTuneControl->getValue() / ivSampleFrequence;

			unsigned int p_1, p_2;
			for( FP *pt=buffer->getData(0) + startFrom; pt < ptStop; pt++ )
			{
				if( ivIsOn )
				{
					if( ivPlayPos >= modSampleLength )	//Sample-end reached
					{
						if( getLooped() )
							ivPlayPos = 0;
						else
							reset();
					}
					else
					{
						p_1 = unsigned int(ivPlayPos);
						p_2 = p_1 + 1;

						ASSERT( p_1 < unsigned int( sampleLength ) );
						ASSERT( p_2 < unsigned int( sampleLength ) );
						
						(*pt) = ((ptData[p_2]-ptData[p_1]) * (ivPlayPos - p_1) + ptData[p_1]) * multiplyWith;
				
						ivPlayPos += ivPitch;
						if( ivPortamentoStep < ivPortamentoLength )
						{	
							this->ivFreq += portamentoDelta;
							this->ivPitch = ivFreq * constPortamentoDeltaMulti;
							ivPortamentoStep++;
						}
						else
							stopPortamento();
					}
				}
				else
				{
					break;
				}
			}

			/*unsigned int modSampleLength = this->ivPtWave->getDataLength() / sizeof( MW_SAMPLETYPE );
			unsigned int samplesToRender = stopAt - startFrom; // samples die gerendert werdens sollen
			unsigned int portamentoSteps = ivPortamentoLength - ivPortamentoStep; // steps zum finishen des portamentos

			// constants for the frequency(X)
			float frequency0 = ivFreq; // start frequence
			float targetFrequence = ivPortamentoTargetFrequence; // ziel frequence
			float aFrequence = ( targetFrequence - frequency0 ) / portamentoSteps; // steigung von frequency(X)

			// constant for the stepWidth(X) = frequency(X)*c
			float c = ivOctaveMultiplier * getFineTune() / ivSampleFrequence; // constante

			float playPos0 = ivPlayPos;
			float endPlayPos =
				0.5f*c*aFrequence*ivPortamentoLength*ivPortamentoLength +
				(frequency0 - 0.5f)*c*ivPortamentoLength +
				playPos0;

				//ivPlayPos + 0.5f * aFrequence * (ivPortamentoLength*ivPortamentoLength) * c + ivPortamentoLength * frequency0 - 0.5f * ivPortamentoLength * c;
			if( unsigned int(endPlayPos) > modSampleLength )
			{
				float a2 = 0.5f * c * aFrequence;
				float a1 = (frequency0 - 0.5f) * c;
				float a0 = playPos0 - modSampleLength;

				float sqrtRoot = (float) sqrt( a1*a1 - 4 * a2 * a0 );
				float a2mul2 = 2 * a2;
				samplesToRender = (unsigned int)(( -a1 + sqrtRoot ) / a2mul2);
			}
			if( samplesToRender > portamentoSteps )
			{
				samplesToRender = portamentoSteps;
			}

			unsigned int p_1, p_2;
			ptStop = buffer->getData(0) + startFrom + samplesToRender;
			for( FP *pt=buffer->getData(0) + startFrom; pt < ptStop; pt++ )
			{
				p_1 = unsigned int(ivPlayPos);
				p_2 = p_1 + 1;

				ASSERT( p_1 < unsigned int( sampleLength ) );
				ASSERT( p_2 < unsigned int( sampleLength ) );
					
				(*pt) = ((ptData[p_2]-ptData[p_1]) * (ivPlayPos - p_1) + ptData[p_1]) * multiplyWith;
				
				ivPlayPos += ivPitch;
				ivFreq += aFrequence;
				ivPitch = ivFreq * c;
			}

			ivPortamentoStep += samplesToRender;
			if( ivPortamentoStep >= this->ivPortamentoLength )
				stopPortamento();*/
		}
		else
		{
			if( ivIsOn )
			{
				MWaveRenderer renderer;
				renderer.setWave( ivPtWave );

				if( getLooped() )
				{
					ivPlayPos =
						renderer.renderLooped(
							buffer,
							startFrom,
							stopAt,
							ivPlayPos,
							ivPitch,
							multiplyWith );
				}
				else
				{
					ivPlayPos =
						renderer.render(
							buffer,
							startFrom,
							stopAt,
							ivPlayPos,
							ivPitch,
							multiplyWith );

					if( ivPlayPos >= modSampleLength )
						reset();
				}
			}
		}
	}	
}

void MWaveOsc::setWave( MWave *wave )
{
	MSynchronize cls( &ivCriticalSection );

	if( ivPtWave )
		delete ivPtWave;
	ivPtWave = wave;
}

MWave* MWaveOsc::getWave()
{
	return ivPtWave;
}

void MWaveOsc::setOn(bool value)
{
	ivPlayPos = 0;
	ivIsOn = value;
}

bool MWaveOsc::getOn()
{
	return ivIsOn;
}

bool MWaveOsc::getReversed()
{
	return ivPtReverseControl->getValue();
}

void MWaveOsc::setReverse( bool value )
{
	ivPtReverseControl->setValue( value );
}

void MWaveOsc::setLooped( bool loop )
{
	ivPtLoopControl->setValue( loop );
}

bool MWaveOsc::getLooped()
{
	return ivPtLoopControl->getValue();
}

void MWaveOsc::loadFile( const char *fileName )
{	
	TRACE( "<Loading wave for oscilator: %s>\n", fileName );

	MSynchronize cls( &ivCriticalSection );

	if( getOn() )
		setOn( false );

	bool reversed = getReversed();
	if( reversed )
		reverse();

	try
	{
		ivPtWave->loadFile( fileName );
		if( ivPtWave->getChannelCount() == 2 )
			ivPtWave->makeMono();
		if( ivPtWave->getBitsPerSample() != 16 )
			ivPtWave->make16bit();
	}
	catch( MException anException )
	{
		ivPtWave->clear();
		setSampleName( "" );
		throw anException;
	}
	setSampleName( fileName );

	if( reversed )
		reverse();
}

void MWaveOsc::saveFile( const char *fileName )
{
	MSynchronize cls( &ivCriticalSection );

	if( getReversed() )
		reverse();
	try
	{
		ivPtWave->saveFile( fileName );
	}
	catch( MException e )
	{
		throw e;
	}
	if( getReversed() )
		reverse();
}

bool MWaveOsc::isValid()
{
	return
		ivPtWave &&
		ivPtWave->getData() &&
		ivPtWave->getDataLength(); 
}

String MWaveOsc::getSampleName()
{
	return ivPtSampleNameControl->getValue();
}

void MWaveOsc::setSampleName( String name )
{
	ivPtSampleNameControl->setValue( name );
}

String MWaveOsc::getName()
{
	return getRtti()->getClassName();
}

String MWaveOsc::getShortName()
{
	return "Wave";
}

int MWaveOsc::getViewId()
{
	return DESC_VIEW_WAVEOSC;
}

MTreeNode* MWaveOsc::save()	
{
	MTreeNode* ptBack = 0;
	MSynchronize cls( &ivCriticalSection );

	ptBack = new MTreeNode( getType() );
	ptBack->setAttribute( "value", getRtti()->getClassName() );

	// controls
	unsigned int count = getControlCount();
	for( unsigned int i=0;i<count;i++ )
		ptBack->addChild( getControl( i )->save() );

	// sample
	if( ivPtWave )
		ptBack->addChild( this->ivPtWave->save() );

	return ptBack;
}

void MWaveOsc::load( MTreeNode* node )
{
	MSynchronize cls( &ivCriticalSection );

	if( getOn() )
		setOn( false );

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
		else if( strcmp( nodeName, MObject::ELEM_OBJECT ) == 0 )
		{
			if( controlNode->getAttribute( MObject::ATTR_CLASS ) ==
				MWave::gvRtti.getClassName() )
			{
				ivPtWave->load( controlNode );
			}
		}
	}
}

void MWaveOsc::reverse()
{
	MSynchronize cls( &ivCriticalSection );

	if( isValid() )
	{
		unsigned int samples = ivPtWave->getDataLength()/sizeof(MW_SAMPLETYPE);
		MW_SAMPLETYPE *buffer = new MW_SAMPLETYPE[samples];	
		MW_SAMPLETYPE *source = (MW_SAMPLETYPE*) (ivPtWave->getData() + ivPtWave->getDataLength());
		
		for(unsigned int i=0;i<samples;i++)
			buffer[i] = *(--source);
				
		ivPtWave->setData( (unsigned char*) buffer, ivPtWave->getDataLength() );

		ivIsReversed = ! ivIsReversed;
	}
}

IRtti* MWaveOsc::getRtti() const
{
	return &gvRtti;
}

MObject* MWaveOsc::createInstance()
{
	return new MWaveOsc();
}