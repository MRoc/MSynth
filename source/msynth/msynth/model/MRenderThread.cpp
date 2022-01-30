#include "MRenderThread.h"
#include <gui/dialog/MMessageBox.h>

MRenderThread::MRenderThread( String fileName, MTimeController* ptController, Sequencer* ptSequencer, MSong* ptSong ) :
	MThread(),
	ivFileName( fileName ),
	ivPtTimeControl( ptController ),
	ivPtSequencer( ptSequencer ),
	ivPtSong( ptSong ),
	ivPtProgress( 0 )
{
}

MRenderThread::~MRenderThread()
{
}

void MRenderThread::run()
{
	int left = ivPtTimeControl->getLeftMarker();
	int right = ivPtTimeControl->getRightMarker();
	int start = left < right ? left : right;
	int stop = left > right ? left : right;
	
	unsigned int samplesToRender = ivPtSequencer->getStepsPerBar() * ivPtSequencer->getStepwidth() * ( stop - start + 1 );
	unsigned int bufferLength = samplesToRender / 100;

	MSoundBuffer buffer( 2, bufferLength );
	MW_SAMPLETYPE* pOut = new MW_SAMPLETYPE[ bufferLength * 2 ];

	MSynchronize cls( & (ivPtSong->ivCriticalSection) );

	WaveFileHandling out;
	out.setFormat(
		44100,
		16,
		2 );
	try
	{
		ivPtTimeControl->setCurrentPatternEx( start );
		out.openFile( ivFileName );
		if( ivPtProgress )
			ivPtProgress->onProgress( 0.0f );
		for( unsigned int i=0;i<100;i++ )
		{
			ivPtSequencer->goNext( &buffer, 0, bufferLength );
			buffer.export2pcm16bit2channel( pOut );
			out.writeData( (unsigned char*) pOut, bufferLength * sizeof(MW_SAMPLETYPE) * 2 );
			if( ! ivPtProgress )
				break;
			else
				ivPtProgress->onProgress( ((float)(i+1)) / 100.0f );
		}
		if( ivPtProgress )
			ivPtProgress->onProgress( 1.0f );
		out.finalizeFile();
		out.closeFile();
	}
	catch( MException ae )
	{
		MMessageBox::showError(
			"Error rendering. Perhaps the file to be written writen opened exclusively by another app" );
	}

	if( pOut )
		delete pOut;

	ivIsRunning = false;
	if( ivPtProgress )
		ivPtProgress->onFinished();
}

void MRenderThread::setProgressCallback( IProgress* ptProgress )
{
	this->ivPtProgress = ptProgress;
}

IProgress* MRenderThread::getProgressCallback()
{
	return this->ivPtProgress;
}