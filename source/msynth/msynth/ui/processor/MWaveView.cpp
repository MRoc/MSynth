#include "MWaveView.h"
#include "../../../MSynthConstants.h"

/**
 * constructor
 */
MWaveView::MWaveView() :
	MWnd(),
	ivPtWave( 0 ),
	ivPtPeakFile( 0 ),
	ivType( MWaveView::PEAK )
{
}

/**
 * destructor
 */
MWaveView::~MWaveView()
{
	SAFE_DELETE( ivPtPeakFile );
}

/**
 * paints the wave
 */
void MWaveView::paint( IGraphics* ptGraphics, const MRect &rect )
{
	ptGraphics->fillRect( rect, MColorMap::BK_COLOR5 );

	ivType = this->ivPtPeakFile != 0 ? PEAK : SAMPLING;

	if( ivPtWave != 0 &&
		ivPtWave->getData() != 0 )
	{
		unsigned int width = rect.getWidth();
		unsigned int height = rect.getHeight();
		unsigned int halfHeight = height / 2;
		unsigned int samples = ivPtWave->getDataLength() / sizeof( MW_SAMPLETYPE );

		switch( ivType )
		{
		case MWaveView::SAMPLING:
			{
				FP div = (FP)samples / (FP)width;
				int oldValue = halfHeight;
				for( unsigned int i=0;i<width;i++ )
				{
					unsigned int index = (unsigned int)(div * i);
					FP sample = (FP) ((MW_SAMPLETYPE*)ivPtWave->getData())[ index ];
					int value = halfHeight - (int) (( sample / FP(MW_MAX) ) * halfHeight);
					ptGraphics->drawLine( i, oldValue, i+1, value, MColorMap::FG_COLOR2 );
					oldValue = value;
				}
			}
			break;
		case MWaveView::PEAK:
			{
				for( unsigned int i=0;i<width;i++ )
				{
					unsigned int indexMax = i * 2;
					unsigned int indexMin = i * 2 + 1;

					// fetch peaks
					FP sampleMax = (FP)(signed char)ivPtPeakFile->getData( 0 )->get( indexMax );
					FP sampleMin = (FP)(signed char)ivPtPeakFile->getData( 0 )->get( indexMin );

					TRACE( "sampleMax: %f sampleMin: %f\n", sampleMax, sampleMin );
					FP sampleMaxF = sampleMax / 127.0f;
					FP sampleMinF = sampleMin / 127.0f;

					int y1 = (int) ( halfHeight - halfHeight * sampleMaxF );
					int y2 = (int) ( halfHeight - halfHeight * sampleMinF );
					if( y1 == y2 )
						y2++;

					ptGraphics->drawLine( i, y1, i, y2, MColorMap::FG_COLOR2 );
				}
			}
			break;
		};
	}
}

/**
 * sets the wave used for painting
 */
void MWaveView::setWave( MWave* ptWave )
{
	ivPtWave = ptWave;
}

/**
 * returns the wave used for painting
 */
MWave* MWaveView::getWave()
{
	return ivPtWave;
}

/**
 * updates the peak file data from the wave file
 * and does a repaint.
 */
void MWaveView::update()
{
	if( this->ivPtWave && ivPtWave->getDataLength() > 0 && ivPtWave->getDataLength()/2 > (unsigned int)getWidth() )
	{
		generatePeakFile();
	}
	else
		SAFE_DELETE( ivPtPeakFile );

	repaint();
}

/**
 * generates a peakfile for the current set wave.
 * it generates as many peaks as the window's width in pixel.
 */
void MWaveView::generatePeakFile()
{
	if( ivPtPeakFile == 0 )
		ivPtPeakFile = new MPeakFile();
	else
		ivPtPeakFile->reset();

	ivPtPeakFile->analyzeMono(
		ivPtWave->getData(),
		ivPtWave->getDataLength(),
		getWidth() );
}