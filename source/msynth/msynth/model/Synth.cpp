/*------------------------------------------------------------------------------------

	Synth (C)2000/2001 MRoc hifiShock

	v.0.1				->	design
	v.0.2				->	ADSR-Envelope as extern-modul
	v.0.3				->	Oscilators, Filters and Clipper as extern-moduls
	v.0.3b 14.12.00		->	MEnvFilters, public ptEnv -> protected ptEnv + new methods
	v.0.4				->	linear modular model
	v.0.4b				->	inherited from MInstrument
	v.0.5  13.08.01		->	ThreadSafe (CriticalSection)
	v.0.51				->	Cleanup
	v.0.52				->	storing transformers in MSelectableObjectList

------------------------------------------------------------------------------------*/

#include "Synth.h"
#include <framework/performance/MClock.h>
#include <framework/io/MLogger.h>
#include <gui/dialog/MMessageBox.h>

const int Synth::TYPE_ID = 1;
int Synth::SYNTH_COUNT = 0;

const char*	Synth::CLASS_NAME = "Synth";

bool Synth::gvNoteMatrixInitialized = false;
char* Synth::gvNotesMatrixHuman[8*12];
FP Synth::gvNoteMatrix[8*12];
FP Synth::gvOctave[12];

#define PORTAMENTO_SPEED 4410

//-----------------------------------------------------------------------------
// + construction
//-----------------------------------------------------------------------------
Synth::Synth() : 
	ivIsNoteOn( false ),
	ivMaxAmp( 1.0f ),
	ivPtOsc( 0 )
{ 
	SYNTH_COUNT++;
	setChannelName( "Synth" + MUtil::toString( SYNTH_COUNT ) );

	if( ! gvNoteMatrixInitialized )
	{
		initNoteMatrix();
		gvNoteMatrixInitialized = true;
	}
};

//-----------------------------------------------------------------------------
// - initNoteMatrix
//-----------------------------------------------------------------------------
void Synth::initNoteMatrix()
{
	unsigned int i;

	// equal tempering
	/*gvOctave[ 0 ] = 1.0f; //c
	gvOctave[ 1 ] = 1.0595f; //c#
	gvOctave[ 2 ] = 1.1225f; // d
	gvOctave[ 3 ] = 1.1892f;
	gvOctave[ 4 ] = 1.2599f;
	gvOctave[ 5 ] = 1.3348f;
	gvOctave[ 6 ] = 1.4142f;
	gvOctave[ 7 ] = 1.4983f;
	gvOctave[ 8 ] = 1.5874f;
	gvOctave[ 9 ] = 1.6818f; // a
	gvOctave[ 10 ] = 1.7818f;
	gvOctave[ 11 ] = 1.8877f;*/

	// just intonation
	gvOctave[ 0 ] = 1.0f; //c
	gvOctave[ 1 ] = 1.0666666667f; //c#
	gvOctave[ 2 ] = 1.125f; // d
	gvOctave[ 3 ] = 1.2f; // d#
	gvOctave[ 4 ] = 1.25f; // e
	gvOctave[ 5 ] = 1.3333333332f; // f
	gvOctave[ 6 ] = 1.4f; // f#
	gvOctave[ 7 ] = 1.5f; // g
	gvOctave[ 8 ] = 1.6f; // g#
	gvOctave[ 9 ] = 1.6666666667f; // a
	gvOctave[ 10 ] = 1.8f; // a#
	gvOctave[ 11 ] = 1.875f; // h

	FP divisor = gvOctave[ 9 ]; // normalize by a
	for( i=0;i<12;i++ )
		gvOctave[ i ] /= divisor;

	FP groundA = 440.0f/16.0f;

	for( i=0;i<8;i++ ) // octaves
		for( int j=0;j<12;j++ ) // notes
		{
			gvNoteMatrix[((i)*12)+j] = groundA * gvOctave[j] * (1<<i);

			gvNotesMatrixHuman[((i)*12)+j] = gvNotesMatrixHuman[ j ];
		}
};

