#include "MNotePiano.h"
#include "../../../model/interfaces/MSoundBufferUtils.h"
#include <gui/dialog/MMessageBox.h>
#include <gui/win32impl/MTopWndWin32Impl.h>
#include <gui/MTopWnd.h>

MNotePiano::MNotePiano( Synth* ptSynth )
	: MyInteractiveWnd(),
	ivPtPianoRenderer( 0 ),
	ivPtStreamingEngine( 0 ),
	ivPtSynth( ptSynth ),
	ivOctaveCount( 8 ),
	ivLastNote( 0 ),
	ivPtStreamImpl( 0 )
{
	ivBkColor = MColorMap::create( 255, 255, 255 );
	ivPtPianoRenderer = new MNotePianoRenderer();
	ivPtStreamImpl = new MNotePiano::MAudioStream( this );

	setSize( MSize(
		ivPtPianoRenderer->GetPianoSize().getWidth(),
		ivOctaveCount * ivPtPianoRenderer->GetPianoSize().getHeight() ) );
}

MNotePiano::~MNotePiano()
{
	SAFE_DELETE( ivPtStreamImpl );
	SAFE_DELETE( ivPtStreamingEngine );
	SAFE_DELETE( ivPtPianoRenderer );
}

void MNotePiano::setParent( MWnd* pParent )
{
	MWnd::setParent( pParent );

	/*if( pParent->getTopParent() && pParent->getTopParent()->getInterface( "MTopWnd" ) )
	{
		ivPtStreamingEngine = new MAudioEngine( ((MTopWnd*)pParent->getTopParent()) );

		try
		{
			ivPtStreamingEngine->create( ivPtStreamImpl );
		}
		catch( MException ae )
		{
			MMessageBox::showError( ae.getExceptionDescripton() );
			exit( -1 );
		}
	}
	else
		MLogger::logError( "no top parent in MNotePiano" );*/
}

void MNotePiano::paint( IGraphics* ptGraphics, const MRect &_rect )
{
	MPoint target = getScrollPos();

	ptGraphics->fillRect( _rect, ivBkColor );

	MSize keyBoardSize = ivPtPianoRenderer->GetPianoSize();
	
	ptGraphics->setOffset( ptGraphics->getOffset() - target );
	int textCount = ivOctaveCount;
	for( int i=0;i<ivOctaveCount;i++ )
	{
		int noteCount = ivOctaveCount * 12;
		int inverse = (noteCount-(ivLastNote+1));
		int activeOctave = inverse / 12;
		if( getMouseDownLeft() && activeOctave == i )
		{
			int noteIndex = inverse % 12; 
			ivPtPianoRenderer->paint( keyBoardSize, ptGraphics, 0, i * ivPtPianoRenderer->GetPianoSize().getHeight(), --textCount, noteIndex );
		}
		else
			ivPtPianoRenderer->paint( keyBoardSize, ptGraphics, 0, i * ivPtPianoRenderer->GetPianoSize().getHeight(), --textCount );
	}
	ptGraphics->setOffset( ptGraphics->getOffset() + target );
}

void MNotePiano::onMouseDown( MMouseEvent* anEvent )
{
	MyInteractiveWnd::onMouseDown( anEvent );
	if( anEvent->getButton() == MMouseEvent::BUTTON1 && ivPtStreamingEngine )
	{
		ivLastNote = getNoteValue( anEvent->getPoint() );
		ivNote.setValue( ivLastNote );
		ivNote.setVolume( 1.0 );
		ivPtSynth->processEvent( &MEvent( MEvent::RESET ) );
		ivPtSynth->processEvent( &MEventNote( MEvent::NOTE_ON, &ivNote ) );
		ivPtStreamingEngine->start();
		repaint();
	}
}

void MNotePiano::onMouseUp( MMouseEvent* anEvent )
{
	if( anEvent->getButton() == MMouseEvent::BUTTON1 && ivPtStreamingEngine )
	{
		ivPtSynth->processEvent( &MEventNote( MEvent::NOTE_OFF, &ivNote ) );
		ivPtStreamingEngine->stop();
		repaint();
	}
	MyInteractiveWnd::onMouseUp( anEvent );
}

void MNotePiano::onMouseMove( MMouseEvent* anEvent )
{
	MyInteractiveWnd::onMouseMove( anEvent );
	if( getMouseDownLeft() )
	{
		if( getNoteValue( anEvent->getPoint() ) != ivLastNote )
		{
			ivLastNote = getNoteValue( anEvent->getPoint() );
			ivNote.setValue( ivLastNote );
			ivNote.setVolume( 1.0 );
			MEventNote anEvent( MEvent::NOTE_ON, &ivNote );
			ivPtSynth->processEvent( &anEvent );
			repaint();
		}
	}
}

unsigned int MNotePiano::getNoteValue( MPoint point )
{
	return ( ivOctaveCount * 12 ) - ( (point.getY()+getScrollPos().getY()) / ( ivPtPianoRenderer->GetPianoSize().getHeight() / 12 ) ) - 1;
}

MNotePiano::MAudioStream::MAudioStream( MNotePiano* ptPiano ) :
	ivPtPiano( ptPiano )
{
}

MNotePiano::MAudioStream::~MAudioStream()
{
}

/**
* this method will be invoked by the IAudioEngine to 
* a streaming source to fill up the buffer.
*/
void MNotePiano::MAudioStream::goNext( MSoundBuffer* pBuffer, unsigned int start, unsigned int stop )
{
	MSoundBuffer tmp( 1, pBuffer->getDataLength() );
	ivPtPiano->ivPtSynth->goNext( &tmp, start, stop );
	MSoundBufferUtils::pasteBuffer( pBuffer, &tmp, 0, pBuffer->getDataLength() );
}

void MNotePiano::MAudioStream::processEvent( MEvent* pEvent )
{
}