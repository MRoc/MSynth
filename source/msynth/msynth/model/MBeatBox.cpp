/*-----------------------------------------------------------------------------

	(C)2000 MRoc hifiShock

	Class for simultanous playing a bunch of samples

	v 0.1		->	design
	v 0.2		->	better PitchAlgorythm, with linear interpolation
	v 0.2b		->	Accent-Off-bugfix
	v 0.2c		->	Save SamplePack design
	v 0.2d		->	Save and Open SamplePack, NEW getSampleName(),
					setSampleName(char*, int) 
	v 0.2d/2	->	Accent-Off-bugfix 2
	v 0.2d/3	->	SaveSampleBank reverse-sample-fix
	v 0.2d/4	->	FineTune bugfix
	v 0.2e		->	Accent-Off-bugfix 3, full working
	v 0.2f		->	loadFile bugFix, crashed when loading failed
	v 0.2g		->	better performance, cast instead of floor!!!

	v.0.3		->	Divided in MWavePlayer and MBeatBox (design)
	v.0.4		->	introducing MNote and MEvent 
	v.0.5		->	Variable ChannelCount
	v.0.5a		->	BugFix Crashs when loading samplebank with more channels
					than current channelcount
	v.0.6		->	Composition with MTransformerCollection to provide MasterFXs
	v.0.61		->	Becomes a MMixerChannel
	v.0.7		->	Buffer-Running, masterChannel got panorama
	v.0.8		->	Inheritanced from MInstrument
	v.0.9		->	ThreadSafe (CriticalSection)
	v.1.0		->	Exclusive mode
	v.1.1		->	fstream replaced by CFile
	v.1.1b		->	removed fatal create wave player id bug
	v.1.2		->	using MSampleBank for binary import and export
	v.1.2b		->	introduced the parse child method because of internal compiler errror... (vs.net2002)

-----------------------------------------------------------------------------*/

#include "MBeatBox.h"
#include <framework/performance/MClock.h>
#include <framework/io/MLogger.h>
#include <gui/dialog/MMessageBox.h>

int MBeatBox::TYPE_ID = 2;
int MBeatBox::DRUMBOX_COUNT = 0;
int MBeatBox::MESSAGE_UPDATE_FROM_MODEL = 0;
int MBeatBox::MESSAGE_CHANNEL_ADDED = 1;
int MBeatBox::MESSAGE_CHANNEL_REMOVED = 2;
int MBeatBox::MESSAGE_CHANNEL_SELECTED = 3;

const char* MBeatBox::CLASS_NAME = "MBeatBox";

//-----------------------------------------------------------------------------
// + construction
//-----------------------------------------------------------------------------
MBeatBox::MBeatBox( IBpmControl* ptBpmControl ) :
	MInstrument(),
	ivPtChannels( 0 ),
	ivPtBpmControl( 0 )
{
	DRUMBOX_COUNT++;

	ivPtChannels = new MSelectableObjectList();

	setChannelName( "BeatBox" + MUtil::toString( DRUMBOX_COUNT ) );

	ivPtBpmControl = ptBpmControl;
	if( ivPtBpmControl )
		ivPtBpmControl->addBpmListener( this );
}

void MBeatBox::makeDefaultPlayer()
{
	ASSERT( ivPtTracker );

	for( unsigned int i=0;i<8;i++ )
	{
		MWavePlayer* ptNewPlaya = new MWavePlayer( i );
		ptNewPlaya->setBpm( ivPtBpmControl->getBpm() );
		ptNewPlaya->registerTargets( ivPtTracker );
		ivPtChannels->add( ptNewPlaya );
	}
	ivPtChannels->setSelection( (int)0 );
}

//------------------------------------------------------------------------------------
// + destruction
//------------------------------------------------------------------------------------
MBeatBox::~MBeatBox()
{
	fireDestroy();

	if( ivPtBpmControl )
		ivPtBpmControl->removeBpmListener( this );

	DRUMBOX_COUNT--;

	ivPtChannels->deleteAll();
	SAFE_DELETE( ivPtChannels );
}

//-----------------------------------------------------------------------------
// + createChannel
//-----------------------------------------------------------------------------
void MBeatBox::createChannel()
{
	ASSERT( ivPtTracker );

	MSynchronize cls( &ivCriticalSection );

	MWavePlayer* ptNewPlayer = new MWavePlayer( ivPtChannels->getLength() );
	ptNewPlayer->registerTargets( ivPtTracker );
	ptNewPlayer->setBpm( ivPtBpmControl->getBpm() );
	ivPtChannels->add( ptNewPlayer );
	fireChannelAdded( ptNewPlayer );
}