//-----------------------------------------------------------------------------
// + destruction
//-----------------------------------------------------------------------------
Synth::~Synth()
{
	fireDestroy();

	SYNTH_COUNT--;

	SAFE_DELETE( ivPtOsc );
	ivTransformers.deleteAll();
}

//-----------------------------------------------------------------------------
// + setSoundSource
//-----------------------------------------------------------------------------
void Synth::setSoundSource( MOsc *source )
{
	MSynchronize cls( &ivCriticalSection );
	ASSERT( this->ivPtTracker );

	MOsc *temp = ivPtOsc;

	ivPtOsc = source;
	if( ivPtOsc )
		ivPtOsc->registerTargets( ivPtTracker );
	fireStateChanged( MInstrumentEvent( MSG_OSC_CHANGED ) );

	SAFE_DELETE( temp );

};

//-----------------------------------------------------------------------------
// + addTransformer
//-----------------------------------------------------------------------------
void Synth::addTransformer( MTransformer *transformer )
{
	MSynchronize cls( &ivCriticalSection );
	ASSERT( this->ivPtTracker );

	ivTransformers.add( transformer );
	transformer->registerTargets( ivPtTracker );
	fireStateChanged( MInstrumentEvent( MSG_TRANSFORMER_ADDED, ivTransformers.getLength()-1 ) );
}

void Synth::transformerUp()
{
	if( this->ivTransformers.getSelectedObject() )
	{
		int index = this->ivTransformers.getSelection();
		if( index > 0 )
		{
			swapTransformer( index-1, index );
			this->ivTransformers.setSelection( index - 1 );
		}
	}
}

void Synth::transformerDown()
{
	if( this->ivTransformers.getSelectedObject() )
	{
		int index = this->ivTransformers.getSelection();
		if( index < (int)(ivTransformers.getLength() - 1) )
		{
			swapTransformer( index, index+1 );
			this->ivTransformers.setSelection( index + 1 );
		}
	}
}

//-----------------------------------------------------------------------------
// + swapTransformer
//-----------------------------------------------------------------------------
void Synth::swapTransformer(int index1, int index2)
{
	MSynchronize cls( &ivCriticalSection );

	if( index1 >= 0 && index2 >= 0 &&
		index1 < (int)ivTransformers.getLength() &&
		index2 < (int)ivTransformers.getLength() )
	{
		if( index1 > index2 )
		{
			int tmp = index1;
			index1 = index2;
			index2 = tmp;
		}

		MTransformer* t2 = (MTransformer*) ivTransformers.remove( index2 );
		MTransformer* t1 = (MTransformer*) ivTransformers.remove( index1 );

		ivTransformers.add( t2, index1 );
		ivTransformers.add( t1, index2 );

		fireStateChanged( MInstrumentEvent( MSG_TRANSFORMER_SWAPPED, index1, index2 ) );
	}
}

//-----------------------------------------------------------------------------
// + removeTransformer
//-----------------------------------------------------------------------------
void Synth::removeTransformer( int index )
{
	ASSERT( index >= 0 && index < (int) ivTransformers.getLength() );

	MSynchronize cls( &ivCriticalSection );

	MTransformer* ptTransformer = (MTransformer*) ivTransformers.remove( index );
	ASSERT( ptTransformer );
	fireStateChanged( MInstrumentEvent( MSG_TRANSFORMER_REMOVED, index ) );
	delete ptTransformer;
}

void Synth::removeSelectedTransformer()
{
	if( this->getTransformers()->getSelectedObject() )
	{
		MSynchronize cls( &ivCriticalSection );

		int index = getTransformers()->getSelection();
		MTransformer* ptTransformer = (MTransformer*)
			ivTransformers.remove(
				index );

		ASSERT( ptTransformer );

		fireStateChanged( MInstrumentEvent( MSG_TRANSFORMER_REMOVED, index ) );
		delete ptTransformer;
	}
}

