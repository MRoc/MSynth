 /*

  MBeatBoxMessage (C)2001 MRoc hifiShock

  Prototype of a beatbox message with an volume

*/

#ifndef __MBeatBoxMessage
#define __MBeatBoxMessage

#include "MMessage.h"

class MBeatBoxMessage :
	public MMessage
{
protected:

	FP				ivVolume;

public:

	MBeatBoxMessage();
	MBeatBoxMessage( int id );
	MBeatBoxMessage(  int id, FP volume );
	MBeatBoxMessage( MBeatBoxMessage* message );
	virtual ~MBeatBoxMessage();

	virtual void		setVolume( FP volume );
	virtual FP			getVolume();

	virtual MTreeNode*	save();
	virtual void		load( MTreeNode* node );

	virtual MMessage*	getCopy();
};

#endif