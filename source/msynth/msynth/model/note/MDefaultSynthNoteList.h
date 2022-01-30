/*
	(C)2001 MRoc hifiShock
*/

#ifndef __MDefaultSynthNoteList
#define __MDefaultSynthNoteList

#include "MDefaultNoteList.h"
#include "MDefaultSynthNote.h"
#include "..\list\MListUtil.h"

class MDefaultSynthNoteList :
	public MDefaultNoteList
{
public:

	static int				TYPE_ID;
	static char*			TYPE_USER_NAME;

public:

	MDefaultSynthNoteList();
	MDefaultSynthNoteList( MDefaultSynthNoteList* ptNoteList );
	virtual ~MDefaultSynthNoteList();

	virtual void			applyDynamics( unsigned int index, FP value );

	virtual int				getAverageNoteValue();

	virtual bool			canInsertAt( MDefaultSynthNote *note, unsigned int index, unsigned int length, unsigned int value );
	virtual unsigned int	getOctaveCount();

	virtual int				getMaxPossibleLength( MDefaultSynthNote *ptNote );

	virtual IAbstractNote	*createDefaultNote();

	virtual MTreeNode*		save();
	virtual void			load( MTreeNode* node );

	virtual MColor			getPreferedColor();

	virtual MDefaultNoteList* createNewInstance();

	virtual ICopyCutPaste*	getCopy();
	virtual int				getTypeId();
	virtual char*			getUserTypeName();


	static MDefaultNoteList* createInstance();
};

#endif
