//-----------------------------------------------------------------------------
// + getTransformerIndex
//-----------------------------------------------------------------------------
int Synth::getTransformerIndex( MTransformer *transformer )
{
	return this->ivTransformers.getIndex( transformer );
}

void Synth::processEventChilds( MEvent* ptEvent )
{
	ivPtOsc->processEvent( ptEvent );

	unsigned int count = ivTransformers.getLength();
	for( unsigned int i=0;i<count;i++ )
		((MTransformer*)this->ivTransformers.get( i ))->processEvent( ptEvent );
}

void Synth::processEvent( MEvent* ptEvent )
{
	switch( ptEvent->getType() )
	{
	case MEvent::NOTE_ON:
		{
			MDefaultSynthNote* ptSynthNote = (MDefaultSynthNote*) ((MEventNote*)ptEvent)->getNote();
			ivMaxAmp = ptSynthNote->getVolume();
			FP oscFrequence = gvNoteMatrix[ ptSynthNote->getValue() ];
			if( ivPtOsc )
			{
				if( ivIsNoteOn )
					processEventChilds( &MEventPortamento( oscFrequence, PORTAMENTO_SPEED ) );
				else
					processEventChilds( &MEventNoteOn( oscFrequence ) );
			}
			ivIsNoteOn = true;
		}
		break;
	case MEvent::NOTE_OFF:
		{
			MDefaultSynthNote* ptSynthNote = (MDefaultSynthNote*) ((MEventNote*)ptEvent)->getNote();
			if( ! ptSynthNote->existsOverlappingAtEnd() ) 
			{
				processEventChilds( &MEvent( MEvent::NOTE_OFF ) );
				ivIsNoteOn = false;
			}
		}
		break;
	case MEvent::FALLDOWN:
		{
			MDefaultSynthNote* ptSynthNote = (MDefaultSynthNote*) ((MEventNote*)ptEvent)->getNote();
			if( ptSynthNote->needsFalldown() )
			{
				MEvent event( MEvent::FALLDOWN );
				processEventChilds( &event );
			}
		}
		break;
	case MEvent::RESET:
		{
			ASSERT( ivPtOsc );
			ivIsNoteOn = false;
			MEvent event1( MEvent::RESET );
			processEventChilds( &event1 );
			MEvent event2( MEvent::NOTE_OFF );
			processEventChilds( &event2 );
		}
		break;
	default:
		ASSERT( false );
		break;
	}
}

//-----------------------------------------------------------------------------
// + goNext (Inherited from MInstrument)
//-----------------------------------------------------------------------------
void Synth::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int endAt )
{
	//MClock sTotal;
	//sTotal.start();
	MSynchronize cls( &ivCriticalSection );
	if( ! getMute() )
	{
		unsigned int channelCount = buffer->getChannelCount();
		for( unsigned int i=0;i<channelCount;i++ )
		{
			//MClock oscClock;
			//oscClock.start();
			ivPtOsc->goNext( buffer, startFrom, endAt );
			//oscClock.stop();

			//MClock tClock;
			//tClock.start();
			unsigned int count = ivTransformers.getLength();
			for( unsigned int n=0;n<count;n++ )
				((MTransformer*)ivTransformers.get(n))->goNext( buffer, startFrom, endAt );
			//tClock.stop();

			FP* ptStop = buffer->getData( i ) + endAt;

			//MClock mClock;
			//mClock.start();
			for( FP* pt=buffer->getData( i ) + startFrom;pt<ptStop;pt++ )
				(*pt) *= ivMaxAmp;
			//mClock.stop();

			/*TRACE( "\tsynth osc: %f msec trans: %f msec mul: %f msec\n",
				oscClock.getLastTimeSpan(),
				tClock.getLastTimeSpan(),
				mClock.getLastTimeSpan() );*/
		}
		MDefaultMixerChannel::goNext( buffer, startFrom, endAt );
	}
	//sTotal.stop();
	//TRACE( "\tsynth total(%fms)\n", sTotal.getLastTimeSpan() );
}

