/*

	MInstrumentEditorFactory (C)2001 MRoc hifiShock

*/

#include "MInstrumentEditorFactory.h"

MInstrumentEditorFactory::MInstrumentEditorFactory()
{
};

MInstrumentEditorFactory::~MInstrumentEditorFactory()
{
	ivRegisteredEditorIds.clear();
	ivRegisteredEditors.clear();
};

void MInstrumentEditorFactory::registerEditor(int id, CREATEEDITORINSTANCE creator)
{
	ivRegisteredEditorIds.push_back(id);
	ivRegisteredEditors.push_back(creator);
};

MInstrumentEditor *MInstrumentEditorFactory::getEditor(int id)
{
	MInstrumentEditor *ptEditor = NULL;

	unsigned int count = ivRegisteredEditorIds.size();
	for(unsigned int i=0;i<count;i++)
		if( ((int)ivRegisteredEditorIds.at(i)) == id )
			ptEditor = (MInstrumentEditor*) ((CREATEEDITORINSTANCE)ivRegisteredEditors.at(i))();

	return ptEditor;
};

int MInstrumentEditorFactory::getRegisterdEditorCount()
{
	return ivRegisteredEditorIds.size();
};

int MInstrumentEditorFactory::getRegisteredEditorId( int index )
{
	return ivRegisteredEditorIds.at(index);
};

MInstrumentEditorFactory* MInstrumentEditorFactory::getInstance()
{
	if( ivSharedInstance == NULL )
	{
		ivSharedInstance = new MInstrumentEditorFactory();
	}

	return ivSharedInstance;
};

void MInstrumentEditorFactory::release()
{
	SAFE_DELETE( ivSharedInstance );
};

MInstrumentEditorFactory* MInstrumentEditorFactory::ivSharedInstance = NULL;
