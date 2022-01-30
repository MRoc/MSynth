/*

  MMessage (C)2001 MRoc hifiShock

  Prototype of a message with an ID

*/

#ifndef __MMessage
#define __MMessage

#include <framework/treedocument/ISerializeable.h>
#include <framework/primitive/MInt.h>
#include <framework/primitive/MFloat.h>

class MMessage :
	public ISerializeable
{
protected:

	int					ivId;

public:

	MMessage();
	MMessage( MMessage* message );
	MMessage( int id );
	virtual ~MMessage();

	virtual void		setId( int id );
	virtual int			getId();

	virtual MTreeNode*	save();
	virtual void		load( MTreeNode* node );

	virtual MMessage*	getCopy();
};

#endif