//-----------------------------------------------------------------------------
// + getChannelCount
// returns 1 because synth is mono
//-----------------------------------------------------------------------------
unsigned int	Synth::getChannelCount()
{
	return 1;
}

//-----------------------------------------------------------------------------
// + getNoteListId (Inherited from MInstrument)
// Needed for DragNDropOperations between different channels of the
// ArrangeView
//-----------------------------------------------------------------------------
int Synth::getNoteListId()
{
	return MDefaultSynthNoteList::TYPE_ID;
}

//-----------------------------------------------------------------------------
// + noteListNeedsListener
//	returns false because notelist has no messages to get
//-----------------------------------------------------------------------------
bool Synth::noteListNeedsListener()
{
	return false;
}

//-----------------------------------------------------------------------------
// + getEditorId (Inherited from MInstrument)
// Needed to open the right editor for this instrument
//-----------------------------------------------------------------------------
int	Synth::getEditorId()
{
	return Synth::TYPE_ID;
}

//-----------------------------------------------------------------------------
// + getName (inheritanced from IDescriptorCollection)
//-----------------------------------------------------------------------------
String	Synth::getName()
{
	return String("MSynth");
}

IDescriptor* Synth::getOsc()
{
	return ivPtOsc;
}

//-----------------------------------------------------------------------------
// + getMDescriptorCount (inheritanced from IDescriptorCollection)
//-----------------------------------------------------------------------------
unsigned int Synth::getTransformerCount()
{
	return ivTransformers.getLength();
}

//-----------------------------------------------------------------------------
// + getMDescriptor (inheritanced from IDescriptorCollection)
//-----------------------------------------------------------------------------
IDescriptor* Synth::getTransformer( unsigned int index )
{
	ASSERT( index < ivTransformers.getLength() );
	return (MTransformer*) ivTransformers.get( index );
}

/**
 * stores as tree node
 */
MTreeNode* Synth::save()
{
	MTreeNode* ptBack = new MTreeNode( getType() );

	// attributes
	ptBack->setAttribute( "value", getName() );

	// controls
	unsigned int count = getControlCount();
	for( unsigned int i=0;i<count;i++ )
		ptBack->addChild( getControl( i )->save() );

	// source processor
	ptBack->addChild( ivPtOsc->save() );

	// processors
	count = ivTransformers.getLength();
	for( i=0;i<count;i++ )
		ptBack->addChild( ((MTransformer*)ivTransformers.get(i))->save() );

	// store properties
	ptBack->addChild( ivPtProperties->MStr2ObjMap::save() );

	return ptBack;
}

/**
 * loads from the given tree node
 */
