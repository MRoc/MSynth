#ifndef __MAudioTriggerList
#define __MAudioTriggerList

#include "MAudioTrigger.h"
#include "../note/mdefaultnotelist.h"

class MAudioTriggerList :
	public MDefaultNoteList,
	public IInstrumentListener
{
public:

	static const int TYPE_ID;
	static char* TYPE_USER_NAME;

public:

	FULL_RTTI_SUPER( MAudioTriggerList, "MAudioTriggerList", MDefaultNoteList );

	MAudioTriggerList();
	MAudioTriggerList( MAudioTriggerList* ptList );
	virtual ~MAudioTriggerList();

	virtual IAbstractNote*		createDefaultNote();

	virtual void				applyDynamics( unsigned int index, FP value );

	virtual MColor				getPreferedColor();					

	virtual MDefaultNoteList*	createNewInstance();

	virtual int					getTypeId();
	virtual ICopyCutPaste*		getCopy();

	virtual MTreeNode*			save();
	virtual void				load( MTreeNode* node );

	virtual char*				getUserTypeName();

	virtual void				stateChanged( MInstrumentEvent anEvent );
	virtual String				getInstrumentListenerName();

	static MDefaultNoteList*	createInstance();

	virtual bool				canInsertAt( MAudioTrigger* ptTrigger, unsigned int col, unsigned int duration );

	virtual bool				hasVolumeControl();
};

#endif