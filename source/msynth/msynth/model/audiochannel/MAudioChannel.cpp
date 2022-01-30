#include "MAudioChannel.h"
#include <gui/dialog/MMessageBox.h>

const int MAudioChannel::TYPE_ID = 3;

int MAudioChannel::AUDIOCHANNEL_COUNT = 0;

MAudioChannel::MAudioChannel() :
	ivPtAudioFiles( 0 ),
	ivPtCurrentFile( 0 ),
	ivFadeOut( false )
{
	AUDIOCHANNEL_COUNT++;
	setChannelName( "Audio" + MUtil::toString( AUDIOCHANNEL_COUNT ) );
	ivPtAudioFiles = new MSelectableObjectList();
}

MAudioChannel::~MAudioChannel()
{
	fireDestroy();

	AUDIOCHANNEL_COUNT--;
	if( ivPtAudioFiles )
		ivPtAudioFiles->deleteAll();
	SAFE_DELETE( ivPtAudioFiles );
}

void MAudioChannel::processEvent( MEvent* ptEvent )
{
	switch( ptEvent->getType() )
	{
	case MEvent::NOTE_ON:
		{
			MAudioTrigger* ptTrigger = (MAudioTrigger*) ((MEventNote*)ptEvent)->getNote();
			if( ptTrigger )
			{
				ivPtCurrentFile = ptTrigger->getFile();
				if( ivPtCurrentFile )
					ivPtCurrentFile->reset();
			}
		}
		break;
	case MEvent::NOTE_OFF:
		{
			ivFadeOut = true;
		}
		break;
	case MEvent::FALLDOWN:
		{
			ivFadeOut = true;
		}
		break;
	case MEvent::RESET:
		{
			if( ivPtCurrentFile )
				ivPtCurrentFile->reset();
			ivPtCurrentFile = 0;
			ivFadeOut = false;
		}
		break;
	}
}

void MAudioChannel::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt)
{
	ASSERT( buffer->getChannelCount() == 2 );
	if( ivPtCurrentFile && ! ivMuteControl->getValue() )
		ivPtCurrentFile->goNext( buffer, startFrom, stopAt );

	if( ivFadeOut )
	{
		ivFadeOut = false;
		buffer->fadeOut( startFrom, stopAt );
		ivPtCurrentFile = 0; // <- set it off man
	}

	MDefaultMixerChannel::goNext( buffer, startFrom, stopAt );
}

unsigned int MAudioChannel::getChannelCount()
{
	return 2;
}

int MAudioChannel::getNoteListId()
{
	return MAudioTriggerList::TYPE_ID;
}

int MAudioChannel::getEditorId()
{
	return MAudioChannel::TYPE_ID;
}

bool MAudioChannel::noteListNeedsListener()
{
	return false;
}

int MAudioChannel::getBitmapId( int state )
{
	switch( state )
	{
		case 0:
			return IDB_ICON_AUDIO;
		default:
			return -1;
	}
};

/**
 * stores as tree node
 */
MTreeNode* MAudioChannel::save()
{
	MTreeNode* ptBack = new MTreeNode( getType() );
	ptBack->setAttribute( "value", getName() );

	// controls
	unsigned int count = getControlCount();
	for( unsigned int i=0;i<count;i++ )
		ptBack->addChild( getControl( i )->save() );

	// audiofiles
	count = this->ivPtAudioFiles->getLength();
	for( i=0;i<count;i++ )
		ptBack->addChild( ((MAudioFile*)ivPtAudioFiles->get( i ))->save() );

	// store properties
	ptBack->addChild( ivPtProperties->MStr2ObjMap::save() );

	return ptBack;
}

/**
 * loads from the given tree node
 */
void MAudioChannel::load( MTreeNode* ptNode )
{
	String nodeName = ptNode->getName();
	ASSERT( nodeName == getType() );

	MTreeNodeIterator iter = ptNode->getIterator();
	while( iter.hasNext() )
	{
		MTreeNode* ptChild = iter.nextTreeNode();
		String childNodeName = ptChild->getName();
		if( childNodeName == "mobject" )
		{
			String className = ptChild->getAttribute( "class" );
			if( className == "MAudioFile" )
			{
				MAudioFile* ptFile = new MAudioFile();

				try
				{
					ptFile->load( ptChild );
				}
				catch( MException ptException )
				{
					MMessageBox::showError(
						"ERROR_LOADING",
						"could not load audiofile \"" + ptChild->getAttribute( "filename" ) + "\"",
						ptException.getExceptionDescripton() );
				}

				ivPtAudioFiles->add( ptFile );
			}
			else
				TRACE( "ERROR line %i file %s\n", __LINE__, __FILE__ );	
		}
		else
			TRACE( "ERROR line %i file %s\n", __LINE__, __FILE__ );
	}
}

String MAudioChannel::getName()
{
	return "MAudioChannel";
}

String MAudioChannel::toString()
{
	return getName();
}

MSelectableObjectList* MAudioChannel::getFileList()
{
	return this->ivPtAudioFiles;
}

void MAudioChannel::importFile( String fileName )
{
	MAudioFile* ptAudioFile = new MAudioFile();
	try
	{
		ptAudioFile->openAudioFile( fileName );
		ivPtAudioFiles->add( ptAudioFile );
	}
	catch( MException ae )
	{
		SAFE_DELETE( ptAudioFile );
		throw ae;
	}
}

IControlCollection* MAudioChannel::getChild( unsigned int index )
{
	return 0;
}

unsigned int MAudioChannel::getChildCount()
{
	return 0;
}