void Synth::load( MTreeNode* ptNode )
{
	ASSERT( this->ivPtTracker );

	if( ptNode->getAttribute( "value" ) != getName() )
		throw MException( "invlid synth found!" );

	MTreeNodeIterator iter = ptNode->getIterator();
	while( iter.hasNext() )
	{
		MTreeNode* node = iter.nextTreeNode();
		String nodeName = node->getName();
		if( nodeName == "control" )
		{
			IControl* ptControl =
				getControlByName(
					node->getAttribute( "name" ) );
			if( ptControl )
				ptControl->load( node );
		}
		else if( nodeName == "sourceprocessor" )
		{
			MObject *ptObj =
				MDefaultFactory::getInstance()->createObject(
					node->getAttribute( "value" ) );
			if( ptObj )
			{
				MOsc* ptSource = (MOsc*) ptObj->getInterface( "MOsc" );
				if( ptSource )
				{
					ptSource->load( node );
					setSoundSource( ptSource );
				}
				else
				{
					MLogger::logError(
						"Synth::load: invalid oscillator found \"%s\"",
						node->getAttribute( "value" ) );
				}
			}
			else
			{
				MLogger::logError(
					"Synth::load: invalid oscillator found \"%s\"",
					node->getAttribute( "value" ) );
			}
		}
		else if( nodeName == "processor" )
		{
			String processorName = node->getAttribute( "value" );

			MTransformerRtti* ptRtti =
				MTransformerRegistry::getInstance()->getTransformerByClass(
					processorName );

			MObject* ptObject =
				MDefaultFactory::getInstance()->createObject(
					processorName );

			if( ptObject )
			{
				MTransformer* ptTransformer = (MTransformer*)
					ptObject->getInterface( "MTransformer" );
				if( ptTransformer )
				{
					ptTransformer->load( node );
					addTransformer( ptTransformer );
				}
				else
				{
					delete ptObject;
					MLogger::logError(
						"Synth::load: invalid processor found \"%s\"",
						processorName );
				}
			}
			else
			{
				MLogger::logError(
					"Synth::load: invalid processor found \"%s\" (not a MObject)",
					processorName );
			}
		}
		else if(
			nodeName == "mobject" &&
			node->getAttribute( "class" ) == "MProperties" )
		{
			ivPtProperties->MStr2ObjMap::load( node );
		}
		else
		{
			MLogger::logError(
				"Synth::load: invalid element found \"%s\"",
				nodeName );
		}
	}

	if( ! ivPtOsc )
		ivPtOsc = (MOsc*) MDefaultFactory::getInstance()->createObject( "MOscSaw" );

}

//-----------------------------------------------------------------------------
// + getBitmapId
//	returns the bitmap id to display on the button that opens the synth editor
//-----------------------------------------------------------------------------
int Synth::getBitmapId( int state )
{
	switch( state )
	{
		case 0:
			return IDB_ICON_SYNTH;
			break;
		default:
			return -1;
	}
}

//-----------------------------------------------------------------------------
// + open
//	opens and loads a synth from a xml file (*.msl)
//-----------------------------------------------------------------------------
void Synth::open( const char* fileName )
{
	MSynchronize cls( &ivCriticalSection );

	String error( "" );

	MSaxTreeDocument doc;
	doc.parse( fileName );
	MTreeNode* ptRoot = doc.getRoot();

	if( ptRoot )
	{
		if( ptRoot->getName() == "MSoundSource" ||
			ptRoot->getName() == "source" )
		{
			MXMLConverter::getInstance()->formatXML( ptRoot, 0 );
		}
		else if( ptRoot->getName() == "synth" )
		{
			MTreeNodeIterator iter = ptRoot->getIterator();
			while( iter.hasNext() )
			{
				MTreeNode* ptChild = iter.nextTreeNode();
				if( ptChild->getName() == "source" )
				{
					ptRoot = ptChild;
				}
			}
		}

		while( ivTransformers.getLength() > 0 )
			removeTransformer( 0 );

		load( ptRoot );
	}
	else
		MMessageBox::showError( "ERROR_LOADING", fileName, "could not get root node" );
}

//-----------------------------------------------------------------------------
// + save
//-----------------------------------------------------------------------------
void Synth::save( const char* fileName )
{
	String pre( "" );
	pre.Format(
		"<!-- MSynth instrument file - DO NOT EDIT -->\n<synth version=\"%s\">\n",
		MFileHandling::getInstance()->getVersion().getData() );
	String post( "</synth>" );

	MTreeNode* ptSaves = save();
	MFilePrinter printer;
	if( printer.open( fileName ) )
	{
		printer.print( pre );
		ptSaves->toXml( &printer );
		printer.print( post );
		printer.close();
	}
	else
		MMessageBox::showError( "ERROR_WRITING_SYNTH" );
	delete ptSaves;
}

String Synth::toString()
{
	return getName();
}

IControlCollection* Synth::getChild( unsigned int index )
{
	if( index == 0 )
		return ivPtOsc;
	else
		return this->getTransformer( index - 1 );
}

unsigned int Synth::getChildCount()
{
	return 1 + getTransformerCount();
}

MSelectableObjectList* Synth::getTransformers()
{
	return &this->ivTransformers;
}