#ifndef __MWaveRenderer
#define __MWaveRenderer

#include "MWave.h"
#include "interfaces/MSoundBuffer.h"

class MWaveRenderer
{
protected:

	/**
	 * the wave to be renderer
	 */
	MWave* ivPtWave;

	/**
	 * the play position before the dot
	 */
	unsigned int ivPlayPosBDot;

	/**
	 * the play position after the dot
	 */
	unsigned int ivPlayPosADot;

	/**
	 * the delta to be added for every step
	 * in the soundbuffer before the dot
	 */
	unsigned int ivDeltaBDot;

	/**
	 * the delta to be added for every step
	 * in the soundbuffer after the dot
	 */
	unsigned int ivDeltaADot;

	/**
	 * the play position base for after the dot
	 */
	unsigned int ivADotBase;

public:

	/**
	 * constructor
	 */
	MWaveRenderer();

	/**
	 * destructor
	 */
	virtual ~MWaveRenderer();

	/**
	 * sets the wave
	 */
	virtual void setWave( MWave* ptWave );

	/**
	 * renders the wave into the given soundbuffer.
	 */
	virtual FP render(
		MSoundBuffer* ptBuffer,
		unsigned int begin,
		unsigned int stop,
		FP beginInWave,
		FP stepWidth,
		FP multi );

	/**
	 * renders the wave looped,
	 * returns the playposition in wave
	 */
	virtual FP renderLooped(
		MSoundBuffer* ptBuffer,
		unsigned int begin,
		unsigned int stop,
		FP beginInWave,
		FP stepWidth,
		FP multi );

protected:

	/**
	 * sets the before and after dot play pos
	 */
	__forceinline void setPlayPos( FP playPos );

	/**
	 * sets the before and after dot delta stepwidth
	 */
	__forceinline void setStepWidth( FP stepWidth );
};

#endif