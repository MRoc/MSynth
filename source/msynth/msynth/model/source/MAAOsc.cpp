#include "MAAOsc.h"
#include "../MWave.h"
#include <framework/resource/MResourceManager.h>
#include <gui/dialog/MMessageBox.h>
#include "../interfaces/MSoundBufferUtils.h"

/**
 * string for mode selection
 */
const char* MAAOsc::MODE_STRINGS[] =
{
	"*",
	"-",
	"0"
};

/**
 * values for mode selection
 */
const int MAAOsc::MODE_VALUES[] =
{
	0,
	1,
	2
};

/**
 * constructor
 */
MWavetable::MWavetable() :
	ivPlayPos( 0 ),
	ivPitch( 0 ),
	ivSampleFrequence( 220 )
{
	try
	{
		load( "resource/wave/triangle.wav" );
	}
	catch( ... )
	{
		MLogger::logError( "MWavetable::MWavetable: error loading default wave" );
	}
}

/**
* destructor
*/
MWavetable::~MWavetable()
{
}

/**
* string filename
*/
void MWavetable::load( String fileName )
{
	MSynchronize sync( &ivSync );

	MWave wave;
	ISeekableInputStream* pIn = MResourceManager::getInstance()->openReadFile( fileName );
	if( pIn )
	{
		wave.loadFile( pIn );

		if( wave.getChannelCount() != 1 )
			wave.makeMono();

		ivPlayPos = 0;
		ivWaveData.setLength( wave.getDataLength() / sizeof( MW_SAMPLETYPE ) );

		MW_SAMPLETYPE* ptSrc = (MW_SAMPLETYPE*) wave.getData();
		FP* pDest = ivWaveData.getData();
		FP* pDestEnd = ivWaveData.getData() + ivWaveData.getDataLength();
		for(
			FP* pt = pDest;
			pt < pDestEnd;
			pt++, ptSrc++ )
		{
			(*pt) = ((FP)*ptSrc) / MW_MAX;
		}
	}
	MResourceManager::getInstance()->close( pIn );
}

/**
* returns the sample data
*/
FP* MWavetable::getData()
{
	return ivWaveData.getData();
}

/**
* returns the sample data length
*/
unsigned int MWavetable::getDataLength()
{
	return ivWaveData.getDataLength();
}

/**
 * resets state
 */
void MWavetable::reset()
{
}

/**
 * sets the frequency
 */
void MWavetable::setFreq(FP freq)
{
	ivSampleFrequence = freq;
	TRACE( "f: %f\n", ivSampleFrequence );
	ASSERT( ivSampleFrequence < MW_SAMPLINGRATE / 2 );
}

/**
 * adds pitch
 */
void MWavetable::addPitch(FP delta)
{
	ivSampleFrequence += delta;
	ASSERT( ivSampleFrequence < MW_SAMPLINGRATE / 2 );
}

/**
 * sets the phase (0.0-1.0)
 */
void MWavetable::setPhase( FP phase )
{
	ivPlayPos = phase * getDataLength();
}

/**
 * renders the osc
 */
void MWavetable::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt )
{
	MSynchronize sync( &ivSync );

	unsigned int dLen = getDataLength();
	ivPitch = ( ivSampleFrequence / MW_SAMPLINGRATE ) * dLen;
	//TRACE( "PITCH: %f\n", ivPitch );

	FP* pSrc = ivWaveData.getData();

	FP* pDest = buffer->getData( 0 ) + startFrom;
	FP* pDestEnd = pDest + ( stopAt - startFrom );

	for( FP* pt = pDest; pt<pDestEnd; pt++ )
	{
		if( ivPlayPos >= dLen-1 )
			ivPlayPos -= dLen-1;

		unsigned int index = (unsigned int)ivPlayPos;
		unsigned int indexNext = index + 1;
		(*pt) = ((pSrc[ indexNext ]-pSrc[ index ]) * (ivPlayPos - index) + pSrc[ index ]);

		ivPlayPos += ivPitch;
	}
}


/** 
 * the runtime type info
 */
MRtti MAAOsc::gvRtti = MRtti( "MAAOsc", MAAOsc::createInstance );

/**
 * constructor
 */
MAAOsc::MAAOsc() :
	ivPtModeControl( 0 )
{
	try
	{
		IFolder* pFolder = MResourceManager::getInstance()->getFolder( "resource/wave" );
		if( pFolder )
		{
			IntList indeces;
			unsigned int fileCount = pFolder->getFileCount();
			for( unsigned int i=0;i<fileCount;i++ )
			{
				ivOscFileNames.push_back( pFolder->getFile( i ) );
				indeces.push_back( i );
			}

			ivPtWave1Control = new MDiscreteComboControl( String( "wave1" ), indeces, ivOscFileNames, 0 );			
			ivPtWave1Control->addControlListener( this );
			ivPtWave2Control = new MDiscreteComboControl( String( "wave2" ), indeces, ivOscFileNames, 0 );
			ivPtWave2Control->addControlListener( this );
		}
		ivWaveTable1.load( "resource/wave/saw.wav" );
		ivWaveTable2.load( "resource/wave/saw.wav" );
	}
	catch( ... )
	{
		MMessageBox::showError( "what the fuck" );
	}
	ivPtPhaseControl->addControlListener( this );

	ivPtModeControl = new MDiscreteComboControl( String( "Mode" ), MODE_VALUES, MODE_STRINGS, 3, 0 );
}

