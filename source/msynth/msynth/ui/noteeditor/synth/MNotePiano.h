/*
	(C)  2000-2002 MRoc hifiShock

	The piano in the synth note frame, should be able to play a note
*/

#ifndef __MNotePiano
#define __MNotePiano

#include "MNotePianoRenderer.h"
#include <gui/scrollbar/MSimpleScrollWnd.h>
#include "../../../model/streamingengine/MAudioEngine.h"
#include "../../../model/Synth.h"

class MNotePiano :
	public MyInteractiveWnd
{
protected:

	class MAudioStream;

	int					ivOctaveCount;
	MNotePianoRenderer	*ivPtPianoRenderer;
	MAudioEngine*		ivPtStreamingEngine;
	Synth*				ivPtSynth;
	unsigned int		ivLastNote;
	MDefaultSynthNote	ivNote;
	MAudioStream*		ivPtStreamImpl;

public:

	MNotePiano( Synth* ptSynth );
	virtual ~MNotePiano();

	virtual void paint( IGraphics* ptGraphics, const MRect &rect );
	virtual void setParent( MWnd* pParent );
	unsigned int getNoteValue( MPoint point );

protected:

	virtual void onMouseDown( MMouseEvent* anEvent );
	virtual void onMouseUp( MMouseEvent* anEvent );
	virtual void onMouseMove( MMouseEvent* anEvent );

	friend class MAudioStream;
	class MAudioStream :
		public IProcessor
	{
	protected:
		MNotePiano* ivPtPiano;
	public:
		MAudioStream( MNotePiano* ivPtPiano );
		virtual ~MAudioStream();
		virtual void goNext( MSoundBuffer* pBuffer, unsigned int start, unsigned int stop );
		virtual void processEvent( MEvent* pEvent );
	};
};

#endif
