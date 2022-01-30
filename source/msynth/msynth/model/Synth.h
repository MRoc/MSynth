/**
 * (C) 2000-2003 MRoc
 */

#ifndef __Synth
#define __Synth

#pragma warning(disable:4786)

#include <framework/list/MSelectableObjectList.h>
#include <framework/sync/MSynchronize.h>

#include "source/MOsc.h"
#include "transformer/MTransformer.h"
#include "transformer/MTransformerRegistry.h"
#include "mixer/MMixerChannel.h"
#include "note/MDefaultSynthNoteList.h"
#include "interfaces/IDescriptorCollection.h"
#include "interfaces/MInstrument.h"
#include "interfaces/MSoundBuffer.h"
#include "file/MFileHandling.h"
#include "converter/MXMLConverter.h"
#include "automation/IAutomationContext.h"
#include "automation/IAutomationTracker.h"

#include "../../resource.h"

class Synth :
	public MInstrument
{
public:

	/**
	 * types for a instrument event
	 * triggered by a synth
	 */
	enum InstrumentEvent
	{
		MSG_OSC_CHANGED,
		MSG_TRANSFORMER_ADDED,
		MSG_TRANSFORMER_REMOVED,
		MSG_TRANSFORMER_SWAPPED
	};

	static const char* CLASS_NAME;

	static const int		TYPE_ID;
	static int				SYNTH_COUNT;
	static char*			gvNotesMatrixHuman[8*12];

protected:

	static bool				gvNoteMatrixInitialized;
	static FP				gvNoteMatrix[8*12];
	static FP				gvOctave[12];

	bool					ivIsNoteOn;

	FP						ivMaxAmp;
	MOsc*					ivPtOsc;
	MSelectableObjectList	ivTransformers;

	MCriticalSection		ivCriticalSection;

public:

	Synth();
	virtual ~Synth();

	virtual String toString();

	virtual IControlCollection* getChild( unsigned int index );
	virtual unsigned int	getChildCount();

	void					open( const char* fileName );
	void					save( const char* fileName );

	// MInstrument
	virtual void			processEvent( MEvent* ptEvent );
	virtual void			goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int endAt );

	virtual void			processEventChilds( MEvent* ptEvent );

	virtual unsigned int	getChannelCount();
	virtual int				getNoteListId();
	virtual bool			noteListNeedsListener();
	virtual int				getEditorId();

	virtual int				getBitmapId( int state );

	// Transformers
	void					setSoundSource( MOsc *source );

	void					addTransformer( MTransformer *transformer );
	void					removeSelectedTransformer();
	void					transformerUp();
	void					transformerDown();

	void					removeTransformer( int index );
	int						getTransformerIndex( MTransformer *transformer );
	MSelectableObjectList*	getTransformers();

	// IDescriptorCollection
	virtual String			getName();
	virtual unsigned int	getTransformerCount();
	virtual IDescriptor*	getTransformer( unsigned int index );
	virtual IDescriptor*	getOsc();

	/**
	 * loads from the given tree node
	 */
	virtual void load( MTreeNode* ptNode );

	/**
	 * stores as tree node
	 */
	virtual MTreeNode* save();

private:

	void					swapTransformer( int index1, int index2 );

	static void				initNoteMatrix();
};

#endif