/**
 * destructor
 */
MAAOsc::~MAAOsc()
{
	ivPtPhaseControl->removeControlListener( this );

	SAFE_DELETE( ivPtModeControl );

	if( ivPtWave1Control )
	{
		ivPtWave1Control->removeControlListener( this );
		delete ivPtWave1Control;
	}
	if( ivPtWave2Control )
	{
		ivPtWave2Control->removeControlListener( this );
		delete ivPtWave2Control;
	}
}

/**
 * returns the number of embedded controls
 */
unsigned int MAAOsc::getControlCount()
{
	return 4;
}

/**
 * returns the indexed control
 */
IControl* MAAOsc::getControl( unsigned int index )
{
	if( index == 0 )
		return ivPtPhaseControl;
	else if( index == 2 )
		return ivPtModeControl;
	else if( index == 1 )
		return ivPtWave1Control;
	else if( index == 3 )
		return ivPtWave2Control;
	else
		return 0;
}

/**
 * renders the osc
 */
void MAAOsc::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt )
{
	MSynchronize sync( &ivSync );

	unsigned int samples = stopAt - startFrom;
	if( ivPtModeControl->getValue() == 0 )
	{
		MSoundBuffer tmp1( 1, stopAt - startFrom );
		ivWaveTable1.goNext( &tmp1, startFrom, stopAt );

		MSoundBuffer tmp2( 1, stopAt - startFrom );
		ivWaveTable2.goNext( &tmp2, startFrom, stopAt );
		for( unsigned int i=0;i<samples;i++ )
			buffer->getData( 0 )[ startFrom + i ] = tmp1.getData( 0 )[ i ] * tmp2.getData( 0 )[ i ];
	}
	else if( ivPtModeControl->getValue() == 1 )
	{
		MSoundBuffer tmp1( 1, stopAt - startFrom );
		ivWaveTable1.goNext( &tmp1, startFrom, stopAt );

		MSoundBuffer tmp2( 1, stopAt - startFrom );
		ivWaveTable2.goNext( &tmp2, startFrom, stopAt );
		for( unsigned int i=0;i<samples;i++ )
			buffer->getData( 0 )[ startFrom + i ] = tmp1.getData( 0 )[ i ] - tmp2.getData( 0 )[ i ];
	}
	else if( ivPtModeControl->getValue() == 2 )
	{
		MSoundBuffer tmp1( 1, stopAt - startFrom );
		ivWaveTable1.goNext( &tmp1, startFrom, stopAt );
		for( unsigned int i=0;i<samples;i++ )
			buffer->getData( 0 )[ startFrom + i ] = tmp1.getData( 0 )[ i ];
	}
}

/**
 * resets state
 */
void MAAOsc::reset()
{
	ivWaveTable1.reset();
	ivWaveTable2.reset();
}

/**
 * sets the frequency
 */
void MAAOsc::setFreq(FP freq)
{
	ivWaveTable1.setFreq( freq );
	ivWaveTable2.setFreq( freq/2 );
}

/**
 * adds pitch
 */
void MAAOsc::addPitch(FP delta)
{
	ivWaveTable1.addPitch( delta );
	ivWaveTable2.addPitch( delta );
}

/**
 * returns the osc's name
 */
String MAAOsc::getName()
{
	return "AAOsc";
}

/**
 * returns the osc's short name
 */
String MAAOsc::getShortName()
{
	return "AAOsc";
}

/**
 * returns the runtime type info
 */
IRtti* MAAOsc::getRtti() const
{
	return &gvRtti;
}

/**
 * creates an instance of this class
 */
MObject* MAAOsc::createInstance()
{
	return new MAAOsc();
}

/**
 * inheritanced from IControlListener
 */
void MAAOsc::valueChanged( MControlListenerEvent *anEvent )
{
	MSynchronize sync( &ivSync );

	if(anEvent->getSource() == ivPtPhaseControl )
	{
		ivWaveTable1.setPhase( 0.0 );
		ivWaveTable2.setPhase( ivPtPhaseControl->getValue() );
	}
	else if( anEvent->getSource() == ivPtWave1Control )
	{
		try
		{
			ivWaveTable1.load( "resource/wave/" + ivOscFileNames[ ivPtWave1Control->getValue() ] );
		}
		catch( ... )
		{
			MMessageBox::showError( "could not load wavetable" );
		}
	}
	else if( anEvent->getSource() == ivPtWave2Control )
	{
		try
		{
			ivWaveTable2.load( "resource/wave/" + ivOscFileNames[ ivPtWave2Control->getValue() ] );
		}
		catch( ... )
		{
			MMessageBox::showError( "could not load wavetable" );
		}
	}
}