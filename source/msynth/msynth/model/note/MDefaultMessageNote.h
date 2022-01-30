/*

  MDefaultMessageNote (C)2001 MRoc hifiShock

  A note containing some MMessages
*/

#ifndef __MDefaultMessageNote
#define __MDefaultMessageNote

#include "MDefaultNote.h"
#include "MMessage.h"
#include "MBeatBoxMessage.h"

class MDefaultMessageNote :
	public MDefaultNote
{
protected:

	std::vector<MMessage*> ivMessages;

public:

	MDefaultMessageNote();
	MDefaultMessageNote( MDefaultMessageNote* ptNote );

	virtual ~MDefaultMessageNote();

	virtual void			setDuration( unsigned int duration );

	virtual void			addMessage( MMessage *ptMessage );

	virtual unsigned int	getMessageCount();
	virtual MMessage*		getMessageAt( unsigned int index );

	virtual MMessage*		getMessageById( int id );
	virtual void			removeMessageAt( unsigned int index );
	virtual void			removeMessage( MMessage *ptMessage );
	virtual void			removeMessageForId( int id );
	virtual void			deleteMessageAt( unsigned int index );
	virtual void			deleteMessageForId( int id );
	virtual int				getIndexById( int id );
	virtual int				getIndexByMessage( MMessage* ptMessage );
	virtual bool			hasMessage( MMessage* ptMessage );
	virtual void			deleteContent();

	virtual bool			isOverlapping( IAbstractNote *note );
	virtual bool			wouldOverlapp( int index, unsigned int noteLength );

	virtual MTreeNode*		save();
	virtual void			load( MTreeNode* node );

	virtual MDefaultNote*	getCopy();

	virtual MDefaultNote*	createNewInstance();
};
#endif