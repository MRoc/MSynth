/*
	(C)2001 MRoc hifiShock
*/

#ifndef __MBeatBoxNoteList
#define __MBeatBoxNoteList

#include "MDefaultMessageNoteList.h"
#include "../MBeatBox.h"
#include "../MAbstractInstrumentListener.h"

class MBeatBoxNoteList :
	public MDefaultMessageNoteList,
	public IInstrumentListener
{
public:

	static int				TYPE_ID;
	static char*			TYPE_USER_NAME;

public:

	FULL_RTTI_SUPER( MBeatBoxNoteList,"MBeatBoxNoteList", MDefaultMessageNoteList );
	MBeatBoxNoteList();
	MBeatBoxNoteList( MBeatBoxNoteList* ptNoteList );

	virtual ~MBeatBoxNoteList();

	virtual void			applyDynamics( unsigned int index, FP value );

	virtual void			stateChanged( MInstrumentEvent anEvent );

	virtual ICopyCutPaste*	getCopy();
	virtual int				getTypeId();
	virtual char*			getUserTypeName();

	virtual MDefaultNoteList* createNewInstance();

	static MDefaultNoteList *createInstance();

	virtual void updateFromInstrument( MInstrument* ptInstrument );

	/**
	* inheritances from IInstrumentListener
	*/
	virtual String			getInstrumentListenerName();

	/**
	* inheritances from IInstrumentListener
	*/
	virtual IInstrumentListener* getInstrumentListener();
};

#endif
































