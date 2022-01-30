/*

	MInstrumentEditorFactory (C)2001 MRoc hifiShock

*/

#ifndef __MInstrumentEditorFactory
#define __MInstrumentEditorFactory

#include <framework/MTypes.h>
#include "MInstrumentEditor.h"

typedef MInstrumentEditor* (*CREATEEDITORINSTANCE)();

class MInstrumentEditorFactory
{
	protected:

		std::vector<CREATEEDITORINSTANCE> ivRegisteredEditors;
		std::vector<int> ivRegisteredEditorIds;

		static MInstrumentEditorFactory* ivSharedInstance;

	public:

		MInstrumentEditorFactory();
		~MInstrumentEditorFactory();

		void registerEditor( int id, CREATEEDITORINSTANCE creator );

		MInstrumentEditor *getEditor( int id );

		int getRegisterdEditorCount();
		int getRegisteredEditorId( int index );

		static MInstrumentEditorFactory* getInstance();
		static void release();
};

#endif