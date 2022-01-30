/*
	(C)2001 MRoc hifiShock
*/

#ifndef __MDefaultMessageNoteList
#define __MDefaultMessageNoteList

#include "MDefaultMessageNote.h"
#include "..\list\MListUtil.h"
#include "MDefaultNoteList.h"
#include "MMEssage.h"

class MDefaultMessageNoteList :
	public MDefaultNoteList
{
public:

	static int				TYPE_ID;
	static char*			TYPE_USER_NAME;

public:

	MDefaultMessageNoteList();
	MDefaultMessageNoteList( MDefaultMessageNoteList* ptNoteList );
	virtual ~MDefaultMessageNoteList();

	// MMessage functions
	virtual void			applyDynamics( unsigned int index, FP value );
	virtual MDefaultMessageNote* postMessage( MMessage *message, int index );
	virtual MMessage*		getMessageForId( int index, int id );
	virtual void			deleteMessageForId( int index, int id );
	virtual void			removeMessageForId( int index, int id );
	virtual void			cutId( int id );
	virtual void			setHighestId( int id );
	virtual MMessage*		createDefaultMessage();
	virtual bool			canPostMessageAt( unsigned int index, int id );

	// Note functions
	virtual IAbstractNote*	createDefaultNote();

	// Serialisation
	virtual MTreeNode*		save();
	virtual void			load( MTreeNode* node );

	virtual MColor			getPreferedColor();

	virtual MDefaultNoteList* createNewInstance();

	static MDefaultNoteList* createInstance();

	virtual ICopyCutPaste*	getCopy();
	virtual int				getTypeId();
	virtual char*			getUserTypeName();
};

#endif