//-----------------------------------------------------------------------------
// + deleteChannel
//-----------------------------------------------------------------------------
void MBeatBox::deleteChannel( MObject* ptChannel )
{
	MSynchronize cls( &ivCriticalSection );

	int index = ivPtChannels->getIndex( ptChannel );
	ASSERT( index >= 0 && index < (int)ivPtChannels->getLength() );

	MWavePlayer* toDelete = getWavePlayerAt( index );
	ivPtChannels->MObjectList::remove( toDelete );
	fireChannelRemoved( toDelete, index );
	delete toDelete;

	int sel = this->ivPtChannels->getSelection();
	if( sel <= 0 )
		this->setSelectedChannel( 0 );
	else if( sel >= (int) ivPtChannels->getLength() )
		this->setSelectedChannel( (int)ivPtChannels->getLength() - 1 );
	else
		this->fireSelectedChannelChanged();
}

//-----------------------------------------------------------------------------
// + setWavePlayerCount
//-----------------------------------------------------------------------------
void MBeatBox::setWavePlayerCount( unsigned int channelCount )
{
	MSynchronize cls( &ivCriticalSection );

	std::vector<MWavePlayer*> bad;

	unsigned int curCount = ivPtChannels->getLength();

	if( channelCount < curCount )
	{
		unsigned int i;
		for( i=channelCount;i<curCount;i++ )
		{
			MWavePlayer* toRemove = ((MWavePlayer*)ivPtChannels->get( i ));
			bad.push_back( toRemove );
		}
		unsigned int count = bad.size();
		for( i=0;i<count;i++ )
			ivPtChannels->MObjectList::remove( (MObject*) bad[ i ] );
	}
	else if( channelCount > curCount )
	{
		for( unsigned int i=ivPtChannels->getLength();i<channelCount;i++ )
		{
			MWavePlayer* ptPlaya = new MWavePlayer( i );
			ptPlaya->setBpm( ivPtBpmControl->getBpm() );			
			ptPlaya->registerTargets( ivPtTracker );
			ivPtChannels->add( ptPlaya );
		}
	}

	fireUpdateFromModel();

	while( bad.size() > 0 )
	{
		MWavePlayer* ptPlayer = bad[ 0 ];
		bad.erase( bad.begin() );
		delete ptPlayer;
	}
}

//-----------------------------------------------------------------------------
// + getWavePlayerCount
//-----------------------------------------------------------------------------
unsigned int MBeatBox::getWavePlayerCount()
{
	return ivPtChannels->getLength();
}

//-----------------------------------------------------------------------------
// + setSelectedChannel
//-----------------------------------------------------------------------------
void MBeatBox::setSelectedChannel( MWavePlayer* ptPlaya )
{
	ivPtChannels->setSelection( ptPlaya );
	fireSelectedChannelChanged();
}

void MBeatBox::setSelectedChannel( int index )
{
	ivPtChannels->setSelection( index );
	fireSelectedChannelChanged();
}

//-----------------------------------------------------------------------------
// + getSelectedChannel
//-----------------------------------------------------------------------------
MWavePlayer* MBeatBox::getSelectedChannel()
{
	return (MWavePlayer*) ivPtChannels->getSelectedObject();
}

int MBeatBox::getSelection()
{
	return ivPtChannels->getSelection();
}

