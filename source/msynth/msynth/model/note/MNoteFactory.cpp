#include "MNoteFactory.h"
#include <gui/dialog/MMessageBox.h>

//-----------------------------------------------------------------------------
// + CONSTRUCTION
//-----------------------------------------------------------------------------
MNoteFactory::MNoteFactory()
{
};

//-----------------------------------------------------------------------------
// + DESTRUCTION
//-----------------------------------------------------------------------------
MNoteFactory::~MNoteFactory()
{
	ivRegisteredNoteLists.clear();
	ivRegisteredNoteListIds.clear();
	ivRegisteredNoteEditors.clear();
};

//-----------------------------------------------------------------------------
// + REGISTER NOTE LIST
//-----------------------------------------------------------------------------
void MNoteFactory::registerNoteList(	int id,
										CREATENOTELISTINSTANCE noteListCreator,
										CREATENOTEEDITORINSTANCE noteEditorCreator )
{
	MLogger::logMessage( "<mnotefactory::registernotelist id=\"%i\"/>\n", id );
	ivRegisteredNoteListIds.push_back( id );
	ivRegisteredNoteLists.push_back( noteListCreator );
	ivRegisteredNoteEditors.push_back( noteEditorCreator );
};

//-----------------------------------------------------------------------------
// + GET NOTE LIST
//-----------------------------------------------------------------------------
MDefaultNoteList* MNoteFactory::getNoteList( int id )
{
	MDefaultNoteList *ptNoteList = NULL;

	for( unsigned int i=0;i<ivRegisteredNoteListIds.size();i++ )
	{
		if( ((int)ivRegisteredNoteListIds.at(i)) == id )
		{
			ptNoteList = (MDefaultNoteList*) ((CREATENOTELISTINSTANCE)ivRegisteredNoteLists.at(i))();
		}
	}

	if( ptNoteList == 0 )
	{
		TRACE( "WARNING: MNoteFactory was asked for an unregistered ID: %i\n", id );
		MMessageBox::showError(	"NOTEFACTORY_UNREG_ID", MUtil::toString( id ) );
	}

	return ptNoteList;
};

//-----------------------------------------------------------------------------
// + GET NOTE EDITOR
// Returns an Instance of an noteEditor for an given noteId
//-----------------------------------------------------------------------------
MAbstractNoteFrame* MNoteFactory::getNoteEditor( int id )
{
	MAbstractNoteFrame *ptNoteEditor = NULL;

	for( unsigned int i=0;i<ivRegisteredNoteListIds.size();i++)
	{
		if( ((int)ivRegisteredNoteListIds.at(i)) == id )
		{
			ptNoteEditor = (MAbstractNoteFrame*) ((CREATENOTEEDITORINSTANCE)ivRegisteredNoteEditors.at(i))();
		}
	}

	if( ptNoteEditor == 0 )
	{
		TRACE( "WARNING: MNoteFactory was asked for an unregistered Editor ID: %i\n", id );
		MMessageBox::showError(	"NOTEFACTORY_UNREG_EDITORID", MUtil::toString( id ) );
	}

	return ptNoteEditor;
};

//-----------------------------------------------------------------------------
// + GET REGISTERED NOTE LIST COUNT
//-----------------------------------------------------------------------------
int MNoteFactory::getRegisteredNoteListCount()
{
	return ivRegisteredNoteListIds.size();
};

//-----------------------------------------------------------------------------
// + GET REGISTERED NOTE LIST ID
//-----------------------------------------------------------------------------
int MNoteFactory::getRegisteredNoteListId(int index)
{
	return ivRegisteredNoteListIds.at(index);
};

//-----------------------------------------------------------------------------
// + GET SHARED INSTANCE
//-----------------------------------------------------------------------------
MNoteFactory* MNoteFactory::getInstance()
{
	if( ivSharedInstance == NULL )
	{
		ivSharedInstance = new MNoteFactory();
	}

	return ivSharedInstance;
};

//-----------------------------------------------------------------------------
// + RELEASE
//-----------------------------------------------------------------------------
void MNoteFactory::release()
{
	SAFE_DELETE( ivSharedInstance );
};

MNoteFactory* MNoteFactory::ivSharedInstance = NULL;
