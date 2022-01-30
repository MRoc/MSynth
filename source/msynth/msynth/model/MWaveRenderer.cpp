#include "mwaverenderer.h"

/**
 * constructor
 */
MWaveRenderer::MWaveRenderer() :
	ivPtWave( 0 ),
	ivPlayPosBDot( 0 ),
	ivPlayPosADot( 0 ),
	ivDeltaBDot( 1 ),
	ivDeltaADot( 0 ),
	ivADotBase( 65536 /* 2^16 */ )
{
}

/**
 * destructor
 */
MWaveRenderer::~MWaveRenderer()
{
}

/**
 * sets the wave
 */
void MWaveRenderer::setWave( MWave* ptWave )
{
	ivPtWave = ptWave;
}

/**
 * renders the wave into the given soundbuffer.
 */
FP MWaveRenderer::render(
	MSoundBuffer* ptBuffer,
	unsigned int start,
	unsigned int stop,
	FP beginInWave,
	FP stepWidth,
	FP multi )
{
	ASSERT( ptBuffer );
	ASSERT( start <= stop );

	unsigned int samplesToRender = stop - start;
	unsigned int sampleLenMod = ivPtWave->getDataLength() / sizeof( MW_SAMPLETYPE ) - 1;
	unsigned int stepsLeftInWave = unsigned int(( sampleLenMod - beginInWave ) / stepWidth);
	ASSERT( stepsLeftInWave >= 0 );
	if( samplesToRender > stepsLeftInWave )
		samplesToRender = stepsLeftInWave;
	
	setPlayPos( beginInWave );
	setStepWidth( stepWidth );

	unsigned int playposNext;
	MW_SAMPLETYPE* ptData = (MW_SAMPLETYPE*) ivPtWave->getData();
	FP* ptStart = ptBuffer->getData( 0 ) + start;
	FP* ptStop = ptStart + samplesToRender;
	FP* pt = ptStart;

	for( ; pt < ptStop ; pt++ )
	{
		// interpolate between playpositionBeforeDot and playpositionBeforeDot + 1
		// with help of playPositionAfterDot and dotBase
		playposNext = ivPlayPosBDot + 1;

		// assert last read position lies before data end
		ASSERT( playposNext < ivPtWave->getDataLength() / 2 );

		(*pt) = ( ptData[ivPlayPosBDot] + ( ptData[playposNext] - ptData[ivPlayPosBDot] ) * (FP(ivPlayPosADot)/FP(ivADotBase) ) ) * multi;

		// calculate position after dot
		ivPlayPosADot += ivDeltaADot;

		// add rest if nescessary
		if( ivPlayPosADot >= ivADotBase )
		{
			ivPlayPosBDot++;
			ivPlayPosADot -= ivADotBase;
		}

		// calculate position before dot
		ivPlayPosBDot += ivDeltaBDot;
	}

	return
		FP(ivPlayPosBDot) + FP(ivPlayPosADot)/FP(ivADotBase);
}

/**
 * renders the wave looped,
 * returns the playposition in wave
 */
FP MWaveRenderer::renderLooped(
	MSoundBuffer* ptBuffer,
	unsigned int start,
	unsigned int stop,
	FP beginInWave,
	FP stepWidth,
	FP multi )
{
	ASSERT( ptBuffer );
	ASSERT( start <= stop );

	unsigned int samplesToRender = stop - start;
	
	setPlayPos( beginInWave );
	setStepWidth( stepWidth );

	unsigned int sampleLenMod = ivPtWave->getDataLength() / sizeof( MW_SAMPLETYPE ) - 1;

	unsigned int positionNext;
	MW_SAMPLETYPE* ptData = (MW_SAMPLETYPE*) ivPtWave->getData();
	FP* ptStart = ptBuffer->getData( 0 ) + start;
	FP* ptStop = ptBuffer->getData( 0 ) + stop;
	FP* pt = ptStart;

	for( ; pt < ptStop ; pt++ )
	{
		// interpolate between playpositionBeforeDot and playpositionBeforeDot + 1
		// with help of playPositionAfterDot and dotBase
		positionNext = ivPlayPosBDot + 1;

		// assert last read position lies before data end
		ASSERT( positionNext < ivPtWave->getDataLength() / 2 );

		(*pt) = ( ptData[ivPlayPosBDot] + ( ptData[positionNext] - ptData[ivPlayPosBDot] ) * (FP(ivPlayPosADot)/FP(ivADotBase) ) ) * multi;

		// calculate position after dot
		ivPlayPosADot += ivDeltaADot;

		// add rest if nescessary
		if( ivPlayPosADot >= ivADotBase )
		{
			ivPlayPosBDot++;
			ivPlayPosADot -= ivADotBase;
		}

		// calculate position before dot
		ivPlayPosBDot += ivDeltaBDot;

		// check is end of sample reached
		if( ivPlayPosBDot >= sampleLenMod )
		{
			ivPlayPosBDot = 0;
			ivPlayPosADot = 0;
		}
	}

	return
		FP(ivPlayPosBDot) + FP(ivPlayPosADot)/FP(ivADotBase);
}

/**
 * sets the before and after dot play pos
 */
void MWaveRenderer::setPlayPos( FP playPos )
{
	this->ivPlayPosBDot = (unsigned int) ( playPos );
	this->ivPlayPosADot = (unsigned int) ( ( playPos - ivPlayPosBDot ) * ivADotBase );
}

/**
 * sets the before and after dot delta stepwidth
 */
void MWaveRenderer::setStepWidth( FP stepWidth )
{
	this->ivDeltaBDot = (unsigned int) ( stepWidth );
	this->ivDeltaADot = (unsigned int) ( ( stepWidth - ivDeltaBDot ) * ivADotBase );
}