void MBeatBox::processEvent( MEvent* ptEvent )
{
	switch( ptEvent->getType() )
	{
	case MEvent::NOTE_ON:
		{
			MSynchronize cls( &ivCriticalSection );

			MDefaultMessageNote* ptMsgNote = (MDefaultMessageNote*) ((MEventNote*)ptEvent)->getNote();
			unsigned int count = ptMsgNote->getMessageCount();
			for( unsigned int i=0;i<count;i++ )
			{
				MBeatBoxMessage* ptMsg = (MBeatBoxMessage*) ptMsgNote->getMessageAt( i );
				if( (unsigned int)ptMsg->getId() < ivPtChannels->getLength() )
				{
					unsigned int index = ptMsg->getId();
					ASSERT( index < getWavePlayerCount() );
					MWavePlayer* ptPlaya = getWavePlayerAt( index );
					ptPlaya->setNoteVolume( ptMsg->getVolume() );
					ptPlaya->setOn(true);
					if( ptPlaya->getExclusive() )
					{
						unsigned int wCount = getWavePlayerCount();
						for( unsigned int j=0;j<wCount;j++ )
						{
							MWavePlayer* ptP2 = getWavePlayerAt( j );
							if( index != j &&
								ptP2->getExclusive() &&
								ptP2->getOn() )
							{
								ptP2->setOn( false );
							}
						}
					}
				}
			}
		}
		break;
	case MEvent::FALLDOWN:
		{
			MSynchronize cls( &ivCriticalSection );

			MDefaultMessageNote* ptMsgNote = (MDefaultMessageNote*) ((MEventNote*)ptEvent)->getNote();
			unsigned int mCount = ptMsgNote->getMessageCount();
			unsigned int channelCount = ivPtChannels->getLength();
			for( unsigned int i=0;i<mCount;i++ )
			{
				MBeatBoxMessage* ptMsg = (MBeatBoxMessage*) ptMsgNote->getMessageAt( i );
				unsigned int channel = unsigned int(ptMsg->getId());
				//ASSERT( ptMsg->getId() < ivChannelCount ); <- msg can have higher id than channels!!!
				if( channel < channelCount &&
					getWavePlayerAt( channel )->getOn() )
				{
					getWavePlayerAt( channel )->setFallDown( true );
				}
			}

			for( i=0;i<channelCount;i++ )
			{
				ASSERT( getWavePlayerAt( i ) );
				if( getWavePlayerAt( i )->getExclusive() &&
					getWavePlayerAt( i )->getOn() )
				{
					for( unsigned int j=0;j<mCount;j++ )
					{
						MBeatBoxMessage* ptMsg = (MBeatBoxMessage*) ptMsgNote->getMessageAt( j );
						ASSERT( unsigned int(ptMsg->getId()) < channelCount );
						if( getWavePlayerAt( ptMsg->getId() )->getExclusive() )
							getWavePlayerAt( i )->setFallDown( true );
					}
				}
			}
		}
		break;
	case MEvent::RESET:
		{
			MSynchronize cls( &ivCriticalSection );
			unsigned int count = ivPtChannels->getLength();
			for( unsigned int i=0;i<count;i++ )
				getWavePlayerAt( i )->resetPlayState();
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// + goNext ( inheritanced from MInstrument )
//-----------------------------------------------------------------------------
void MBeatBox::goNext( MSoundBuffer *buffer, unsigned int renderFrom, unsigned int renderTo  )
{
	if( ! getMute() )
	{
		MSynchronize cls( &ivCriticalSection );

		unsigned int count = ivPtChannels->getLength();
		for( unsigned int i=0;i<count;i++ )
		{
			MWavePlayer* ptPlaya = getWavePlayerAt( i );
			if( ptPlaya->getOn() &&
				ptPlaya->isValid() &&
				! ptPlaya->getMute() )
			{
				ptPlaya->goNext( buffer, renderFrom, renderTo );
			}
		}
		MDefaultMixerChannel::goNext( buffer, renderFrom, renderTo );
	}
}

//-----------------------------------------------------------------------------
// + getChannelCount
// returns 2, because the beatbox is stereo
//-----------------------------------------------------------------------------
unsigned int MBeatBox::getChannelCount()
{
	return 2;
}

//-----------------------------------------------------------------------------
// + getNoteListId (Inherited from MInstrument)
//-----------------------------------------------------------------------------
int MBeatBox::getNoteListId()
{
	return MBeatBoxNoteList::TYPE_ID;
}

//-----------------------------------------------------------------------------
// + noteListNeedsListener (Inherited from MInstrument)
//-----------------------------------------------------------------------------
bool MBeatBox::noteListNeedsListener()
{
	return true;
}

//-----------------------------------------------------------------------------
// + getBitmapId (Inherited from MInstrument)
//-----------------------------------------------------------------------------
int MBeatBox::getBitmapId( int state )
{
	switch( state )
	{
		case 0:
			return IDB_ICON_BEATBOX;
		default:
			return -1;
	}
}

//-----------------------------------------------------------------------------
// + getEditorId (Inherited from MInstrument)
// needed to open the right editor for this instrument
//-----------------------------------------------------------------------------
int	MBeatBox::getEditorId()
{
	return MBeatBox::TYPE_ID;
}

void MBeatBox::bpmChanged( unsigned int bpm )
{
	unsigned int count = ivPtChannels->getLength();
	for( unsigned int i=0;i<count;i++ )
		getWavePlayerAt( i )->setBpm( (int) bpm );
}

//-----------------------------------------------------------------------------
// + getWavePlayerAt
//-----------------------------------------------------------------------------
MWavePlayer *MBeatBox::getWavePlayerAt( unsigned int index )
{
	ASSERT( ivPtChannels && index < ivPtChannels->getLength() );
	return (MWavePlayer*) ivPtChannels->get( index );
}

//-----------------------------------------------------------------------------
// + newSampleBank
//-----------------------------------------------------------------------------
bool MBeatBox::newSampleBank()
{
	MSynchronize cls( &ivCriticalSection );

	unsigned int count = ivPtChannels->getLength();
	for( unsigned int i=0;i<count;i++ )
	{
		getWavePlayerAt( i )->resetPlayState();
		getWavePlayerAt( i )->resetSample();
	}

	return true;
}

//-----------------------------------------------------------------------------
// + saveSampleBank
//-----------------------------------------------------------------------------
bool MBeatBox::saveSampleBank( String fileName )
{
	MSynchronize cls( &ivCriticalSection );

	MSampleBank samplebank;
	for( unsigned int i=0;i<this->getWavePlayerCount();i++ )
	{
		MSampleBankEntry* ptEntry = new MSampleBankEntry( this->getWavePlayerAt( i )->getWave() );
		ptEntry->setName( getWavePlayerAt( i )->getSampleName() );
		samplebank.addSample( ptEntry );
	}
	samplebank.export( fileName );
	while( samplebank.getSampleCount() > 0 )
	{
		MSampleBankEntry* ptEntry = samplebank.getSample( 0 );
		samplebank.removeSample( ptEntry );
		ptEntry->setWave( 0 );
		delete ptEntry;
	}
	ivSampleBankName = fileName;

	return true;
}

//-----------------------------------------------------------------------------
// + loadSampleBank
//-----------------------------------------------------------------------------
bool MBeatBox::loadSampleBank( String fileName )
{
	MSynchronize cls( &ivCriticalSection );

	try
	{
		MSampleBank samplebank;
		samplebank.import( fileName );
		this->setWavePlayerCount( samplebank.getSampleCount() );

		unsigned int count = 0;
		while( samplebank.getSampleCount() > 0 )
		{
			MSampleBankEntry* ptEntry = samplebank.getSample( 0 );
			MWavePlayer* ptPlayer = this->getWavePlayerAt( count++ );
			ptPlayer->setWave( ptEntry->getWave() );
			ptPlayer->setSampleName( ptEntry->getName() );
			ptEntry->setWave( 0 );
			samplebank.removeSample( ptEntry );
			delete ptEntry;
		}

		ivSampleBankName = fileName;

		this->fireUpdateFromModel();
	}
	catch( MException ae )
	{
		MMessageBox::showError( "Could not load samplebank!" );
	}

	return true;
}

/**
 * stores as tree node
 */
MTreeNode* MBeatBox::save()
{
	MTreeNode* ptBack = 0;
	MSynchronize cls( &ivCriticalSection );

	// start tag
	ptBack = new MTreeNode( getType() );

	// attributes
	ptBack->setAttribute( "value", getName() );
	ptBack->setAttribute( "channelCount", MInt::toString( ivPtChannels->getLength() ) );

	// controls
	unsigned int count = getControlCount();
	for( unsigned int i=0;i<count;i++ )
		ptBack->addChild( getControl( i )->save() );

	// waveplayers
	count = ivPtChannels->getLength();
	for( i=0;i<count;i++ )
		ptBack->addChild( getWavePlayerAt( i )->save() );

	// properties
	ptBack->addChild( ivPtProperties->MStr2ObjMap::save() );
	
	return ptBack;
}

void MBeatBox::parseChildNode( MTreeNode* childNode, unsigned int *curChannel )
{
	String nodeName = childNode->getName();

	if( nodeName == "channel" )
	{
		getWavePlayerAt( (*curChannel)++ )->load( childNode );
	}
	else if( nodeName ==  "control" )
	{
		IControl* ptControl =
			getControlByName(
				childNode->getAttribute( "name" ) ); 
		if( ptControl )
			ptControl->load( childNode );
	}
	else if(
		nodeName == "mobject" &&
		childNode->getAttribute( "class" ) == "MProperties" )
	{
		ivPtProperties->MStr2ObjMap::load( childNode );
	}
	else
	{
		MLogger::logError(
			"MBeatBox::load: found unknown node \"%s\"",
			nodeName );
	}
}

/**
 * loads from the given tree node
 */
void MBeatBox::load( MTreeNode* ptNode )
{
	ASSERT( ivPtTracker );
	MSynchronize cls( &ivCriticalSection );

	setWavePlayerCount(
		MInt::parse(
			ptNode->getAttribute( "channelCount" ),
			1,
			32,
			8 ) );

	unsigned int curChannel = 0;
	MTreeNodeIterator iter = ptNode->getIterator();
	while( iter.hasNext() )
		parseChildNode(
			iter.nextTreeNode(),
			&curChannel );

	String sampleBank = ptNode->getAttribute( "samplebank" );
	if( sampleBank != "" )
	{
		ivSampleBankName = sampleBank;
		loadSampleBank(	sampleBank );
	}

	this->setSelectedChannel( (int) 0 );
}

//-----------------------------------------------------------------------------
// + setSampleBankName
//-----------------------------------------------------------------------------
void MBeatBox::setSampleBankName( String name )
{
	ivSampleBankName = name;
}

//-----------------------------------------------------------------------------
// + getPathName
//-----------------------------------------------------------------------------
String MBeatBox::getPathName()
{
	return ivSampleBankName;
}

//-----------------------------------------------------------------------------
// + getSampleBankName
//-----------------------------------------------------------------------------
String MBeatBox::getSampleBankName()
{
	int index = ivSampleBankName.ReverseFind( '\\' );

	String back;
	if( index == -1 )
		back = ivSampleBankName;
	else
		back = ivSampleBankName.Mid( index + 1, ivSampleBankName.GetLength() - index );

	return back;
}

//-----------------------------------------------------------------------------
// + getSampleBankFolder
//-----------------------------------------------------------------------------
String MBeatBox::getSampleBankFolder()
{
	int index = ivSampleBankName.ReverseFind( '\\' );

	String back;
	if( index != -1 )
		back = ivSampleBankName.Left( index );

	return back;
}

//-----------------------------------------------------------------------------
// DESCRIPTOR
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// + getName
//-----------------------------------------------------------------------------
String	MBeatBox::getName()
{
	return String("MDrumBox");
}

//-----------------------------------------------------------------------------
// + fireUpdateFromModel
//-----------------------------------------------------------------------------
void MBeatBox::fireUpdateFromModel()
{
	fireStateChanged( MInstrumentEvent( MESSAGE_UPDATE_FROM_MODEL, ivPtChannels->getLength() ) );
}

//-----------------------------------------------------------------------------
// + fireChannelRemoved
//-----------------------------------------------------------------------------
void MBeatBox::fireChannelRemoved( MWavePlayer* ptPlayer, unsigned int index )
{
	fireStateChanged(
		MInstrumentEvent(
			MESSAGE_CHANNEL_REMOVED,
			index ) );
}


//-----------------------------------------------------------------------------
// + fireChannelAdded
//-----------------------------------------------------------------------------
void MBeatBox::fireChannelAdded( MWavePlayer* ptPlayer )
{
	fireStateChanged(
		MInstrumentEvent(
			MESSAGE_CHANNEL_ADDED,
			0 ) );
}

//-----------------------------------------------------------------------------
// + fireSelectedChannelChanged
//-----------------------------------------------------------------------------
void MBeatBox::fireSelectedChannelChanged()
{
	fireStateChanged(
		MInstrumentEvent(
			MESSAGE_CHANNEL_SELECTED,
			ivPtChannels->getSelection() ) );
}


String MBeatBox::toString()
{
	return getName();
}

IControlCollection* MBeatBox::getChild( unsigned int index )
{
	return getWavePlayerAt( index );
}

unsigned int MBeatBox::getChildCount()
{
	return getWavePlayerCount();
}

int MBeatBox::getWavePlayerIndex( MObject* ptPlaya )
{
	ASSERT( ptPlaya );
	return ivPtChannels->getIndex( ptPlaya );
}

String MBeatBox::getClassName()
{
	return CLASS_NAME;
}