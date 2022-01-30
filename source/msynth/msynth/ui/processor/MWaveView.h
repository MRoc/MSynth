#ifndef __MWAVEVIEW
#define __MWAVEVIEW

#include <framework/MTypes.h>
#include <gui/MWnd.h>
#include "../../model/MWave.h"
#include "../../../resource.h"

#include "../../model/audiochannel/MPeakFile.h"

/**
 *
 * a wave view with two available modes
 * drawing a wave:
 *
 * sampled or using a peakfile
 *
 */
class MWaveView :
	public MWnd
{
protected:

	/**
	 * available rendering modes
	 */
	enum PaintingType
	{
		SAMPLING,
		PEAK
	};

	/**
	 * the painting type used
	 */
	PaintingType ivType;

	/**
	 * the wave file to ge painted
	 */
	MWave* ivPtWave;

	/**
	 * the peak file used for painting,
	 * if null, the sampling painting algo
	 * is used.
	 */
	MPeakFile* ivPtPeakFile;

public:

	/**
	 * constructor
	 */
	MWaveView();

	/**
	 * destructor
	 */
	virtual ~MWaveView();

	/**
	 * paints the wave
	 */
	virtual void paint( IGraphics* ptGraphics, const MRect &rect );

	/**
	 * sets the wave used for painting
	 */
	virtual void setWave( MWave* ptWave );

	/**
	 * returns the wave used for painting
	 */
	virtual MWave* getWave();

	/**
	 * updates the peak file data from the wave file
	 * and does a repaint.
	 */
	virtual void update();

protected:

	/**
	 * generates a peakfile for the current set wave.
	 * it generates as many peaks as the window's width in pixel.
	 */
	virtual void generatePeakFile();
};

#endif