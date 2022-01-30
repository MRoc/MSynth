/*
	(C)2001 MRoc hifiShock

*/

#ifndef __MNoteFactory
#define __MNoteFactory

#include <framework/MTypes.h>
#include "MDefaultNoteList.h"
#include "../../ui/noteeditor/MAbstractNoteFrame.h"

typedef MDefaultNoteList* (*CREATENOTELISTINSTANCE)();
typedef MAbstractNoteFrame* (*CREATENOTEEDITORINSTANCE)();

class MNoteFactory
{
	protected:

		std::vector<CREATENOTELISTINSTANCE> ivRegisteredNoteLists;
		std::vector<CREATENOTEEDITORINSTANCE> ivRegisteredNoteEditors;
		std::vector<int>		ivRegisteredNoteListIds;

		static MNoteFactory*	ivSharedInstance;

	public:

		MNoteFactory();

		~MNoteFactory();

		void					registerNoteList(	int id,
													CREATENOTELISTINSTANCE noteListCreator,
													CREATENOTEEDITORINSTANCE noteEditorCreator );

		MDefaultNoteList*		getNoteList( int id );
		MAbstractNoteFrame*		getNoteEditor( int id );

		int						getRegisteredNoteListCount();
		int						getRegisteredNoteListId( int index );

		static MNoteFactory*	getInstance();
		static void				release();
};

#endif